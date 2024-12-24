#include "PID.h"
#include <math.h>
#include <stdio.h>
#include "Receiver.h"
#include "AHRS.h"
#include "stm32f4xx.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_rcc.h"
#include "GY86.h"
#include "OLED.h"
#include "ATKBLE01.h"

#define innerkP 3.0
#define innerkI 0.0
#define innerkD 0.0

#define outterkP 5.0
#define outterkI 2.0
#define outterkD 0.3

const float OUTER_INT_MAX = 40.0f;      // 外环积分限幅，对应角度误差累积和
const float INNER_INT_MAX = 40.0f;      // 内环积分限幅，对应角速度误差累积和
const float PWM_OUT_MAX = 300.0f;       // PWM 输出限幅，300.0 对应 30% 的油门
const float TAKEOFF_THROTTLE = 1450.0f; // 起飞油门，飞行时的油门应大于此值

// 对应 roll 内环、 roll 外环、 pitch 内环、 pitch 外环、 yaw 外环、 yaw 内环
PID_t rollInner, rollOuter, pitchInner, pitchOuter, yawOuter, yawInner;
// 期望的 roll, pitch, yaw 角速度和油门（即遥控器发出的信号）
float expRoll, expPitch, expYaw, expMode;
// PID 输出的油门控制量（范围在 [-PWM_OUT_MAX, PWM_OUT_MAX]）
float pidRoll, pidPitch, pidYaw, pidThr;
enum YAW_STATE {
    YAW_STABLE, // 油门大于 TAKEOFF_THROTTLE 且期望角速度为 0，此时采用串级 PID 控制
    YAW_ROTATE  // 否则为此状态，此时采用单环 PID 控制
};
enum FLY_STATE {
    HOVER,      // 悬停
    NOT_HOVER   // 非悬停
};
enum CH5_STATE { // 遥控器 CH5 三档开关状态，状态发生改变则进行机械校准
    CH5_DOWN,
    CH5_MIDDLE,
    CH5_UP
};
enum YAW_STATE yawState = YAW_STABLE;
enum FLY_STATE flyState = NOT_HOVER;
enum CH5_STATE ch5State = CH5_DOWN;
float targetYaw = 0.0f, targetHeight = 200.0f;



extern Angle angle;
extern float deltaT, height;
extern int correctFlag;
void TIM4_Init();
void pidInit() {
    // roll
    rollOuter.Kp = outterkP;
    rollOuter.Ki = outterkI;
    rollOuter.Kd = outterkD;

    rollInner.Kp = innerkP;
    rollInner.Ki = innerkI;
    rollInner.Kd = innerkD;

    // pitch
    pitchOuter.Kp = outterkP;
    pitchOuter.Ki = outterkI;
    pitchOuter.Kd = outterkD;

    pitchInner.Kp = innerkP;
    pitchInner.Ki = innerkI;
    pitchInner.Kd = innerkD;

    // Yaw
    yawOuter.Kp = outterkP;
    yawOuter.Ki = outterkI;
    yawOuter.Kd = outterkD;

    yawInner.Kp = innerkP;
    yawInner.Ki = innerkI;
    yawInner.Kd = innerkD;

    TIM4_Init();
}

static float limit(float x, float minv, float maxv) {
    return x < minv ? minv : (x > maxv ? maxv : x);
}

/**
 * @brief 串级 PID 控制
 * 
 * @param errTheta 误差角度
 * @param omega 对应轴的角速度（单位：度/秒）
 * @param outer 外环 PID
 * @param inner 内环 PID
 * @return float 电机输出
 */
float serialPIDcontrol(float errTheta, float omega, PID_t *outer, PID_t *inner) {
    outer->err = errTheta;
    if (expMode > TAKEOFF_THROTTLE) {
        outer->errSum = limit(outer->errSum + outer->err * deltaT, -OUTER_INT_MAX, OUTER_INT_MAX);
    } else {
        outer->errSum = 0.0f;
    }
    outer->outputP = outer->Kp * outer->err;
    outer->outputI = outer->Ki * outer->errSum;
    outer->outputD = -outer->Kd * omega;
    outer->output = outer->outputP + outer->outputI + outer->outputD;

    inner->err = outer->output - omega;
    if (expMode > TAKEOFF_THROTTLE) {
        inner->errSum = limit(inner->errSum + inner->err * deltaT, -INNER_INT_MAX, INNER_INT_MAX);
    } else {
        inner->errSum = 0.0f;
    }
    // inner->output = inner->Kp * inner->err + inner->Ki * inner->errSum + inner->Kd * (inner->err - inner->errLast);
    inner->outputP = inner->Kp * inner->err;
    inner->outputI = inner->Ki * inner->errSum;
    inner->outputD = inner->Kd * (inner->err - inner->errLast);
    // inner->outputD = inner->Kd * (inner->errLast - omega);
    inner->output = inner->outputP + inner->outputI + inner->outputD;
    inner->errLast = inner->err;
    if (++inner->cnt == 10) {
        inner->errLast = inner->err;
        // inner->errLast = omega;
        inner->cnt = 0;
    }
    // printf("outer->output = %f omega = %f errLast = %f err = %f outputD = %f\r\n", outer->output, omega, inner->errLast, inner->err, inner->outputD);
    inner->output = limit(inner->output, -PWM_OUT_MAX, PWM_OUT_MAX);

    return inner->output;
}

float singlePIDcontrol(float omega, PID_t *pid) {
    pid->err = expYaw - omega;
    if (expMode > TAKEOFF_THROTTLE) {
        pid->errSum = limit(pid->errSum + pid->err * deltaT, -OUTER_INT_MAX, OUTER_INT_MAX);
    } else {
        pid->errSum = 0.0f;
    }
    pid->outputP = pid->Kp * pid->err;
    pid->outputI = pid->Ki * pid->errSum;
    pid->outputD = pid->Kd * (pid->err - pid->errLast);
    if (++pid->cnt == 10) {
        pid->errLast = pid->err;
        pid->cnt = 0;
    }
    pid->output = pid->outputP + pid->outputI + pid->outputD;
    pid->output = limit(pid->output, -PWM_OUT_MAX, PWM_OUT_MAX);

    return pid->output;
}

/**
 * @brief PID 控制入口
 * 
 * @param gyroFilterd 滤波后的陀螺仪数据（单位：弧度/秒）
 */
void pidControl() {
    float motor1, motor2, motor3, motor4;

    // 接收机信号解析，范围均在 [1000, 2000]
    // expRoll = limit((CH2[1] - 1500) * 0.02f, -10, 10);   // roll 在 [-10, 10] 度
    // expPitch = limit(-(CH2[2] - 1500) * 0.02f, -10, 10); // pitch 在 [-10, 10] 度
    // expYaw = limit(-(CH2[4] - 1500) * 0.1f, -50, 50);    // yaw 在 [-50, 50] 度/秒
    // expMode = limit(CH2[3], 1000, 1800);                 // 油门在 [1000, 1800]
        // 1. Roll 计算并限制范围
    float rollInput = (float)(CH2[1] - 1500);  // 中间变量：计算偏移量
    float rollScaled = rollInput * 0.02f;      // 中间变量：按比例缩放
    if (rollScaled > 10.0f) {                  // 限制上限
        expRoll = 10.0f;
    } else if (rollScaled < -10.0f) {          // 限制下限
        expRoll = -10.0f;
    } else {
        expRoll = rollScaled;                  // 正常值
    }

    // 2. Pitch 计算并限制范围
    float pitchInput = (float)(CH2[2] - 1500); // 中间变量：计算偏移量
    float pitchScaled = -pitchInput * 0.02f;   // 中间变量：按比例缩放并取反
    if (pitchScaled > 10.0f) {                 // 限制上限
        expPitch = 10.0f;
    } else if (pitchScaled < -10.0f) {         // 限制下限
        expPitch = -10.0f;
    } else {
        expPitch = pitchScaled;                // 正常值
    }

    // 3. Yaw 计算并限制范围
    float yawInput = (float)(CH2[4] - 1500);   // 中间变量：计算偏移量
    float yawScaled = -yawInput * 0.1f;        // 中间变量：按比例缩放并取反
    if (yawScaled > 50.0f) {                   // 限制上限
        expYaw = 50.0f;
    } else if (yawScaled < -50.0f) {           // 限制下限
        expYaw = -50.0f;
    } else {
        expYaw = yawScaled;                    // 正常值
    }

    // 4. 油门模式计算并限制范围
    int16_t modeInput = CH2[3];                // 中间变量：原始输入值
    if (modeInput > 1800) {                    // 限制上限
        expMode = 1800;
    } else if (modeInput < 1000) {             // 限制下限
        expMode = 1000;
    } else {
        expMode = modeInput;                   // 正常值
    }
    // expMode = limit((data[3] < 1200 ? 2.0f * data[3] - 1000.0f : 0.5f * data[3] + 800.0f), 1000, 1800); // 油门在 [1000, 1800]

    // 机械校准
    // enum CH5_STATE curCh5State = data[5] < 1400 ? CH5_DOWN : (data[5] < 1600 ? CH5_MIDDLE : CH5_UP);
    // if (curCh5State != ch5State) {
    //     ch5State = curCh5State;
    //     correctFlag = 0;
    // }

    // 串级 PID 控制
    // printf("expRoll = %f angle.roll = %f\r\n", expRoll, angle.roll);
    pidRoll = serialPIDcontrol(expRoll - angle.roll, GY86DataList.GX * RAD_TO_DEGREE, &rollOuter, &rollInner);
    pidPitch = serialPIDcontrol(expPitch - angle.pitch, GY86DataList.GY * RAD_TO_DEGREE, &pitchOuter, &pitchInner);
    // pidYaw = singlePIDcontrol(gyroFilterd[2] * RAD_TO_DEGREE, &yawInner);

    if (expMode > TAKEOFF_THROTTLE && fabsf(expYaw) < 5.0f) {
        if (yawState != YAW_STABLE) {
            yawState = YAW_STABLE;
            targetYaw = angle.yaw;
            yawOuter.errSum = yawInner.errSum = 0.0f;
        }
        float errYaw = targetYaw - angle.yaw;
        if (errYaw < -180.0f) {
            errYaw += 360.0f;
        } else if (errYaw > 180.0f) {
            errYaw -= 360.0f;
        }
        pidYaw = serialPIDcontrol(errYaw, GY86DataList.GZ * RAD_TO_DEGREE, &yawOuter, &yawInner);
    } else {
        if (yawState != YAW_ROTATE) {
            yawState = YAW_ROTATE;
            yawOuter.errSum = yawInner.errSum = 0.0f;
        } 
        pidYaw = singlePIDcontrol(GY86DataList.GZ * RAD_TO_DEGREE, &yawInner);
    }

    // 电机输出（1000 对应 0% 油门，2000 对应 100% 油门）
    motor1 = limit(expMode + pidRoll + pidPitch + pidYaw + pidThr, 1000, 2000);
    motor2 = limit(expMode - pidRoll + pidPitch - pidYaw + pidThr, 1000, 2000);
    motor3 = limit(expMode - pidRoll - pidPitch + pidYaw + pidThr, 1000, 2000);
    motor4 = limit(expMode + pidRoll - pidPitch - pidYaw + pidThr, 1000, 2000);

    if (expMode > 1010) {
        TIM_SetCompare1(TIM3, motor1);
        TIM_SetCompare2(TIM3, motor2);
        TIM_SetCompare3(TIM3, motor3);
        TIM_SetCompare4(TIM3, motor4);
        // // BLE_Printf("%4d,%4d,%4d,%4d\n",motor1,motor2,motor3,motor4);
        // OLED_ShowNum(2,1 ,motor1,4);
        // OLED_ShowNum(2,13,motor2,4);
        // OLED_ShowNum(4,1 ,motor4,4);
        // OLED_ShowNum(4,13,motor3,4);
    } else {
        TIM_SetCompare1(TIM3, expMode);
        TIM_SetCompare2(TIM3, expMode);
        TIM_SetCompare3(TIM3, expMode);
        TIM_SetCompare4(TIM3, expMode);
        // OLED_ShowNum(2,1 ,motor1,4);
        // OLED_ShowNum(2,13,motor2,4);
        // OLED_ShowNum(4,1 ,motor4,4);
        // OLED_ShowNum(4,13,motor3,4);
    }
}

void TIM4_Init() {
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 65535;
	TIM_TimeBaseInitStructure.TIM_Prescaler = 16 - 1; // 计数器频率为 16MHz

	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStructure);
	TIM_ARRPreloadConfig(TIM4, ENABLE);
	TIM_Cmd(TIM4, ENABLE);
}
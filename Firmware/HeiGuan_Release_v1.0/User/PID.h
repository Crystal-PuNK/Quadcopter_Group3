#ifndef __PID_H__
#define __PID_H__

// PID 控制结构体
typedef struct {
    float err;      // 本次误差
    float errLast;  // 上次误差
    float errSum;   // 误差累积和
    float Kp;       // 比例系数
    float Ki;       // 积分系数
    float Kd;       // 微分系数
    float output;   //PID 输出
    float outputP;  // 比例输出
    float outputI;  // 积分输出
    float outputD;  // 微分输出
    int cnt;
} PID_t;

void pidInit();
void pidControl();

#endif
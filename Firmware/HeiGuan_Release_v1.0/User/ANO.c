#include"stm32f4xx.h"
#include <stdio.h>
#include <string.h>
#include "AHRS.h"
#include "PID.h"
#include"stm32f4xx_usart.h"
#include "GY86.h"

extern Angle angle;
extern float height;
extern volatile float q0, q1, q2, q3;
extern PID_t rollInner, rollOuter, pitchInner, pitchOuter, yawInner, heightSingle;

int _write(int fd, char *pBuffer, int size) {
    for (int i = 0; i < size; i++) {
        while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) {};
        USART_SendData(USART1, (u16)pBuffer[i]);
    }
    return size;
}

void addCheckSum(uint8_t *data) {
	uint8_t sumCheck = 0;
	uint8_t addCheck = 0;
	for (uint8_t i = 0; i < data[3] + 4; ++i) {
		sumCheck += data[i];  	   // 从帧头开始，对每一字节进行求和，直到DATA区结束
		addCheck += sumCheck;      // 每一字节的求和操作，进行一次sumcheck的累加
	}
	data[data[3] + 4] = sumCheck;  // 将 sumcheck 的值赋给帧尾的第一个字节
	data[data[3] + 5] = addCheck;  // 将 addcheck 的值赋给帧尾的第二个字节
}

void sendQuaternion () {
	uint8_t data[15];
	data[0] = 0xAA, data[1] = 0xFF, data[2] = 0x04;
	data[3] = 9; // 数据长度
	int16_t t = q0 * 10000;
	*((int16_t*)&data[4]) = t; 
	t = q1 * 10000;
	*((int16_t*)&data[6]) = t;
	t = q2 * 10000;
	*((int16_t*)&data[8]) = t;
	t = q3 * 10000;
	*((int16_t*)&data[10]) = t;
	data[12] = 0;
	addCheckSum(data);
	_write(0, (char*)data, sizeof(data));
}

void sendAttitudeInEulerAngle() {
    uint8_t data[13];
    data[0] = 0xAA, data[1] = 0xFF, data[2] = 0x03;
    data[3] = 7; // 数据长度
    int16_t t = angle.roll * 100;
    *((int16_t*)&data[4]) = t;
    t = -angle.pitch * 100;
    *((int16_t*)&data[6]) = t;
    t = -angle.yaw * 100;
    *((int16_t*)&data[8]) = t;
    data[10] = 0;
    addCheckSum(data);
    _write(0, (char*)data, sizeof(data));
}

void sendMotorState() {
	uint8_t data[14];
	data[0] = 0xAA, data[1] = 0xFF, data[2] = 0x20;
	data[3] = 8; // 数据长度
	uint16_t t = TIM3->CCR1;
	*((uint16_t*)&data[4]) = t;
	t = TIM3->CCR2;
	*((uint16_t*)&data[6]) = t;
	t = TIM3->CCR3;
	*((uint16_t*)&data[8]) = t;
	t = TIM3->CCR4;
	*((uint16_t*)&data[10]) = t;
	addCheckSum(data);
	_write(0, (char*)data, sizeof(data));
}

void sendIMUstate2() {
    uint8_t data[20];
    data[0] = 0xAA, data[1] = 0xFF, data[2] = 0x02;
    data[3] = 14; // 数据长度
    int16_t t = (int16_t)GY86DataList.GaX * 100;
    *((int16_t*)&data[4]) = t;
    t = (int16_t)GY86DataList.GaY * 100;
    *((int16_t*)&data[6]) = t;
    t = (int16_t)GY86DataList.GaZ * 100;
    *((int16_t*)&data[8]) = t;
    *((int32_t*)&data[10]) = 0;
    t = (int16_t)(GY86DataList.CORE_Temperature * 100);
    *((int16_t*)&data[14]) = t;
    *((uint8_t*)&data[16]) = 0;
    *((uint8_t*)&data[17]) = 0;
    addCheckSum(data);
    _write(0, (char*)data, sizeof(data));
}

extern float deltaT, targetYaw;
void sendIMUstate1() {
    uint8_t data[19];
    data[0] = 0xAA, data[1] = 0xFF, data[2] = 0x01;
    data[3] = 13; // 数据长度
    int16_t t = GY86DataList.AX;
    *((int16_t*)&data[4]) = t;
    t = GY86DataList.AY;
    *((int16_t*)&data[6]) = t;
    t = GY86DataList.AZ;
    *((int16_t*)&data[8]) = t;
    t = GY86DataList.GX * RAD_TO_DEGREE;
    *((int16_t*)&data[10]) = t;
    t = GY86DataList.GY * RAD_TO_DEGREE;
    *((int16_t*)&data[12]) = t;
    t = GY86DataList.GZ * RAD_TO_DEGREE;
    *((int16_t*)&data[14]) = t;
    *((uint8_t*)&data[15]) = 0;
    addCheckSum(data);
    _write(0, (char*)data, sizeof(data));
}

extern float expRoll, expPitch, expMode, expYaw;
extern float pidRoll, pidPitch, pidThr, targetHeight;
void sendPidInfo() {
    uint8_t data[26];
    data[0] = 0xAA, data[1] = 0xFF, data[2] = 0xF2;
    data[3] = 20; // 数据长度
    int16_t t = angle.roll * 10.0f;
    *((int16_t*)&data[4]) = t;
    t = angle.pitch * 10.0f;
    *((int16_t*)&data[6]) = t;
    t = angle.yaw * 10.0f;
    *((int16_t*)&data[8]) = t;
    t = expRoll * 10.0f;
    *((int16_t*)&data[10]) = t;
    t = expPitch * 10.0f;
    *((int16_t*)&data[12]) = t;
    t = pidRoll * 10.0f;
    *((int16_t*)&data[14]) = t;
    t = pidPitch * 10.0f;
    *((int16_t*)&data[16]) = t;
    t = rollOuter.errSum * 10.0f;
    *((int16_t*)&data[18]) = t;
    t = targetHeight * 10.0f;
    *((int16_t*)&data[20]) = t;
    t = yawInner.output * 10.0f;
    *((int16_t*)&data[22]) = t;
    addCheckSum(data);
    _write(0, (char*)data, sizeof(data));
}

void sendPidInfo2() {
    uint8_t data[26];
    data[0] = 0xAA, data[1] = 0xFF, data[2] = 0xF3;
    data[3] = 20; // 数据长度
    int16_t t = rollInner.outputP * 10.0f;
    *((int16_t*)&data[4]) = t;
    t = expYaw * 10.0f;
    *((int16_t*)&data[6]) = t;
    t = rollInner.outputD * 10.0f;
    *((int16_t*)&data[8]) = t;
    t = rollOuter.outputP * 10.0f;
    *((int16_t*)&data[10]) = t;
    t = rollOuter.outputI * 10.0f;
    *((int16_t*)&data[12]) = t;
    t = pidThr * 10.0f;
    *((int16_t*)&data[14]) = t;
    t = rollOuter.output * 10.0f;
    *((int16_t*)&data[16]) = t;
    t = rollInner.err * 10.0f;
    *((int16_t*)&data[18]) = t;
    t = rollInner.errLast * 10.0f;
    *((int16_t*)&data[20]) = t;
    t = expMode * 10.0f;
    *((int16_t*)&data[22]) = t;
    addCheckSum(data);
    _write(0, (char*)data, sizeof(data));
}

void sendInfo() {
	sendQuaternion();
    // sendEulerAngle();
    sendIMUstate1();
    sendIMUstate2();
    // sendPidInfo();
    // sendPidInfo2();
	// sendMotorState();
}

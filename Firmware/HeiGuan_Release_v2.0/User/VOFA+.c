#include "VOFA+.h"
#include "stm32f4xx.h"
#include <stdio.h>
#include <string.h>
#include "AHRS.h"
#include "PID.h"
#include "stm32f4xx_usart.h"
#include "GY86.h"

extern Angle angle;
extern float height;
extern volatile float q0, q1, q2, q3;
extern PID_t rollInner, rollOuter, pitchInner, pitchOuter, yawInner, heightSingle;

#define CH_COUNT 8

float fdata[CH_COUNT];
unsigned char tail[4] = {0x00, 0x00, 0x80, 0x7f};

extern float expRoll, expPitch, expMode, expYaw;
extern float pidRoll, pidPitch, pidThr, targetHeight;

int _write(int fd, char *pBuffer, int size) {
    for (int i = 0; i < size; i++) {
        while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) {};
        USART_SendData(USART1, (u16)pBuffer[i]);
    }
    return size;
}

void sendVOFA() {
    fdata[0] = angle.roll;
    fdata[1] = expRoll;
    fdata[2] = expMode;
    fdata[3] = pidRoll;
    fdata[4] = GY86DataList.GY * RAD_TO_DEGREE;
    fdata[5] = rollInner.outputP;
    fdata[6] = TIM4->CNT;
    fdata[7] = rollInner.outputD;
    _write(0,(char*)fdata,sizeof(fdata));
    _write(0,(char*)tail,sizeof(tail));
}
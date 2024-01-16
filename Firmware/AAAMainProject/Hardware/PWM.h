#ifndef __PWM_H
#define __PWM_H
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include <stdio.h>
#include <Delay.h>

/* Defines -------------------------------------------------------------------*/ 


/* Public Functions ------------------------------------------------------------------*/
void Motor_Init();
void Motor_SetSpeed_All(int8_t speed);
#endif


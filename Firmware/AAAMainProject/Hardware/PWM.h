#ifndef __PWM_H
#define __PWM_H
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include <stdio.h>


/* Defines -------------------------------------------------------------------*/ 


/* Functions ------------------------------------------------------------------*/
void PWM_Init();
void PWM_SetCompareAll(uint16_t Compare);

#endif

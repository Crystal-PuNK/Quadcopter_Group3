#ifndef __ATKBLE01_H
#define __ATKBLE01_H
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include <stdio.h>
#include <stdarg.h>

/* Defines -------------------------------------------------------------------*/ 


/* Functions ------------------------------------------------------------------*/
void BLE_Init(void);

void BLE_SendArray(uint8_t *Array, uint16_t Length);
void BLE_SendString(char *String);
void BLE_SendNumber(uint32_t Number, uint8_t Length);
void BLE_Printf(char *format, ...);


#endif

	

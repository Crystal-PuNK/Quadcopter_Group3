#ifndef __MPU6050_H
#define __MPU6050_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "Delay.h"
#include "MyIIC.h"

/* Defines -------------------------------------------------------------------*/

#define MPU6050_ADDRESS 0xD0


/* Functions -------------------------------------------------------------------*/

void MPU6050_init(void);
uint8_t MPU6050_ReadRegister(uint8_t RegAddress);
void MPU6050_WriteRegister(uint8_t RegAddress, uint8_t Data);

#endif

#ifndef __MYIIC_H
#define __MYIIC_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "Delay.h"

/* Defines -------------------------------------------------------------------*/ 
#define SCL_PORT   GPIOB
#define SCL_PIN    GPIO_Pin_8

#define SDA_PORT   GPIOB
#define SDA_PIN    GPIO_Pin_9

/* Functions ------------------------------------------------------------------*/

uint8_t MyIIC_R_SCL(void);


void MyIIC_Init(void);

void MyIIC_Start(void);
void MyIIC_Stop(void);

void MyIIC_SendByte(uint8_t Byte);
uint8_t MyIIC_ReceiveByte(void);

void MyIIC_ACK(uint8_t AckBit);
uint8_t MyIIC_ReceiveACK(void);

#endif


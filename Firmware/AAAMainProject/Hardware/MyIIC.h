#ifndef __MYIIC_H
#define __MYIIC_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "Delay.h"


/*Public Functions ------------------------------------------------------------------*/

void    MyIIC_Init(void);

void    MyIIC_Start(void);
void    MyIIC_Stop(void);

void    MyIIC_SendByte(uint8_t Byte);
uint8_t MyIIC_ReceiveByte(void);

void MyIIC_SendACK(uint8_t AckBit);
uint8_t MyIIC_ReceiveACK(void);

#endif


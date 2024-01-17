#ifndef __LD2_H
#define __LD2_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "Delay.h"

/* Defines -------------------------------------------------------------------*/ 
#define LD2_PORT GPIOA
#define LD2_PIN  GPIO_Pin_5

/* Functions ------------------------------------------------------------------*/
void LD2_Toggle(uint64_t interval);
void LD2_OFF(void);
void LD2_ON(void);
void LD2_init(void);

#endif


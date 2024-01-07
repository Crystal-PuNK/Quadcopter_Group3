#ifndef __GY86_H
#define __GY86_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "Delay.h"
#include "MyIIC.h"


/* Public Variables -------------------------------------------------------------------*/
extern struct GY86_Data GY86DataList;

/* Public Functions -------------------------------------------------------------------*/

void GY86_init(void);

void GY86_GetData(void);

#endif

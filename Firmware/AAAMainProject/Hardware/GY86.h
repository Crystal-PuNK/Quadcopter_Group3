#ifndef __GY86_H
#define __GY86_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "Delay.h"
#include "MyIIC.h"


/* Public Variables -------------------------------------------------------------------*/
extern struct GY86_Data{
	int16_t AX;
	int16_t AY;
	int16_t AZ;
	int16_t CORE_Temperature;
	int16_t GX;
	int16_t GY;
	int16_t GZ;
	int16_t GaX;
	int16_t GaY;
	int16_t GaZ;
	int32_t Height;
}GY86DataList;

/* Public Functions -------------------------------------------------------------------*/

void GY86_init(void);

void GY86_GetData(void);

#endif

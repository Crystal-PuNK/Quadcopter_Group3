#ifndef __GY86_H
#define __GY86_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "Delay.h"
#include "MyIIC.h"
#include "Parameters.h"


/* Public Variables -------------------------------------------------------------------*/
typedef struct {
	float AX;
	float AY;
	float AZ;
	int16_t CORE_Temperature;
	float GX;
	float GY;
	float GZ;
	float GaX;
	float GaY;
	float GaZ;
	float Height;
}GY86_Data;

extern volatile GY86_Data GY86DataList;

/* Public Functions -------------------------------------------------------------------*/

void GY86_init(void);

void GY86_GetData(void);

#endif

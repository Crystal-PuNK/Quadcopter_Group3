/* Inculdes -------------------------------------------------------------------*/ 
#include "LD2.h"

/* Functions -------------------------------------------------------------------*/ 

/*********************************************************************************
 *板载绿色LED（LD2）的初始化，包括
 * - PA5的初始化
 *     - 推挽输出
 ********************************************************************************/
void LD2_init(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = LD2_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(LD2_PORT, &GPIO_InitStructure);
}

/*********************************************************************************
 *点亮/熄灭该灯
 ********************************************************************************/
void LD2_ON(void)
{
	GPIO_SetBits(LD2_PORT,LD2_PIN);
}

void LD2_OFF(void)
{
	GPIO_ResetBits(LD2_PORT,LD2_PIN);
}


/*********************************************************************************
 *以一定时间间隔闪烁该灯
 *  -参数：闪烁间隔 单位 ms
 ********************************************************************************/
void LD2_Toggle(uint64_t interval)
{
	LD2_ON();
	Delay_ms(interval);
	LD2_OFF();
	Delay_ms(interval);
}


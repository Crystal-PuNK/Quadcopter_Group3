/* Inculdes -------------------------------------------------------------------*/ 
#include "LD2.h"

/* Functions -------------------------------------------------------------------*/ 
uint16_t Global_a_UnInitial;
uint16_t Global_b_Initial = 1;
uint16_t Global_c_ZInitial = 0;
/*********************************************************************************
 *������ɫLED��LD2���ĳ�ʼ��������
 * - PA5�ĳ�ʼ��
 *     - �������
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
 *����/Ϩ��õ�
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
 *��һ��ʱ������˸�õ�
 *  -��������˸��� ��λ ms
 ********************************************************************************/
void LD2_Toggle(uint64_t interval)
{
	LD2_ON();
	Delay_ms(interval);
	LD2_OFF();
	Delay_ms(interval);
}


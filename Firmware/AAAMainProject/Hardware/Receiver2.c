/* Inculdes -------------------------------------------------------------------*/ 
#include "Receiver.h"

/* Functions -------------------------------------------------------------------*/ 
uint16_t CH2[9];
uint16_t i2;
uint16_t okay2 = 0;
/*********************************************************************************
 *初始化TIM2定时器  TIM2_CH2
 ********************************************************************************/
void REC_Init2(void)
{
	RCC->APB1ENR |= 1;   //打开TIM2时钟
	RCC->AHB1ENR |= 1;   //打开GPIOA时钟
	
	GPIOA->MODER |= 1<<3; //复用模式
	GPIOA->OSPEEDR |= 3<<2; //high speed
	GPIOA->AFR[0] &= ~(0xF<<4);
	GPIOA->AFR[0] |= 1<<4;  //复用AF1
	
	TIM2->ARR |= 65536-1;
	TIM2->PSC |= 16-1;
	
	TIM2->SMCR |= 6<<4;  //TL2FP2
	TIM2->SMCR |= 1<<2;  //复位模式 内部时钟源
	TIM2->CCMR1 |= 1<<8; //IC2映射到TL2上
	TIM2->CCMR1 |= 16<<12; //0xF滤波
	TIM2->CCER |= 1<<4;    //使能捕获寄存器
	TIM2->DIER |= 1<<2;    //捕获中断使能
	
	SCB->AIRCR = 0x05FA0000 | 0x500;  //划分优先组 NVIC_PriorityGroup_2
	
	NVIC->IP[28] = 0x9 << 4;          //抢占优先级2，响应优先级1
	NVIC->ISER[0] = 1 << 28;          //开启NVIC中断
	
	TIM2->CR1 |= 1;  //TIM2使能
}

void TIM2_IRQHandler(void)
{
	if ((TIM2->SR & (1<<2))!=0)
	{
		uint16_t CCR2 = TIM2->CCR2;
		if((CCR2 >= 3500) && (CCR2<= 11500)){
			okay2 = 1;
			i2=0;
		}
		if(okay2==1){
			CH2[i2] = CCR2;
			i2++;
			if(i2>=9){
				okay2 = 0;
			}
		}
		TIM2->SR &= (uint16_t)~0x4;
	}
}

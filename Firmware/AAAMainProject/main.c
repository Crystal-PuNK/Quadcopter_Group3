/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/main.c 
  * @author  MCD Application Team
  * @version V1.8.1
  * @date    27-January-2022
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2016 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup Template_Project
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
static __IO uint32_t uwTimingDelay;
RCC_ClocksTypeDef RCC_Clocks;

/* Private function prototypes -----------------------------------------------*/


/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
	
	
	
/* System -------------------------------------------------------------------*/ 
 
  /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       files before to branch to application main.
       To reconfigure the default setting of SystemInit() function, 
       refer to system_stm32f4xx.c file */

  /* SysTick end of count event each 10ms */
  RCC_GetClocksFreq(&RCC_Clocks);
  SysTick_Config(RCC_Clocks.HCLK_Frequency / 100);
  
  /* Add your application code here */
  /* Insert 50 ms delay */
  Delay(5);
  
	
	
	
	
	
/* Initials -------------------------------------------------------------------*/ 
	LD2_init();
	GY86_init();
	OLED_Init();
	OLED2_Init();
	BLE_Init();
	REC_Init2();
	
/* IIC_test-----SUCCESS--------------------------------------------------------------*/ 	

//	MyIIC_Start();
//	MyIIC_SendByte(0xD2);  //1101 000 0
//	uint8_t Ack = MyIIC_ReceiveACK();
//	MyIIC_Stop();
//	OLED_ShowString(1,1,"MPU6050");
//	OLED_ShowString(2,1,"ACK:");
//	OLED_ShowNum(2,5,Ack,1);


/* MPU6050_test-----SUCCESS--------------------------------------------------------------*/ 
//	uint8_t ID_MPU6050 = MPU6050_ReadRegister(0x75);
//	OLED_ShowString(1,1,"MPU6050_ID");
//	OLED_ShowHexNum(2,1,ID_MPU6050,2);

/* USART_test-----SUCCESS--------------------------------------------------------------*/ 



/* GY-86 TEST--------------------SUCCESS------------------------------------------------*/ 
/*	
  while (1)0
  {
	GY86_GetData();
	LD2_ON();
//	  
//	 
//	OLED_ShowString(1,1,"Acc");	
//	OLED_ShowString(1,8,"Gyro");
//	OLED_ShowSignedNum(2,1,GY86DataList.AX,5);
//	OLED_ShowSignedNum(3,1,GY86DataList.AY,5);
//	OLED_ShowSignedNum(4,1,GY86DataList.AZ,5);
//	OLED_ShowSignedNum(2,8,GY86DataList.GX,5);
//	OLED_ShowSignedNum(3,8,GY86DataList.GY,5);
//	OLED_ShowSignedNum(4,8,GY86DataList.GZ,5);

//	Delay_ms(1000);
//	OLED_Clear();
//	
	OLED_ShowString(1,1,"Temperature");
	OLED_ShowSignedNum(2,1,GY86DataList.CORE_Temperature,5);
	OLED_ShowString(3,1,"Pressure");
	OLED_ShowSignedNum(4,1,GY86DataList.Height,5);
	Delay_ms(100);
//	OLED_Clear();
//	
//	OLED_ShowString(1,1,"Gauss");
//	OLED_ShowSignedNum(2,1,GY86DataList.GaX,5);
//	OLED_ShowSignedNum(3,1,GY86DataList.GaY,5);
//	OLED_ShowSignedNum(4,1,GY86DataList.GaZ,5);
//	
//	Delay_ms(1000);
//	OLED_Clear();
	
	LD2_OFF();
//	Delay_ms(2000);
  }
	
*/


/* PWM TEST -------------SUCCESS-------------------------------------------------*/ 


//½âËøµçµ÷
//2ms(2000= 1s)
	Motor_Init();
	LD2_ON();
	while(1)
	{
		GY86_GetData();
		OLED_ShowSignedNum(1,1,GY86DataList.AX,5);
		OLED_ShowSignedNum(2,1,GY86DataList.AY,5);
		OLED_ShowSignedNum(3,1,GY86DataList.AZ,5);
		OLED_ShowSignedNum(1,8,GY86DataList.GX,5);
		OLED_ShowSignedNum(2,8,GY86DataList.GY,5);
		OLED_ShowSignedNum(3,8,GY86DataList.GZ,5);
		
		OLED_ShowSignedNum(4,1,GY86DataList.GaX,5);
		OLED_ShowSignedNum(4,8,GY86DataList.GaY,5);
		
		OLED2_ShowString(1,1,"Group3 LCCZD");
		
		OLED2_ShowNum(2,1,CH2[1],5);
		OLED2_ShowNum(2,8,CH2[2],5);
		
		OLED2_ShowNum(3,1,CH2[3],5);
		OLED2_ShowNum(3,8,CH2[4],5);
		
		OLED2_ShowNum(4,1,CH2[5],5);
		OLED2_ShowNum(4,8,CH2[6],5);
		
		BLE_Printf("Acc:%d-%d-%d\n",GY86DataList.AX,GY86DataList.AY,GY86DataList.AZ);
		BLE_Printf("G:%d-%d-%d\n",GY86DataList.GX,GY86DataList.GY,GY86DataList.GZ);

//		BLE_Printf("CH[1]:%d  CH[2]:%d\n",CH2[1],CH2[2]);
//		BLE_Printf("CH[3]:%d  CH[4]:%d\n",CH2[3],CH2[4]);
//		BLE_Printf("CH[5]:%d  CH[6]:%d\n",CH2[5],CH2[6]);
//		BLE_Printf("CH[7]:%d  CH[8]:%d\n",CH2[7],CH2[8]);

		Motor_SetSpeed_All((CH2[3]/10)-100);
		
	}

//	LD2_OFF();
//	PWM_SetCompare1(140-1);

}






























/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in milliseconds.
  * @retval None
  */
void Delay(__IO uint32_t nTime)
{ 
  uwTimingDelay = nTime;

  while(uwTimingDelay != 0);
}

/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
void TimingDelay_Decrement(void)
{
  if (uwTimingDelay != 0x00)
  { 
    uwTimingDelay--;
  }
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */



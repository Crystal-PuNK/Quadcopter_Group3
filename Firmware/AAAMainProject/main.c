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
	BLE_Init();
	
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

/* USART_test-----Failed--------------------------------------------------------------*/ 



/* MPU6050------------------------------------------------------------------------------*/ 
	

	



/* Infinite Loop -------------------------------------------------------------------*/ 
  while (1)
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



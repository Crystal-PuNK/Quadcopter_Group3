#include "main.h"
#include "os_cpu.h"
#include "os.h"
// #define DEBUG

//@ Task Priority  ------------------------------------------------------------*/
#ifdef DEBUG
  #define TEST_TASK_PRIO 10
  #define TEST1_TASK_PRIO 20
#endif
  #define TMP_TASK_PRIO 5
//@ Tasks' Stake   ------------------------------------------------------------*/
#ifdef DEBUG
  #define TEST_STK_SIZE  64
  OS_STK TEST_TASK_STK[TEST_STK_SIZE];
  #define TEST1_STK_SIZE  64
  OS_STK TEST1_TASK_STK[TEST1_STK_SIZE];
#endif
  #define TMP_STK_SIZE  64
  OS_STK TMP_TASK_STK[TMP_STK_SIZE];
//# Private define ------------------------------------------------------------*/

//# Private variables ---------------------------------------------------------*/
RCC_ClocksTypeDef RCC_Clocks;
//# Private function prototypes -----------------------------------------------*/
#ifdef DEBUG
  void test_task(void *pdata);
  void test1_task(void *pdata);
#endif
  void tmp_task(void *pdata);
//# Private functions ---------------------------------------------------------*/
/**
 * @brief main Function
 * 
 * @return int 
 */
int main(void)
{
//* SysTick Config  
  RCC_GetClocksFreq(&RCC_Clocks);
  SysTick_Config(RCC_Clocks.HCLK_Frequency / 1); /* SysTick end of count event each 1000ms */
//* Initial
  CPU_TS_TmrInit();
  LD2_init();
  BLE_Init();
  GY86_init();
  OLED_Init();
  OLED2_Init();
  Motor_Init();
  REC_Init2();
  OSInit();
  OS_TRACE_INIT();   // 初始化 SystemView
#ifdef DEBUG
  OSTaskCreate(test_task,(void*)0,(OS_STK*)&TEST_TASK_STK[TEST_STK_SIZE-1],TEST_TASK_PRIO);
  OSTaskCreate(test1_task,(void*)0,(OS_STK*)&TEST1_TASK_STK[TEST1_STK_SIZE-1],TEST1_TASK_PRIO);
#endif
  OSTaskCreate(tmp_task,(void*)0,(OS_STK*)&TMP_TASK_STK[TMP_STK_SIZE-1],TMP_TASK_PRIO);
  OSStart();
  return 0;
}

void tmp_task(void *pdata)
{
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
		
		// BLE_Printf("Acc:%d-%d-%d\n",GY86DataList.AX,GY86DataList.AY,GY86DataList.AZ);
		// BLE_Printf("G:%d-%d-%d\n",GY86DataList.GX,GY86DataList.GY,GY86DataList.GZ);

//		BLE_Printf("CH[1]:%d  CH[2]:%d\n",CH2[1],CH2[2]);
//		BLE_Printf("CH[3]:%d  CH[4]:%d\n",CH2[3],CH2[4]);
//		BLE_Printf("CH[5]:%d  CH[6]:%d\n",CH2[5],CH2[6]);
//		BLE_Printf("CH[7]:%d  CH[8]:%d\n",CH2[7],CH2[8]);

		Motor_SetSpeed_All((CH2[3]/10)-100);
  }
}



#ifdef DEBUG
void test_task(void *pdata)
{
  while(1)
  {
    Delay_ms(100);
    LD2_ON();
    OSTimeDly(5);
  }
}

void test1_task(void *pdata)
{
  while(1)
  {
    Delay_ms(100);
    LD2_OFF();
    OSTimeDly(10);
  }
}
#endif
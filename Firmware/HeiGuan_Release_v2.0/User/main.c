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




volatile float deltaT = 0.003f;
Angle angle;
float height = 0.0f;
int8_t correctFlag = -1;
RCC_ClocksTypeDef RCC_ClockFreq_test;
// quaternion of sensor frame relative to auxiliary frame
volatile float q0 = 1.0f, q1 = 0.0f, q2 = 0.0f, q3 = 0.0f;
float rollOffset=0,pitchOffset = 0,rollOffsetSum, pitchOffsetSum;
int32_t counttttt = 0;
// void toEulerAngles(Angle *angle) {
// 	angle->yaw   = atan2(2 * q1 * q2  + 2 * q0 * q3, -2 * q2 * q2 - 2 * q3* q3 + 1) * RAD_TO_DEGREE; 				// yaw, z-axis
// 	angle->pitch = asin(-2 * q1 * q3 + 2 * q0* q2) * RAD_TO_DEGREE - pitchOffset; 							    // pitch
// 	angle->roll  = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2* q2 + 1) * RAD_TO_DEGREE - rollOffset; // roll
//   // angle->yaw   = atan2(2 * q1 * q2  - 2 * q0 * q3, -2 * q2 * q2 - 2 * q3* q3 + 1) * RAD_TO_DEGREE; 				// yaw, z-axis
// 	// angle->pitch =  asin(-2 * q1 * q3 - 2 * q0* q2) * RAD_TO_DEGREE - pitchOffset; 							    // pitch
// 	// angle->roll  = atan2(2 * q2 * q3 - 2 * q0 * q1, -2 * q1 * q1 - 2 * q2* q2 + 1) * RAD_TO_DEGREE - rollOffset; // roll
// }
void toEulerAngles(Angle *angle) {
	angle->yaw = atan2(2 * q1 * q2  + 2 * q0 * q3, -2 * q2 * q2 - 2 * q3* q3 + 1) * RAD_TO_DEGREE; 				// yaw
	angle->roll = asin(-2 * q1 * q3 + 2 * q0* q2) * RAD_TO_DEGREE - rollOffset; 							    // pitch
	angle->pitch = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2* q2 + 1) * RAD_TO_DEGREE -pitchOffset; // roll
}

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  // RCC_GetClocksFreq(&RCC_ClockFreq_test);                              /* Get the frequencies of different on chip clocks. */
  // HSE_SetSysClock(4,84,2,7);  
  SystemInit();
  RCC_GetClocksFreq(&RCC_ClockFreq_test);                              /* Get the frequencies of different on chip clocks. */
/* Initials -------------------------------------------------------------------*/ 
	LD2_init();
	GY86_init();
	OLED_Init();
	OLED2_Init();
	BLE_Init();
	REC_Init2();
	Motor_Init();
	LD2_ON();
  LoadParameters();
  pidInit();
  // while (1)
  // {
  //   Motor_SetSpeed_All(CH2[3]);
  // }
  

  while (1)
  {
    int8_t boot_delay_s_times = 0;
    int8_t boot_flag = 0;
    int8_t para_flag = 0;
    int8_t para_PID_flag = 0;
    int8_t para_IMU_flag = 0;
    int8_t para_baro_flag = 0;
    int8_t save_flag = 0;
    OLED_ShowString(1,5,"HEI GUAN");
    if (CH2[5]<1100){
      OLED_ShowString(2,1,"* Boot Engines  ");
      OLED_ShowString(3,1,"  Parameter set ");
      OLED_ShowString(4,1,"  Save Changes  ");
      while (CH2[2]>1900 && CH2[1]<1100 && CH2[1] <1100 && CH2[2]>1900){
        Delay_s(1);
        boot_delay_s_times++;
        if (boot_delay_s_times==2){boot_flag = 1;break;}
      }
      while (boot_flag){    
        GY86_GetData();
        #ifdef DEBUG
        // Motor_SetSpeed_All((CH2[3]));
        // OLED_ShowString(2,1,"AX:");
        // OLED_ShowString(3,1,"AY:"); 
        // OLED_ShowString(4,1,"AZ:");
        // OLED_ShowFNum(2,4,GY86DataList.AX,10,8);
        // OLED_ShowFNum(3,4,GY86DataList.AY,10,8);
        // OLED_ShowFNum(4,4,GY86DataList.AZ,10,8);
        // OLED2_ShowString(2,1,"GX:");
        // OLED2_ShowString(3,1,"GY:");
        // OLED2_ShowString(4,1,"GZ:");
        // OLED2_ShowFNum(2,4,GY86DataList.GX,10,8);
        // OLED2_ShowFNum(3,4,GY86DataList.GY,10,8);
        // OLED2_ShowFNum(4,4,GY86DataList.GZ,10,8);
        
        #endif
        counttttt++;
        if(counttttt ==20){
          sendVOFA();
          counttttt=0;
        }
        // sendVOFA();
        deltaT = TIM4->CNT / 1000000.0f;
		    TIM4->CNT = 0;
        MahonyAHRSupdate(GY86DataList.GX , GY86DataList.GY , GY86DataList.GZ , 
						             GY86DataList.AX , GY86DataList.AY , GY86DataList.AZ , 
                         GY86DataList.GaX, GY86DataList.GaY, GY86DataList.GaZ);
        pidControl();
        toEulerAngles(&angle);
        // OLED_ShowNum(2,1,CH2[3],5);
        if (angle.roll > 90 || angle.pitch>90)
        {
          boot_flag = 0;
        }
        
        if (correctFlag != -1) {
          if (correctFlag == 0) {
            rollOffsetSum = params_ram.rollOffset = 0.0f;
            pitchOffsetSum = params_ram.pitchOffset = 0.0f;
          }
          rollOffsetSum += angle.roll;
          pitchOffsetSum += angle.pitch;
          if (++correctFlag == 100) {
            rollOffset = rollOffsetSum / 100.0f;
            pitchOffset = pitchOffsetSum / 100.0f;
            correctFlag = -1;
            // StoreParameters();
            // LoadParameters();
            // BLE_Printf("rolloffset:%f\n",rollOffset);
            // BLE_Printf("pitchoffset:%d\n",pitchOffset*10000);
            // Delay_s(10);
          }
        }
        if (boot_delay_s_times<2) boot_delay_s_times++;
        while (CH2[6] >1900){
          Motor_SetSpeed_All(0);
          correctFlag = 0;
          Delay_s(1);
          boot_delay_s_times--;
          if (boot_delay_s_times==0){boot_flag = 0;break;}
        }
        
      }
    }else if(CH2[5]<1600){ 
      OLED_ShowString(2,1,"  Boot Engines  ");
      OLED_ShowString(3,1,"* Parameter set ");
      OLED_ShowString(4,1,"  Save Changes  ");
      if (CH2[6] >1900) para_flag = 1;
      while (para_flag)
      {
        Delay_ms(100);
        if (CH2[5]<1100){  
          OLED_ShowString(2,1,"* PID setting   ");
          OLED_ShowString(3,1,"  IMU calibrate ");
          OLED_ShowString(4,1,"  Baro calibrate");
          if (CH2[6] >1900) para_PID_flag = 1;
          while (para_PID_flag)
          {
            while (CH2[5]<1100&&para_PID_flag){  
              OLED_ShowString(2,1,"* kp:");
              OLED_ShowString(3,1,"  ki:");
              OLED_ShowString(4,1,"  kd:");
              OLED_ShowFNum(2,6,params_ram.kp,8,3);
              OLED_ShowFNum(3,6,params_ram.ki,8,3);
              OLED_ShowFNum(4,6,params_ram.kd,8,3);
              if (CH2[4]<1100){
                params_ram.kp = params_ram.kp -1;
                Delay_s(1);
              }else if(CH2[4]>1100&&CH2[4]<1200){
                params_ram.kp = params_ram.kp -0.1;
                Delay_ms(500);
              }else if(CH2[4]>1200&&CH2[4]<1400){
                params_ram.kp = params_ram.kp -0.05;
                Delay_ms(500);
              }else if(CH2[4]>1600&&CH2[4]<1800){
                params_ram.kp = params_ram.kp +0.05;
                Delay_ms(500);
              }else if(CH2[4]>1800&&CH2[4]<1900){
                params_ram.kp = params_ram.kp +0.1;
                Delay_ms(500);
              }else if(CH2[4]>1900){
                params_ram.kp = params_ram.kp +1;
                Delay_s(1);
              }
              if (CH2[1]<1100) {para_PID_flag = 0;}
              Delay_ms(500);
            }
            while (CH2[5]>1200&&CH2[5]<1800&&para_PID_flag){ 
              OLED_ShowString(2,1,"  kp:");
              OLED_ShowString(3,1,"* ki:");
              OLED_ShowString(4,1,"  kd:");
              OLED_ShowFNum(2,6,params_ram.kp,8,3);
              OLED_ShowFNum(3,6,params_ram.ki,8,3);
              OLED_ShowFNum(4,6,params_ram.kd,8,3);
              if (CH2[4]<1100){
                params_ram.ki = params_ram.ki -1;
                Delay_s(1);
              }else if(CH2[4]>1100&&CH2[4]<1200){
                params_ram.ki = params_ram.ki -0.1;
                Delay_ms(500);
              }else if(CH2[4]>1200&&CH2[4]<1400){
                params_ram.ki = params_ram.ki -0.05;
                Delay_ms(500);
              }else if(CH2[4]>1600&&CH2[4]<1800){
                params_ram.ki = params_ram.ki +0.05;
                Delay_ms(500);
              }else if(CH2[4]>1800&&CH2[4]<1900){
                params_ram.ki = params_ram.ki +0.1;
                Delay_ms(500);
              }else if(CH2[4]>1900){
                params_ram.ki = params_ram.ki +1;
                Delay_s(1);
              }
              if (CH2[1]<1100) {para_PID_flag = 0;}
              Delay_ms(500);
            }
            while (CH2[5]>1800&&para_PID_flag){  
              OLED_ShowString(2,1,"  kp:");
              OLED_ShowString(3,1,"  ki:");
              OLED_ShowString(4,1,"* kd:");
              OLED_ShowFNum(2,6,params_ram.kp,8,3);
              OLED_ShowFNum(3,6,params_ram.ki,8,3);
              OLED_ShowFNum(4,6,params_ram.kd,8,3);
              if (CH2[4]<1100){
                params_ram.kd = params_ram.kd -1;
                Delay_s(1);
              }else if(CH2[4]>1100&&CH2[4]<1200){
                params_ram.kd = params_ram.kd -0.1;
                Delay_ms(500);
              }else if(CH2[4]>1200&&CH2[4]<1400){
                params_ram.kd = params_ram.kd -0.05;
                Delay_ms(500);
              }else if(CH2[4]>1600&&CH2[4]<1800){
                params_ram.kd = params_ram.kd +0.05;
                Delay_ms(500);
              }else if(CH2[4]>1800&&CH2[4]<1900){
                params_ram.kd = params_ram.kd +0.1;
                Delay_ms(500);
              }else if(CH2[4]>1900){
                params_ram.kd = params_ram.kd +1;
                Delay_s(1);
              }
              if (CH2[1]<1100) {para_PID_flag = 0;}
              Delay_ms(500);
            }
          }       
        }else if(CH2[5]<1600){
          OLED_ShowString(2,1,"  PID setting   ");
          OLED_ShowString(3,1,"* IMU calibrate ");
          OLED_ShowString(4,1,"  Baro calibrate");
          if (CH2[6] >1900) para_IMU_flag = 1;
          while (para_IMU_flag)
          {
            OLED_ShowString(2,1," IMU calibrating");
            OLED_ShowString(3,1,"                ");
            OLED_ShowString(4,1,"                ");
            IMU_Calibrate();
            OLED_ShowString(2,1," IMU calibrating");
            OLED_ShowString(3,1,"    Success!    ");
            OLED_ShowString(4,1,"                ");
            para_IMU_flag = 0;
            Delay_ms(500);
          }
        }else{
          OLED_ShowString(2,1,"  PID setting   ");
          OLED_ShowString(3,1,"  IMU calibrate ");
          OLED_ShowString(4,1,"* Baro calibrate");
          if (CH2[6] >1900) para_baro_flag = 1;
          while (para_baro_flag)
          {  
            OLED_ShowString(2,1,"Baro calibrating");
            OLED_ShowString(3,1,"                ");
            OLED_ShowString(4,1,"                ");
            // Baro_Calibrate();
            if (CH2[1]<1100) {para_baro_flag = 0;}
            Delay_ms(500);
          }
        }
        if (CH2[1]<1100) {para_flag = 0;}
      }    
    }else{    
      OLED_ShowString(2,1,"  Boot Engines  ");
      OLED_ShowString(3,1,"  Parameter set ");
      OLED_ShowString(4,1,"* Save Changes  ");
      if (CH2[6] >1900) save_flag = 1;
      while (save_flag)
      {
        OLED_ShowString(2,1," Saving Changes!");
        OLED_ShowString(3,1,"                ");
        OLED_ShowString(4,1,"                ");
        StoreParameters();
        LoadParameters();
        save_flag = 0;
      }
    }
  }
}


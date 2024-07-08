/**
  ******************************************************************************
  * @file    Delay.h
  * @author  XinLi
  * @version v1.0
  * @date    24-October-2017
  * @brief   Header file for Delay.c module.
  ******************************************************************************
  * @attention
  *
  * <h2><center>Copyright &copy; 2017 XinLi</center></h2>
  *
  * This program is free software: you can redistribute it and/or modify
  * it under the terms of the GNU General Public License as published by
  * the Free Software Foundation, either version 3 of the License, or
  * (at your option) any later version.
  *
  * This program is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  * GNU General Public License for more details.
  *
  * You should have received a copy of the GNU General Public License
  * along with this program.  If not, see <https://www.gnu.org/licenses/>.
  *
  ******************************************************************************
  */

#ifndef __DELAY_H
#define __DELAY_H

/* Header includes -----------------------------------------------------------*/
#include "stm32f4xx.h"

//@ Macro definitions ---------------------------------------------------------*/
/* 为方便使用，在延时函数内部调用CPU_TS_TmrInit函数初始化时间戳寄存器，
   这样每次调用函数都会初始化一遍。
   把本宏值设置为0，然后在main函数刚运行时调用CPU_TS_TmrInit可避免每次都初始化 */
  #define CPU_TS_INIT_IN_DELAY_FUNCTION   0

/* Type definitions ----------------------------------------------------------*/
/* Variable declarations -----------------------------------------------------*/
/* Variable definitions ------------------------------------------------------*/
//@ Function declarations -----------------------------------------------------*/
  // void SysTick_Init(u8 SYSCLK);
  void CPU_TS_TmrInit(void);
  uint32_t CPU_TS_TmrRd(void);
  void Delay_us(uint64_t nus);
  void Delay_ms(uint64_t nms);
  void Delay_s(uint64_t ns);
//使用以下函数前必须先调用CPU_TS_TmrInit函数使能计数器，或使能宏CPU_TS_INIT_IN_DELAY_FUNCTION
//最大延时值为60秒
  void CPU_TS_Tmr_Delay_US(uint32_t us);
  #define CPU_TS_Tmr_Delay_MS(ms)     CPU_TS_Tmr_Delay_US(ms*1000)
  #define CPU_TS_Tmr_Delay_S(s)       CPU_TS_Tmr_Delay_MS(s*1000)

//@ Function definitions ------------------------------------------------------*/
  #define Delay_ms(ms)     CPU_TS_Tmr_Delay_MS(ms)
  #define Delay_us(us)     CPU_TS_Tmr_Delay_US(us)
  #define Delay_s(s)     CPU_TS_Tmr_Delay_S(s) ///* 最大延时 60s=2的32次方/72000000 */
/* 获取内核时钟频率 */
  #define GET_CPU_ClkFreq()       (SystemCoreClock)
  #define SysClockFreq            (SystemCoreClock)

/*******************************************************************************
 *                       函数声明
 ******************************************************************************/


#endif

/**
  ******************************************************************************
  * @file    Delay.c
  * @author  XinLi
  * @version v1.0
  * @date    24-October-2017
  * @brief   Delay module driver.
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

/* Header includes -----------------------------------------------------------*/
#include "Delay.h"
#define OS_TICKS_PER_SEC 1
/* Macro definitions ---------------------------------------------------------*/
  //# USE DWT Peripherl
  #define  DWT_CR      *(__IO uint32_t *)0xE0001000
  #define  DWT_CYCCNT  *(__IO uint32_t *)0xE0001004
  #define  DEM_CR      *(__IO uint32_t *)0xE000EDFC


  #define  DEM_CR_TRCENA                   (1 << 24)
  #define  DWT_CR_CYCCNTENA                (1 <<  0)  
/* Type definitions ----------------------------------------------------------*/
/* Variable declarations -----------------------------------------------------*/
/* Variable definitions ------------------------------------------------------*/
/* Function declarations -----------------------------------------------------*/
/* Function definitions ------------------------------------------------------*/
// /**
//  * @brief 初始化延迟函数
//  * 当使用OS的时候,此函数会初始化OS的时钟节拍
//  * SYSTICK的时钟固定为AHB时钟的1/8
//  * SYSCLK:系统时钟频率
//  * @param SYSCLK 
//  */
// void SysTick_Init(u8 SYSCLK) {
//     u32 reload;
//     SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
//     reload = SYSCLK / 8;                   //每秒钟的计数次数 单位为M
//     reload *= 1000000 / OS_TICKS_PER_SEC;  //根据OS_TICKS_PER_SEC设定溢出时间
//                                            // reload为24位寄存器,最大值:16777216
//     SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;  //开启SYSTICK中断
//     SysTick->LOAD = reload;  //每1/OS_TICKS_PER_SEC秒中断一次
//     SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;  //开启SYSTICK
// }
/**
  * @brief  初始化时间戳
  * @param  无
  * @retval 无
  * @note   使用延时函数前，必须调用本函数
  */
void CPU_TS_TmrInit(void)
{
    /* 使能DWT外设 */
    DEM_CR |= (uint32_t)DEM_CR_TRCENA;

    /* DWT CYCCNT寄存器计数清0 */
    DWT_CYCCNT = (uint32_t)0u;

    /* 使能Cortex-M DWT CYCCNT寄存器 */
    DWT_CR |= (uint32_t)DWT_CR_CYCCNTENA;
}

/**
  * @brief  读取当前时间戳
  * @param  无
  * @retval 当前时间戳，即DWT_CYCCNT寄存器的值
  */
uint32_t CPU_TS_TmrRd(void)
{
    return ((uint32_t)DWT_CYCCNT);
}

/**
  * @brief  采用CPU的内部计数实现精确延时，32位计数器
  * @param  us : 延迟长度，单位1 us
  * @retval 无
  * @note   使用本函数前必须先调用CPU_TS_TmrInit函数使能计数器，
            或使能宏CPU_TS_INIT_IN_DELAY_FUNCTION
            最大延时值为8秒，即8*1000*1000
  */
void CPU_TS_Tmr_Delay_US(__IO uint32_t us)
{
    uint32_t ticks;
    uint32_t told,tnow,tcnt=0;

    /* 在函数内部初始化时间戳寄存器， */
#if (CPU_TS_INIT_IN_DELAY_FUNCTION)
    /* 初始化时间戳并清零 */
  CPU_TS_TmrInit();
#endif

    ticks = us * (GET_CPU_ClkFreq() / 1000000);  /* 需要的节拍数 */
    tcnt = 0;
    told = (uint32_t)CPU_TS_TmrRd();         /* 刚进入时的计数器值 */

    while(1)
    {
        tnow = (uint32_t)CPU_TS_TmrRd();
        if(tnow != told)
        {
            /* 32位计数器是递增计数器 */
            if(tnow > told)
            {
                tcnt += tnow - told;
            }
                /* 重新装载 */
            else
            {
                tcnt += UINT32_MAX - told + tnow;
            }

            told = tnow;

            /*时间超过/等于要延迟的时间,则退出 */
            if(tcnt >= ticks)break;
        }
    }
}

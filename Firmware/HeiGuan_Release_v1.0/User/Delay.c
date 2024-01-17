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

/* Macro definitions ---------------------------------------------------------*/
/* Type definitions ----------------------------------------------------------*/
/* Variable declarations -----------------------------------------------------*/
/* Variable definitions ------------------------------------------------------*/
/* Function declarations -----------------------------------------------------*/
/* Function definitions ------------------------------------------------------*/

/**
  * @brief  Microsecond delay.
  * @param  [in] nus: The number of microseconds delay.
  * @return None.
  */
void Delay_us(uint64_t nus)
{
  uint64_t nms = 0;
  
  if(nus == 0)
  {
    return;
  }
  
  nms = nus / 1000;
  nus = nus % 1000;
  
  if(nms > 0)
  {
    Delay_ms(nms);
  }
  
  if(nus > 0)
  {
    RCC_ClocksTypeDef RCC_ClockFreq;
    
    RCC_GetClocksFreq(&RCC_ClockFreq);                              /* Get the frequencies of different on chip clocks. */
    
    if(RCC_ClockFreq.HCLK_Frequency < 8000000)
    {
      SysTick->CTRL |= SysTick_CLKSource_HCLK;                      /* Configures the SysTick clock source. */
      SysTick->LOAD = RCC_ClockFreq.HCLK_Frequency / 1000000 * nus; /* Time load (SysTick-> LOAD is 24bit). */
    }
    else
    {
      SysTick->CTRL &= SysTick_CLKSource_HCLK_Div8;                 /* Configures the SysTick clock source. */
      SysTick->LOAD = RCC_ClockFreq.HCLK_Frequency / 8000000 * nus; /* Time load (SysTick-> LOAD is 24bit). */
    }
    
    SysTick->VAL = 0;                                               /* Empty counter. */
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;                       /* Start the countdown. */
    
    while((SysTick->CTRL&(1UL<<16)) != (1UL<<16));                  /* Wait time is reached. */
    
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;                      /* Close counter. */
  }
}

/**
  * @brief  Millisecond delay.
  * @param  [in] nms: The number of millisecond delay.
  * @return None.
  */
void Delay_ms(uint64_t nms)
{
  if(nms == 0)
  {
    return;
  }
  
  while(nms > 500)
  {
    RCC_ClocksTypeDef RCC_ClockFreq;
    
    RCC_GetClocksFreq(&RCC_ClockFreq);                            /* Get the frequencies of different on chip clocks. */
    
    if(RCC_ClockFreq.HCLK_Frequency < 8000000)
    {
      SysTick->CTRL |= SysTick_CLKSource_HCLK;                    /* Configures the SysTick clock source. */
      SysTick->LOAD = RCC_ClockFreq.HCLK_Frequency / 1000 * 500;  /* Time load (SysTick-> LOAD is 24bit). */
    }
    else
    {
      SysTick->CTRL &= SysTick_CLKSource_HCLK_Div8;               /* Configures the SysTick clock source. */
      SysTick->LOAD = RCC_ClockFreq.HCLK_Frequency / 8000 * 500;  /* Time load (SysTick-> LOAD is 24bit). */
    }
    
    SysTick->VAL = 0;                                             /* Empty counter. */
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;                     /* Start the countdown. */
    
    while((SysTick->CTRL&(1UL<<16)) != (1UL<<16));                /* Wait time is reached. */
    
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;                    /* Close counter. */
    
    nms -= 500;
  }
  
  RCC_ClocksTypeDef RCC_ClockFreq;
  
  RCC_GetClocksFreq(&RCC_ClockFreq);                              /* Get the frequencies of different on chip clocks. */
  
  if(RCC_ClockFreq.HCLK_Frequency < 8000000)
  {
    SysTick->CTRL |= SysTick_CLKSource_HCLK;                      /* Configures the SysTick clock source. */
    SysTick->LOAD = RCC_ClockFreq.HCLK_Frequency / 1000 * nms;    /* Time load (SysTick-> LOAD is 24bit). */
  }
  else
  {
    SysTick->CTRL &= SysTick_CLKSource_HCLK_Div8;                 /* Configures the SysTick clock source. */
    SysTick->LOAD = RCC_ClockFreq.HCLK_Frequency / 8000 * nms;    /* Time load (SysTick-> LOAD is 24bit). */
  }
  
  SysTick->VAL = 0;                                               /* Empty counter. */
  SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;                       /* Start the countdown. */
  
  while((SysTick->CTRL&(1UL<<16)) != (1UL<<16));                  /* Wait time is reached. */
  
  SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;                      /* Close counter. */
}

/**
  * @brief  Second delay.
  * @param  [in] ns: The number of second delay.
  * @return None.
  */
void Delay_s(uint64_t ns)
{
  while(ns > 0)
  {
    Delay_ms(1000);
    ns--;
  }
}

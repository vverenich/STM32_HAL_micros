/*
 * micros.h
 *
 *  Created on: Dec 9, 2024
 *      Author: Viarenich Uladzislau
 *
 *  Contact information
 *  -------------------
 *
 * GitHub   :  @vverenich
 */

/*
 * |---------------------------------------------------------------------------------
 * | Copyright (C) Viarenich Uladzislau,2024
 * |
 * | This program is free software: you can redistribute it and/or modify
 * | it under the terms of the GNU General Public License as published by
 * | the Free Software Foundation, either version 3 of the License, or
 * | any later version.
 * |
 * | This program is distributed in the hope that it will be useful,
 * | but WITHOUT ANY WARRANTY; without even the implied warranty of
 * | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * | GNU General Public License for more details.
 * |
 * | You should have received a copy of the GNU General Public License
 * | along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * |
 * | Based on the following sources:
 * | 1) https://github.com/leech001/micros
 * | 2) https://deepbluembedded.com/stm32-delay-microsecond-millisecond-utility-dwt-delay-timer-delay/
 * | 3) https://stackoverflow.com/questions/36378280/stm32-how-to-enable-dwt-cycle-counter
 * |---------------------------------------------------------------------------------
 */

#ifndef INC_MICROS_H_
#define INC_MICROS_H_

#include "main.h"

__STATIC_INLINE uint32_t DWT_Init(void)
{
	// Disable TRC 
    CoreDebug->DEMCR &= ~CoreDebug_DEMCR_TRCENA_Msk; // ~0x01000000;
    // Enable TRC 
    CoreDebug->DEMCR |=  CoreDebug_DEMCR_TRCENA_Msk; // 0x01000000;
 
    // Disable clock cycle counter 
    DWT->CTRL &= ~DWT_CTRL_CYCCNTENA_Msk; //~0x00000001;
    // Enable  clock cycle counter 
    DWT->CTRL |=  DWT_CTRL_CYCCNTENA_Msk; //0x00000001;

	// Unlock the dbg regs
	DWT->LAR = 0xC5ACCE55; 
 
    // Reset the clock cycle counter value 
    DWT->CYCCNT = 0;
 
    // 3 NO OPERATION instructions 
    __ASM volatile ("NOP");
    __ASM volatile ("NOP");
    __ASM volatile ("NOP");
 
    // Check if clock cycle counter has started 
    if(DWT->CYCCNT)
    {
       return 0; //clock cycle counter started
    }
    else
    {
      return 1; //clock cycle counter not started
    }
}

__STATIC_INLINE void delay_us(uint32_t us)
{
	uint32_t initial_ticks = DWT->CYCCNT;
	uint32_t us_count_tic =  us * (SystemCoreClock / 1000000U);
	while ((DWT->CYCCNT - initial_ticks) < us_count_tic);
}

__STATIC_INLINE uint32_t micros(void){
	return  DWT->CYCCNT / (SystemCoreClock / 1000000U);
}

#endif /* INC_MICROS_H_ */

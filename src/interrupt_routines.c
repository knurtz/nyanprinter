/*
 * interrupt_routines.c
 *
 *  Created on: May 27, 2019
 *  Author: Johannes Herold
 */

#include "stm32f10x.h"
#include "stm32f10x_conf.h"

#include "pins.h"

uint8_t foo = 0;


// TIM1 update (printer clk timer)
void TIM1_UP_IRQHandler() {
	// this triggers whenever the printer has received a full new line of image data (64 transfers)
    if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET) {
        TIM_ClearITPendingBit(TIM1, TIM_IT_Update);

        // trigger latch so printer copies data from shift registers
	    GPIO_WriteBit(PRINTER_LATCH_PORT, PRINTER_LATCH_PIN, PRINTER_LATCH_SET_STATE);
	    GPIO_WriteBit(PRINTER_LATCH_PORT, PRINTER_LATCH_PIN, PRINTER_LATCH_RESET_STATE);
    }
}


// TIM2 update (motor step timer)
void TIM2_IRQHandler() {
	// this triggers whenever the motor gets a new step impulse
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) {
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

        // every other motor pulse switch between:
        if (foo) TIM_Cmd(TIM1, ENABLE);		// transferring new image data
        else TIM_Cmd(TIM3, ENABLE);			// and firing strobe
        foo = !foo;
    }
}


// DMA1 channel2
void DMA1_Channel2_IRQHandler() {
	// this triggers whenever the image buffer has been transferred completely (all lines)
    if (DMA_GetFlagStatus(DMA1_FLAG_TC2) != RESET) {
    	DMA_ClearFlag(DMA1_FLAG_TC2);

        GPIO_WriteBit(LED1_PORT, LED1_PIN, GPIO_ReadOutputDataBit(LED1_PORT, LED1_PIN) ? Bit_RESET : Bit_SET);		// toggle led for debugging
    }
}


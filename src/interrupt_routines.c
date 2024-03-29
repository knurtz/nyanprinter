/*
 * interrupt_routines.c
 *
 *  Created on: May 27, 2019
 *  Author: Johannes Herold
 */

#include "stm32f10x.h"
#include "stm32f10x_conf.h"

#include "pins.h"
#include "image_nyan_loop.h"
#include "init.h"


volatile uint8_t cycle = 1;		// cycle = 1: send new image data, cycle = 0: activate strobe
volatile uint8_t startframe_printed = 0;


// TIM1 update (printer image data clock)
void TIM1_UP_IRQHandler() {
	// this triggers whenever the printer has received a full new line of image data (64 transfers)
	if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET) {
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);

		// trigger latch so printer copies data from shift registers
		GPIO_WriteBit(PRINTER_LATCH_PORT, PRINTER_LATCH_PIN, PRINTER_LATCH_SET_STATE);
		GPIO_WriteBit(PRINTER_LATCH_PORT, PRINTER_LATCH_PIN, PRINTER_LATCH_RESET_STATE);
		//GPIO_WriteBit(LED4_PORT, LED4_PIN, Bit_SET);
	}
}


// TIM2 update (motor steps)
void TIM2_IRQHandler() {
	// this triggers whenever the motor gets a new step impulse
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) {
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

		// every other motor pulse alternate between
		if (cycle) TIM_Cmd(TIM1, ENABLE);		// transferring new image data
		else {
			TIM_Cmd(TIM3, ENABLE);						// and firing strobe
			GPIO_WriteBit(MOTOR_DIR_PORT, MOTOR_DIR_PIN, MOTOR_DIR_FORWARD);	// reset motor direction every time when firing strobe
		}
		cycle = !cycle;
	}

	/*
	// this triggers halfway in between two motor steps
	// if we started a data transmission on the last step (cycle now = 0), it should be done by now (takes 64 us) and we can deactivate latch to safe data on printer
	if (TIM_GetITStatus(TIM2, TIM_IT_CC1) != RESET) {
		TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);


	}
	*/
}


// DMA1 channel2 (printer image data)
void DMA1_Channel2_IRQHandler() {
	// this triggers whenever the image buffer has been transferred completely (all lines)
	if (DMA_GetFlagStatus(DMA1_FLAG_TC2) != RESET) {
		DMA_ClearFlag(DMA1_FLAG_TC2);
		/*if (!startframe_printed) {
			startframe_printed = 1;
			//image_dma_init(image_nyan_loop, image_nyan_loop_length);
			DMA_Cmd(DMA1_Channel2, DISABLE);
			DMA1_Channel2->CNDTR = image_nyan_loop_length;
			DMA1_Channel2->CMAR = (uint32_t)image_nyan_loop + 18;
			DMA_Cmd(DMA1_Channel2, ENABLE);
		}
*/
	}
}


// DMA1 channel3 (SPI LED driver data)
void DMA1_Channel3_IRQHandler() {
	// this triggers whenever the led buffer has been transferred completely
	if (DMA_GetFlagStatus(DMA1_FLAG_TC3) != RESET) {
		DMA_ClearFlag(DMA1_FLAG_TC3);

		//GPIO_WriteBit(LED1_PORT, LED1_PIN, GPIO_ReadOutputDataBit(LED1_PORT, LED1_PIN) ? Bit_RESET : Bit_SET);		// toggle led for debugging
	}
}

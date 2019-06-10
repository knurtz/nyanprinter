/*
 * init.c
 *
 *  Created on: May 27, 2019
 *  Author: Johannes Herold
 */

#include "stm32f10x.h"
#include "stm32f10x_conf.h"

#include "pins.h"

void gpio_init() {

	GPIO_InitTypeDef gpio_init;
	gpio_init.GPIO_Speed = GPIO_Speed_50MHz;


	// all GPIOS of the F103 are needed in thiss project
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);


	// disable JTAG so all pins other than SWD are available
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);


	// init LED pins
	gpio_init.GPIO_Mode = GPIO_Mode_Out_PP;			// normal output

	gpio_init.GPIO_Pin = LED1_PIN;
	GPIO_Init(LED1_PORT, &gpio_init);
	GPIO_WriteBit(LED1_PORT, LED1_PIN, LED_RESET_STATE);

	gpio_init.GPIO_Pin = LED2_PIN;
	GPIO_Init(LED2_PORT, &gpio_init);
	GPIO_WriteBit(LED2_PORT, LED2_PIN, LED_RESET_STATE);

	gpio_init.GPIO_Pin = LED3_PIN;
	GPIO_Init(LED3_PORT, &gpio_init);
	GPIO_WriteBit(LED3_PORT, LED3_PIN, LED_RESET_STATE);

	gpio_init.GPIO_Pin = LED4_PIN;
	GPIO_Init(LED4_PORT, &gpio_init);
	GPIO_WriteBit(LED4_PORT, LED4_PIN, LED_RESET_STATE);


	// init motor pins
	gpio_init.GPIO_Mode = GPIO_Mode_Out_PP;			// normal output
	gpio_init.GPIO_Pin = MOTOR_EN_PIN;
	GPIO_Init(MOTOR_EN_PORT, &gpio_init);
	GPIO_WriteBit(MOTOR_EN_PORT, MOTOR_EN_PIN, MOTOR_EN_RESET_STATE);

	gpio_init.GPIO_Mode = GPIO_Mode_AF_PP;			// alternate function means TIM2 CC1 output
	gpio_init.GPIO_Pin = MOTOR_STEP_PIN;
	GPIO_Init(MOTOR_STEP_PORT, &gpio_init);


	// init printer pins
	gpio_init.GPIO_Mode = GPIO_Mode_Out_PP;			// normal outputs
	gpio_init.GPIO_Pin = PRINTER_DATA_PINS;
	GPIO_Init(PRINTER_DATA_PORT, &gpio_init);

	gpio_init.GPIO_Mode = GPIO_Mode_Out_PP;			// normal output
	gpio_init.GPIO_Pin = PRINTER_LATCH_PIN;
	GPIO_Init(PRINTER_LATCH_PORT, &gpio_init);
	GPIO_WriteBit(PRINTER_LATCH_PORT, PRINTER_LATCH_PIN, PRINTER_LATCH_RESET_STATE);

	gpio_init.GPIO_Mode = GPIO_Mode_AF_PP;			// alternate function means TIM1 CC1 output
	gpio_init.GPIO_Pin = PRINTER_CLK_PIN;
	GPIO_Init(PRINTER_CLK_PORT, &gpio_init);

	gpio_init.GPIO_Mode = GPIO_Mode_AF_PP;			// alternate function means TIM3 CC1 output
	gpio_init.GPIO_Pin = PRINTER_STROBE_PIN;
	GPIO_Init(PRINTER_STROBE_PORT, &gpio_init);

}

// TIM1 creates clock for parallel printer data (TIM1 Channel1 -> PA8)
void printer_clk_init() {

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

	TIM_TimeBaseInitTypeDef timerInitStructure;
	timerInitStructure.TIM_Prescaler = 0;								// frequency for TIM1: 72 MHz
	timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	timerInitStructure.TIM_Period = (SystemCoreClock / 1000000) - 1;	// timer update with a frequency of 1 MHz
	timerInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	timerInitStructure.TIM_RepetitionCounter = 63;						// 64 cycles for one chunk of picture data (one line needs 64 bytes)
	TIM_TimeBaseInit(TIM1, &timerInitStructure);

	TIM_OCInitTypeDef outputChannelInit;
	outputChannelInit.TIM_OCMode = TIM_OCMode_PWM1;						// set output low if counter > pulse --> pattern "high - low"
	outputChannelInit.TIM_Pulse = 35;									// 50 % doodie cycle
	outputChannelInit.TIM_OutputState = TIM_OutputState_Enable;
	outputChannelInit.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC1Init(TIM1, &outputChannelInit);
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_CtrlPWMOutputs(TIM1, ENABLE);

	NVIC_InitTypeDef nvicStructure;
	nvicStructure.NVIC_IRQChannel = TIM1_UP_IRQn;
	nvicStructure.NVIC_IRQChannelPreemptionPriority = 0;
	nvicStructure.NVIC_IRQChannelSubPriority = 1;
	nvicStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvicStructure);

	TIM_SelectOnePulseMode(TIM1, TIM_OPMode_Single);					// only fire once (each time with the amount of repetitions set earlier)
	TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);							// update interrupt is only generated after repetition counter has reached zero
	TIM_DMACmd(TIM1, TIM_DMA_CC1, ENABLE);								// hook up TIM1_CH1 to DMA1 channel2 request (image dma)

	TIM_Cmd(TIM1, DISABLE);												// will be activated in interrupt rountine of motor step timer every other step

}

// TIM2 creates motor steps (TIM2 Channel1 -> PA0)
void motor_step_init() {

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	TIM_TimeBaseInitTypeDef timerInitStructure;
	timerInitStructure.TIM_Prescaler = 71;								// frequency for TIM2: 1 MHz
	timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	timerInitStructure.TIM_Period = 2000;								// default 500 Hz, motor speed can be updated with set_motor_freq(uint16_t hz) in helper.c
	timerInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	timerInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &timerInitStructure);

	TIM_OCInitTypeDef outputChannelInit;
	outputChannelInit.TIM_OCMode = TIM_OCMode_PWM2;						// set output high if counter > pulse --> pattern "low - high"
	outputChannelInit.TIM_Pulse = 500;									// smallest clock period is 900 (for 1100 Hz) -> Pulse of 500 should always work
	outputChannelInit.TIM_OutputState = TIM_OutputState_Enable;
	outputChannelInit.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC1Init(TIM2, &outputChannelInit);
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);

	NVIC_InitTypeDef nvicStructure;
	nvicStructure.NVIC_IRQChannel = TIM2_IRQn;
	nvicStructure.NVIC_IRQChannelPreemptionPriority = 0;
	nvicStructure.NVIC_IRQChannelSubPriority = 1;
	nvicStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvicStructure);

	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);							// enable update interrupt triggering every motor step

	TIM_Cmd(TIM2, DISABLE);												// will be activated in main routine, whenever motor is supposed to move
}

// TIM3 creates strobe pulse (TIM3 Channel11 -> PA6)
void printer_strobe_init() {

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	TIM_TimeBaseInitTypeDef timerInitStructure;
	timerInitStructure.TIM_Prescaler = 71;								// frequency for TIM3: 1 MHz -> 1 us per timer tick
	timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	timerInitStructure.TIM_Period = 800;								// 750 us strobe length should suffice at 8 volts... maybe 1000 is necessary
	timerInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	timerInitStructure.TIM_RepetitionCounter = 0;						// only 1 cycle, then stop because of one pulse mode
	TIM_TimeBaseInit(TIM3, &timerInitStructure);

	TIM_OCInitTypeDef outputChannelInit;
	outputChannelInit.TIM_OCMode = TIM_OCMode_PWM2;						// set output high if counter > pulse --> pattern "low - high"
	outputChannelInit.TIM_Pulse = 2;									// match at 2, so output will be low at timer reset but goes high immediately after start
	outputChannelInit.TIM_OutputState = TIM_OutputState_Enable;
	outputChannelInit.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC1Init(TIM3, &outputChannelInit);
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);

	TIM_SelectOnePulseMode(TIM3, TIM_OPMode_Single);					// only fire once

	TIM_Cmd(TIM3, DISABLE);												// will be activated in interrupt rountine of motor step timer every other step
}

void image_dma_init(const uint8_t *image_buffer, uint16_t image_length) {

	// disable channel first in case of reconfiguring during runtime
	DMA_Cmd(DMA1_Channel2, DISABLE);

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	DMA_InitTypeDef dmaInitStructure;
	dmaInitStructure.DMA_BufferSize = image_length;						// 64 bytes per line, depends on image data
	dmaInitStructure.DMA_DIR = DMA_DIR_PeripheralDST;					// peripheral (GPIO) is destination
	dmaInitStructure.DMA_M2M = DMA_M2M_Disable;
	dmaInitStructure.DMA_MemoryBaseAddr = (uint32_t)image_buffer;		// transfer image data
	dmaInitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	dmaInitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	dmaInitStructure.DMA_Mode = DMA_Mode_Circular;
	dmaInitStructure.DMA_PeripheralBaseAddr = (uint32_t)((&PRINTER_DATA_PORT->ODR));	// output data register of GPIOB is used
	dmaInitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	dmaInitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	dmaInitStructure.DMA_Priority = DMA_Priority_High;
	DMA_Init(DMA1_Channel2, &dmaInitStructure);

	// activate transfer complete interrupt for debugging
	NVIC_InitTypeDef nvicStructure;
	nvicStructure.NVIC_IRQChannel = DMA1_Channel2_IRQn;
	nvicStructure.NVIC_IRQChannelPreemptionPriority = 0;
	nvicStructure.NVIC_IRQChannelSubPriority = 1;
	nvicStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvicStructure);
	DMA_ITConfig(DMA1_Channel2, DMA_IT_TC, ENABLE);

	DMA_Cmd(DMA1_Channel2, ENABLE);										// once dma channel has been enabled, a transfer will be engaged when request comes in (here: TIM1_CH1)

}



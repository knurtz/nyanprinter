/**
 * @main.cpp
 *
 * Boilerplate Project for STM32F103C8 as explained on
 * http://www.atakansarioglu.com/stm32-project-from-scratch-on-eclipse/
 *
 * Additional info on STM32F103
 * https://www.st.com/en/microcontrollers/stm32f103.html?querycriteria=productId=LN1565
 *
 */
#include "stm32f10x.h"
#include "stm32f10x_conf.h"

#include "config.h"
#include "delay.h"

uint8_t test_picture [8][64] = {
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
};

// TIM2 update event: switch between activating strobe (TIM3) or data transfer (TIM1)
extern "C" void TIM2_IRQHandler()
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
        GPIO_WriteBit(GPIOC, GPIO_Pin_13, GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_13) ? Bit_RESET : Bit_SET);
    }
}

/*
// TIM1 update event test function
extern "C" void TIM1_CC_IRQHandler()
{
    if (TIM_GetITStatus(TIM1, TIM_IT_CC1) != RESET)
    {
        TIM_ClearITPendingBit(TIM1, TIM_IT_CC1);
        GPIO_WriteBit(GPIOC, GPIO_Pin_13, GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_13) ? Bit_RESET : Bit_SET);
    }
}
*/

// manipulate picture data on a pixel level
// pixel to byte assignment is weird due to the way the printer gets its data (6 serial interfaces in parallel)
// only the 6 LSBs in each byte are picture data
// bit 6 is for motor direction and bit 7 is for latch. They only matter on the very last byte per line, so in picture[y][63]
void picture_set_pixel(uint8_t picture[][64], int line, int pixel, bool value) {

	if (value) picture[line][pixel%64] |= (1<<(pixel/64));
	else picture[line][pixel%64] &= ~(1<<(pixel/64));

}

void picture_set_line(uint8_t picture[][64], int line, bool value) {

	if (value) for (int i = 0; i < 64; i++) picture[line][i] = 0b11111111;
	else for (int i = 0; i < 64; i++) picture[line][i] = 0;

}


void gpio_init() {

	GPIO_InitTypeDef gpio_init;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

	// init step pin PA0 and clock pin PA8 both on alternate function
	gpio_init.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_8;
	gpio_init.GPIO_Mode = GPIO_Mode_AF_PP;			// output, but controlled by TIM1 / TIM2
	gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpio_init);


	// latch pin PA11 and motor enable pin PA4
	gpio_init.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_11;
	gpio_init.GPIO_Mode = GPIO_Mode_Out_PP;			// normal output
	gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpio_init);
    GPIO_WriteBit(GPIOA, GPIO_Pin_4, Bit_SET);		// motor enable is active low
    GPIO_WriteBit(GPIOA, GPIO_Pin_11, Bit_SET); 	// latch is active low

    // init step pin PB4
	gpio_init.GPIO_Pin = GPIO_Pin_4;
	gpio_init.GPIO_Mode = GPIO_Mode_AF_PP;			// output, but controlled by TIM3
	gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &gpio_init);

	// init parallel data pins PB8-PB15
	gpio_init.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14;
	gpio_init.GPIO_Mode = GPIO_Mode_Out_PP;			// normal output, put ODR is written by DMA1, channel 2
	gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &gpio_init);
    GPIO_WriteBit(GPIOB, GPIO_Pin_14, Bit_RESET);	// initialize motor dir pin

	// init LED pin PC13 (onboard LED)
	gpio_init.GPIO_Pin = GPIO_Pin_13;
	gpio_init.GPIO_Mode = GPIO_Mode_Out_PP;			// normal output
	gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &gpio_init);
    GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_SET);		// active low

}


// TIM1 creates clock for parallel data for printer: TIM1 Channel 1 -> PA8
void parallel_clock_timer_init() {

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

	TIM_TimeBaseInitTypeDef timerInitStructure;
	timerInitStructure.TIM_Prescaler = 0;								// target frequency for TIM1: 72 MHz
	timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	timerInitStructure.TIM_Period = 72;									// timer update with a frequency of 1 MHz
	timerInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	timerInitStructure.TIM_RepetitionCounter = 63;						// 64 cycles for one chunk of picture data
	TIM_TimeBaseInit(TIM1, &timerInitStructure);

	TIM_OCInitTypeDef outputChannelInit = {0,};
	outputChannelInit.TIM_OCMode = TIM_OCMode_PWM1;						// set active high if counter > pulse
	outputChannelInit.TIM_Pulse = 36;									// 50 % doodie cycle
	outputChannelInit.TIM_OutputState = TIM_OutputState_Enable;
	outputChannelInit.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC1Init(TIM1, &outputChannelInit);
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);

	NVIC_InitTypeDef nvicStructure;
	nvicStructure.NVIC_IRQChannel = TIM1_CC_IRQn;
	nvicStructure.NVIC_IRQChannelPreemptionPriority = 0;
	nvicStructure.NVIC_IRQChannelSubPriority = 1;
	nvicStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvicStructure);

	//TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);						// TIM1 update (high -> low) triggers printer to shift in data
	//TIM_ITConfig(TIM1, TIM_IT_CC1, ENABLE);							// TIM1 compare match at 50% (low -> high) triggers DMA to put new data to D1 - D6 pins
	TIM_DMACmd(TIM1, TIM_DMA_CC1, ENABLE);

	TIM_SelectOnePulseMode(TIM1, TIM_OPMode_Single);					// only fire once (each time with 64 repetitions)

	TIM_Cmd(TIM1, DISABLE);												// will be activated by code later
}

// TIM2 creates motor steps: TIM2 Channel 1 -> PA0 (step signal)
void motor_timer_init() {

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	TIM_TimeBaseInitTypeDef timerInitStructure;
	timerInitStructure.TIM_Prescaler = 720;								// target frequency for TIM2: 100 kHz
	timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	timerInitStructure.TIM_Period = 10000;								// for 10 Hz step frequency. gets adjusted to match current pitch
	timerInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	timerInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &timerInitStructure);

	TIM_OCInitTypeDef outputChannelInit = {0,};
	outputChannelInit.TIM_OCMode = TIM_OCMode_PWM2;						// set active low if counter > pulse
	outputChannelInit.TIM_Pulse = 5000;									// 50 % doodie cycle - also adjusted according to pitch
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

	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);							// TIM2 update ISR triggers data transfer (TIM1) or strobe (TIM3)

	TIM_Cmd(TIM2, DISABLE);												// will be activated by code later
}

// TIM3 creates strobe pulse: TIM3 Channel 1 -> PB4
void strobe_timer_init() {

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	TIM_TimeBaseInitTypeDef timerInitStructure;
	timerInitStructure.TIM_Prescaler = (SystemCoreClock / 1000000) - 1;		// target frequency for TIM3: 1 MHz -> 1 us per tick
	timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	timerInitStructure.TIM_Period = 750;								// 750 us strobe length should suffice at 8 volts... maybe 1000 is necessary
	timerInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	timerInitStructure.TIM_RepetitionCounter = 0;						// 1 cycle, then stop because in oe pulse mode
	TIM_TimeBaseInit(TIM3, &timerInitStructure);

	TIM_OCInitTypeDef outputChannelInit = {0,};
	outputChannelInit.TIM_OCMode = TIM_OCMode_PWM1;						// set active high if counter > pulse
	outputChannelInit.TIM_Pulse = 1;									// match at 1, so output will be low at timer reset but goes high immediately after start
	outputChannelInit.TIM_OutputState = TIM_OutputState_Enable;
	outputChannelInit.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC1Init(TIM3, &outputChannelInit);
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);

	TIM_SelectOnePulseMode(TIM3, TIM_OPMode_Single);					// only fire once

	TIM_Cmd(TIM3, DISABLE);												// will be activated by code later
}

void picture_dma_init() {

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	DMA_InitTypeDef dmaInitStructure;
	dmaInitStructure.DMA_BufferSize = 512;								// 8 * 64 byte for text_picture
	dmaInitStructure.DMA_DIR = DMA_DIR_PeripheralDST;					// peripheral (GPIO) is destination
	dmaInitStructure.DMA_M2M = DMA_M2M_Disable;
	dmaInitStructure.DMA_MemoryBaseAddr = (uint32_t)&test_picture;		// transfer image data
	dmaInitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	dmaInitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	dmaInitStructure.DMA_Mode = DMA_Mode_Circular;
	dmaInitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&GPIOB->ODR);	// output data register of GPIOB is used
	dmaInitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	dmaInitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	dmaInitStructure.DMA_Priority = DMA_Priority_High;
	DMA_Init(DMA1_Channel2, &dmaInitStructure);
}



int main(void) {

	SystemInit();

	// initialize some stuff
	delay_init(DELAY_RESOLUTION_10_US);				// init delay with granularity of 10 us
	gpio_init();
	motor_timer_init();
	parallel_clock_timer_init();
	picture_dma_init();
	//strobe_timer_init();



	while(1) {
		// trigger TIM1 once, it should do 64 repetitions, triggering DMA1 each time -> one line of picture data should get transferred to printer
		TIM_Cmd(TIM1, ENABLE);

		// manually trigger latch for a few us -> data gets transferred from shift registers to strobes
	    GPIO_WriteBit(GPIOB, GPIO_Pin_14, Bit_RESET);
	    delay_usec(100);
	    GPIO_WriteBit(GPIOB, GPIO_Pin_14, Bit_SET);

	    // manually trigger strobe for 1000 us
	    GPIO_WriteBit(GPIOB, GPIO_Pin_14, Bit_SET);
		delay_usec(1000);
		GPIO_WriteBit(GPIOB, GPIO_Pin_14, Bit_RESET);

		// forward motor a bit
		GPIO_WriteBit(GPIOA, GPIO_Pin_4, Bit_RESET);	// enable motor driver
		TIM_Cmd(TIM2, ENABLE);
		delay_msec(1000);
		TIM_Cmd(TIM2, DISABLE);
	    GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_SET);		// disable onboard LED
	    GPIO_WriteBit(GPIOA, GPIO_Pin_4, Bit_SET);		// disable motor driver

		delay_sec(5);

	}

	return 0;
}

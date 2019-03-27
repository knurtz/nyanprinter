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


extern "C" void TIM2_IRQHandler()
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
        GPIO_WriteBit(GPIOC, GPIO_Pin_13, GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_13) ? Bit_RESET : Bit_SET);
    }
}


void gpio_init() {

	GPIO_InitTypeDef gpio_init;

	// init LED pin PC13 (onboard LED)
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	gpio_init.GPIO_Pin = GPIO_Pin_13;
	gpio_init.GPIO_Mode = GPIO_Mode_Out_PP;			// normal output
	gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &gpio_init);
    GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_RESET);

    // init step pin PA0
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	gpio_init.GPIO_Pin = GPIO_Pin_0;
	gpio_init.GPIO_Mode = GPIO_Mode_AF_PP;			// output, but controlled by TIM2
	gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpio_init);

}

// TIM2 creates motor steps: TIM2 Channel 1 -> PA0 (step signal)
void motor_timer_init() {

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	TIM_TimeBaseInitTypeDef timerInitStructure;
	timerInitStructure.TIM_Prescaler = SystemCoreClock / 100000;		// target frequency for TIM2: 100 kHz
	timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	timerInitStructure.TIM_Period = 20000;								// for 10 Hz step frequency. gets adjusted to match current music note
	timerInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	timerInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &timerInitStructure);

	TIM_OCInitTypeDef outputChannelInit = {0,};
	outputChannelInit.TIM_OCMode = TIM_OCMode_PWM2;
	outputChannelInit.TIM_Pulse = 10000;								// 50 % doodie cycle for period of 1000000. needs to get adjusted as well.
	outputChannelInit.TIM_OutputState = TIM_OutputState_Enable;
	outputChannelInit.TIM_OCPolarity = TIM_OCPolarity_High;

	TIM_OC1Init(TIM2, &outputChannelInit);
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);				// trigger TIM1 (data transfer) or TIM3 (strobe) in TIM2 update ISR (switch between TIM1 and TIM3 every other cycle)

	NVIC_InitTypeDef nvicStructure;
	nvicStructure.NVIC_IRQChannel = TIM2_IRQn;
	nvicStructure.NVIC_IRQChannelPreemptionPriority = 0;
	nvicStructure.NVIC_IRQChannelSubPriority = 1;
	nvicStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvicStructure);

	TIM_Cmd(TIM2, ENABLE);

}



int main(void) {

	// initialize some stuff
	delay_init(DELAY_RESOLUTION_10_US);				// init delay with granularity of 10 us
	gpio_init();
	motor_timer_init();

	while(1) {
		// Main Loop Delay
		delay_msec(100);

		// blink LED
		//

	}

	return 0;
}

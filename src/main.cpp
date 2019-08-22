/**
 * @main.cpp
 *
 * NyanPrinter main source code
 *
 * Johannes Herold, 2019
 *
 * Additional info on STM32F103
 * https://www.st.com/en/microcontrollers/stm32f103.html?querycriteria=productId=LN1565
 *
 */

#include "stm32f10x.h"
#include "stm32f10x_conf.h"

#include "delay.h"
#include "init.h"
#include "helper.h"
#include "pins.h"
#include "notes.h"

#include "image_test.h"

#define LED_BUFFER_LENGTH

uint8_t led_buffer[20] =
	{
		0, 0xff,
		1, 0xff,
		2, 0xff,
		3, 0xff,
		4, 0xff,
		5, 0xff,
		6, 0xff,
		7, 0xff,
		8, 0xff,
		9, 0xff
	};



int main(void) {

	SystemInit();


	// initialize some stuff
	delay_init(DELAY_RESOLUTION_1_MS);				// configures systick and systick handler, restarts uptime counter
	gpio_init();
	printer_clk_init();
	motor_step_init();
	printer_strobe_init();
	image_dma_init(image_test, image_test_length);


	// enable motor
	GPIO_WriteBit(MOTOR_EN_PORT, MOTOR_EN_PIN, MOTOR_EN_SET_STATE);

	while(1) {

		// check if another interval of delay resolution has passed
		if (systick_interrupt) {
			systick_interrupt = 0;

		}

		set_motor_freq(NOTE_A);
		TIM_Cmd(TIM2, ENABLE);

		led_half_blink();

	}

	return 0;
}

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

//#include "image_nyan_loop.h"
#include "image_test.h"

#define LED_FRAMEPERIOD		40
#define LED_FRAMERATE		25
#define BREATHING_PERIOD	4

uint8_t current_frame = 0;
int frametime_counter = 0;

uint8_t breathing_table[] = {
	3, 3, 3, 3, 4, 4, 5, 6, 8, 9, 11, 13, 15, 17, 20,
	23, 26, 29, 33, 37, 42, 46, 52, 57, 64, 70, 77, 84,
	92, 100, 109, 118, 127, 136, 146, 155, 165, 175, 185,
	194, 203, 212, 220, 227, 234, 240, 245, 248, 251, 253,
	254, 253, 251, 248, 245, 240, 234, 227, 220, 212, 203,
	194, 185, 175, 165, 155, 146, 136, 127, 118, 109, 100,
	92, 84, 77, 70, 64, 57, 52, 46, 42, 37, 33, 29, 26, 23,
	20, 17, 15, 13, 11, 9, 8, 6, 5, 4, 4, 3, 3, 3
};

/*
uint8_t led_buffer[20] = {
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
*/

int main(void) {

	SystemInit();


	// initialize some stuff
	delay_init(DELAY_RESOLUTION_1_MS);				// configures systick and systick handler, restarts uptime counter
	gpio_init();
	printer_clk_init();
	motor_step_init();
	printer_strobe_init();
	image_dma_init(image_test, image_test_length);
	led_spi_init();
	//image_dma_init(image_nyan_loop, image_nyan_loop_length);


	// enable motor
	GPIO_WriteBit(MOTOR_EN_PORT, MOTOR_EN_PIN, MOTOR_EN_SET_STATE);
	set_motor_freq(NOTE_D);
	TIM_Cmd(TIM2, ENABLE);

	while(1) {

		// wait until another interval of delay resolution has passed
		while(!systick_int);
		systick_int = 0;

		// if uptime_counter > next_note_time --> set next note / pause


		if (frametime_counter >= LED_FRAMEPERIOD) {
			frametime_counter = 0;
			spi_led_send(4, breathing_table[current_frame++]);			// update debug led attached to P4
			if (current_frame >= (LED_FRAMERATE * BREATHING_PERIOD)) {		// check for end of animation loop
				led_half_blink();
				current_frame = 0;
			}
		}

		frametime_counter++;			// systick configured to 1000 Hz -> counts milliseconds
	}

	return 0;
}

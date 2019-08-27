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

#include "image_nyan_loop.h"

#define LED_FRAMEPERIOD		40
#define LED_FRAMERATE		25
#define BREATHING_PERIOD	5

uint8_t breathing_table[] = {
		3, 3, 3, 3, 3, 4, 4, 5, 6, 7, 8, 9, 10, 11, 13, 15, 17,
		19, 21, 23, 26, 28, 31, 35, 38, 42, 45, 50, 54, 59, 64,
		69, 74, 80, 86, 92, 98, 105, 112, 119, 127, 134, 142, 150,
		157, 165, 173, 181, 188, 196, 203, 210, 217, 223, 229, 234,
		239, 243, 246, 249, 251, 253, 253, 253, 253, 251, 249, 246,
		243, 239, 234, 229, 223, 217, 210, 203, 196, 188, 181, 173,
		165, 157, 150, 142, 134, 127, 119, 112, 105, 98, 92, 86, 80,
		74, 69, 64, 59, 54, 50, 45, 42, 38, 35, 31, 28, 26, 23, 21,
		19, 17, 15, 13, 11, 10, 9, 8, 7, 6, 5, 4, 4, 3, 3, 3, 3
};

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


int main(void) {

	SystemInit();


	// initialize some stuff
	delay_init(DELAY_RESOLUTION_1_MS);				// configures systick and systick handler, restarts uptime counter
	gpio_init();
	printer_clk_init();
	motor_step_init();
	printer_strobe_init();
	image_dma_init(image_nyan_loop, image_nyan_loop_length);


	// enable motor
	GPIO_WriteBit(MOTOR_EN_PORT, MOTOR_EN_PIN, MOTOR_EN_SET_STATE);
	set_motor_freq(NOTE_A);
	TIM_Cmd(TIM2, ENABLE);

	while(1) {

		// wait until another interval of delay resolution has passed
		while(!systick_int);
		systick_int = 0;

		// if uptime_counter > next_note_time --> set next note / pause

		// if uptime counter > next led frame --> calculate next animation frame and update spi leds


		led_half_blink();

	}

	return 0;
}

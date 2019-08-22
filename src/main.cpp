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

#define LED_FRAMEPERIOD		40
#define LED_FRAMERATE		25
#define BREATHING_PERIOD	5

uint8_t breathing_table[] = {
	0, 0, 0, 0, 0, 1, 1, 2, 3, 4, 5, 6, 7, 9, 10, 12, 14, 16, 18, 20, 23, 26,
	29, 32, 36, 39, 43, 47, 52, 57, 61, 67, 72, 78, 84, 90, 97, 104, 111, 118,
	126, 133, 141, 149, 157, 165, 173, 181, 188, 196, 203, 210, 217, 223, 229,
	235, 239, 244, 247, 250, 252, 254, 254, 254, 254, 252, 250, 247, 244, 239,
	235, 229, 223, 217, 210, 203, 196, 188, 181, 173, 165, 157, 149, 141, 133,
	126, 118, 111, 104, 97, 90, 84, 78, 72, 67, 61, 57, 52, 47, 43, 39, 36, 32,
	29, 26, 23, 20, 18, 16, 14, 12, 10, 9, 7, 6, 5, 4, 3, 2, 1, 1, 0, 0, 0, 0
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

uint8_t


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
	set_motor_freq(NOTE_A);
	TIM_Cmd(TIM2, ENABLE);

	while(1) {

		// wait until another interval of delay resolution has passed
		while(!systick_interrupt);
		systick_interrupt = 0;

		// if uptime_counter > next_note_time --> set next note / pause

		// if uptime counter > next led frame --> calculate next animation frame and update spi leds


		led_half_blink();

	}

	return 0;
}

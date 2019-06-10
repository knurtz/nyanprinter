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


int main(void) {

	SystemInit();

	// initialize some stuff
	delay_init(DELAY_RESOLUTION_10_US);
	gpio_init();
	printer_clk_init();
	motor_step_init();
	printer_strobe_init();
	image_dma_init(image_test, image_test_length);

	// enable motor
	GPIO_WriteBit(MOTOR_EN_PORT, MOTOR_EN_PIN, MOTOR_EN_SET_STATE);

	while(1) {

		delay_msec(1000);

		set_motor_freq(NOTE_A << 1);			// A
		TIM_Cmd(TIM2, ENABLE);


		delay_msec(300);
		set_motor_freq(NOTE_GIS << 1);			// G#
		delay_msec(300);
		set_motor_freq(NOTE_FIS << 1);			// F#
		delay_msec(300);
		set_motor_freq(NOTE_E << 1);			// E
		delay_msec(300);
		set_motor_freq(NOTE_D << 1);			// D
		delay_msec(300);
		set_motor_freq(NOTE_CIS << 1);			// C#
		delay_msec(300);
		set_motor_freq(NOTE_H);			// B
		delay_msec(300);
		set_motor_freq(NOTE_A);			// A
		delay_msec(300);
		set_motor_freq(NOTE_GIS);			// G#
		delay_msec(300);
		set_motor_freq(NOTE_FIS);			// F#
		delay_msec(300);
		set_motor_freq(NOTE_E);			// E
		delay_msec(300);
		set_motor_freq(NOTE_D);			// D
		delay_msec(300);
		set_motor_freq(NOTE_CIS);			// C#
		delay_msec(300);
		set_motor_freq(NOTE_H >> 1);			// B
		delay_msec(300);
		set_motor_freq(NOTE_A >> 1);			// A
		delay_msec(300);

		TIM_Cmd(TIM2, DISABLE);


		led_half_blink();

	}

	return 0;
}

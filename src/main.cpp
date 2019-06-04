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

	    TIM_Cmd(TIM2, ENABLE);

	    set_motor_freq(220);			// A
		delay_msec(300);
		set_motor_freq(247);			// B
		delay_msec(300);
		set_motor_freq(277);			// C#
	    delay_msec(300);
		set_motor_freq(294);			// D
		delay_msec(300);
		set_motor_freq(330);			// E
	    delay_msec(300);
	    set_motor_freq(370);			// F#
	    delay_msec(300);
	    set_motor_freq(415);			// G#
	    delay_msec(300);
	    set_motor_freq(440);			// A
	    delay_msec(300);
	    set_motor_freq(494);			// B
	    delay_msec(300);
	    set_motor_freq(554);			// C#
	    delay_msec(300);
	    set_motor_freq(587);			// D
		delay_msec(300);
		set_motor_freq(659);			// E
		delay_msec(300);
		set_motor_freq(740);			// F#
		delay_msec(300);
		set_motor_freq(830);			// G#
		delay_msec(300);
		set_motor_freq(880);			// A

		led_half_blink();

	}

	return 0;
}

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
    TIM_Cmd(TIM2, ENABLE);

	while(1) {

		led_half_blink();
		delay_msec(500);

	}

	return 0;
}

/*
 * helper.c
 *
 *  Created on: May 27, 2019
 *  Author: Johannes Herold
 */

#include "stm32f10x.h"
#include "stm32f10x_conf.h"

#include "pins.h"
#include "delay.h"

void led_blink() {

	GPIO_WriteBit(LED1_PORT, LED1_PIN, Bit_SET);
	delay_msec(100);
	GPIO_WriteBit(LED1_PORT, LED1_PIN, Bit_RESET);
	GPIO_WriteBit(LED2_PORT, LED2_PIN, Bit_SET);
	delay_msec(100);
	GPIO_WriteBit(LED2_PORT, LED2_PIN, Bit_RESET);
	GPIO_WriteBit(LED3_PORT, LED3_PIN, Bit_SET);
	delay_msec(100);
	GPIO_WriteBit(LED3_PORT, LED3_PIN, Bit_RESET);
	GPIO_WriteBit(LED4_PORT, LED4_PIN, Bit_SET);
	delay_msec(100);
	GPIO_WriteBit(LED4_PORT, LED4_PIN, Bit_RESET);

}


void led_half_blink() {

	GPIO_WriteBit(LED3_PORT, LED3_PIN, Bit_SET);
	delay_msec(100);
	GPIO_WriteBit(LED3_PORT, LED3_PIN, Bit_RESET);
	GPIO_WriteBit(LED4_PORT, LED4_PIN, Bit_SET);
	delay_msec(100);
	GPIO_WriteBit(LED4_PORT, LED4_PIN, Bit_RESET);

}


void set_motor_freq(uint16_t hz) {

	if (hz < 50 || hz > 1100) return;

	//TIM2->CCR1 = 500000 / hz;		// new oc value is half of the new period
	TIM2->ARR = 1000000 / hz;		// new timer period, timer runs at 1 MHz

}

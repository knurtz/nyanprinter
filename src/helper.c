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


void spi_led_send(uint8_t addr, uint8_t data) {

	GPIO_WriteBit(LED_CS_PORT, LED_CS_PIN, LED_CS_SET_STATE);

	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(SPI1, (addr << 8) | data);
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	GPIO_WriteBit(LED_CS_PORT, LED_CS_PIN, LED_CS_RESET_STATE);

}


void spi_led_clear() {

	spi_led_send(0x0a, 0xff);		// special command to set all outputs simultaneously

}

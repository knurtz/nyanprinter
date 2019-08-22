/*
 * helper.h
 *
 *  Created on: May 27, 2019
 *  Author: Johannes Herold
 */

#ifndef HELPER_H_
#define HELPER_H_

#ifdef __cplusplus
extern "C" {
#endif

void led_blink();
void led_half_blink();

void set_motor_freq(uint16_t hz);

void spi_led_send(uint8_t addr, uint8_t data);
void spi_led_clear();

#ifdef __cplusplus
}
#endif

#endif /* HELPER_H_ */

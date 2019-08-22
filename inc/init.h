/*
 * init.h
 *
 *  Created on: May 27, 2019
 *  Author: Johannes Herold
 */

#ifndef INIT_H_
#define INIT_H_

#include "stm32f10x.h"
#include "stm32f10x_conf.h"

#ifdef __cplusplus
extern "C" {
#endif

void gpio_init();
void printer_clk_init();
void motor_step_init();
void printer_strobe_init();
void image_dma_init(const uint8_t *image_buffer, uint16_t image_length);
void led_spi_init(uint32_t led_buffer);

#ifdef __cplusplus
}
#endif

#endif /* INIT_H_ */

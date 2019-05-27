/*
 * image_functions.h
 *
 *  Created on: May 27, 2019
 *  Author: Johannes Herold
 */

#ifndef IMAGE_FUNCTIONS_H_
#define IMAGE_FUNCTIONS_H_

#include "stm32f10x.h"
#include "stm32f10x_conf.h"

void image_set_pixel(uint8_t image[][64], int line, int pixel, bool value);
void image_set_line(uint8_t image[][64], int line, bool value);

#endif /* IMAGE_FUNCTIONS_H_ */

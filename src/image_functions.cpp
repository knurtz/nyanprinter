/*
 * image_functions.c
 *
 *  Created on: May 27, 2019
 *  Author: Johannes Herold
 */

#include "stm32f10x.h"
#include "stm32f10x_conf.h"

// manipulate image data on a pixel level
// pixel to byte assignment is weird due to the way the printer gets its data (6 serial interfaces in parallel)
// only the 6 LSBs in each byte are picture data
// bit 6 is for motor direction and bit 7 is for latch. They only matter on the very last byte per line, so in picture[y][63]
void image_set_pixel(uint8_t image[][64], int line, int pixel, bool value) {

	if (value) image[line][pixel%64] |= (1<<(pixel/64));
	else image[line][pixel%64] &= ~(1<<(pixel/64));

}

void image_set_line(uint8_t image[][64], int line, bool value) {

	if (value) for (int i = 0; i < 64; i++) image[line][i] = 0b11111111;
	else for (int i = 0; i < 64; i++) image[line][i] = 0;

}


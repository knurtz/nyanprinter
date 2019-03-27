/*-------------------------------------------------------------------------------------------------------------------------------------------
 * delay.h - declaration of delay functions
 *
 * Copyright (c) 2014-2018 Frank Meyer - frank(at)fli4l.de
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
#ifndef DELAY_H
#define DELAY_H

#include "stm32f10x.h"
#include "stm32f10x_rcc.h"


// resolution of delay functions
#define DELAY_RESOLUTION_1_US             1
#define DELAY_RESOLUTION_5_US             5
#define DELAY_RESOLUTION_10_US           10
#define DELAY_RESOLUTION_100_US         100

#define DELAY_DEFAULT_RESOLUTION        DELAY_RESOLUTION_100_US

extern volatile uint32_t                delay_counter;              // counts down in units of resolution


#ifdef __cplusplus
 extern "C" {
#endif


void delay_usec (uint32_t);                                  // delay of n usec, only reasonable if resolution is 1us or 5us
void delay_msec (uint32_t);                                  // delay of n msec
void delay_sec  (uint32_t);                                  // delay of n sec
void delay_init (uint_fast8_t);                              // init delay functions


#ifdef __cplusplus
}
#endif


#endif

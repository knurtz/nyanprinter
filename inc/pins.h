/*
 * pins.h
 *
 *  Created on: May 27, 2019
 *  Author: Johannes Herold
 */

#ifndef PINS_H_
#define PINS_H_


// ******** Onboard LED pins ***********
#define LED1_PORT                   GPIOA
#define LED1_PIN                    GPIO_Pin_15

#define LED2_PORT                   GPIOA
#define LED2_PIN                    GPIO_Pin_7

#define LED3_PORT                   GPIOA
#define LED3_PIN                    GPIO_Pin_5

#define LED4_PORT                   GPIOC
#define LED4_PIN                    GPIO_Pin_13

#define LED_SET_STATE				Bit_SET
#define LED_RESET_STATE				Bit_RESET


// ******** SPI LED driver pins ***********
#define LED_CS_PORT	 				GPIOA
#define LED_CS_PIN					GPIO_Pin_2
#define LED_CS_SET_STATE			Bit_RESET
#define LED_CS_RESET_STATE			Bit_SET

#define LED_CLOCK_PORT				GPIOB
#define LED_CLOCK_PIN				GPIO_Pin_3

#define LED_MISO_PORT				GPIOB
#define LED_MISO_PIN				GPIO_Pin_4

#define LED_MOSI_PORT				GPIOB
#define LED_MOSI_PIN				GPIO_Pin_5


// ******** MOTOR pins ***********
#define MOTOR_STEP_PORT             GPIOA
#define MOTOR_STEP_PIN              GPIO_Pin_0

#define MOTOR_EN_PORT               GPIOA
#define MOTOR_EN_PIN                GPIO_Pin_4
#define MOTOR_EN_SET_STATE			Bit_RESET
#define MOTOR_EN_RESET_STATE		Bit_SET

#define MOTOR_DIR_PORT				GPIOB
#define MOTOR_DIR_PIN				GPIO_Pin_14
#define MOTOR_DIR_FORWARD			Bit_SET
#define	MOTOR_DIR_BACKWARD			Bit_RESET


// ******** PRINTER pins ***********
#define PRINTER_STROBE_PORT         GPIOA
#define PRINTER_STROBE_PIN          GPIO_Pin_6

#define PRINTER_CLK_PORT            GPIOA
#define PRINTER_CLK_PIN             GPIO_Pin_8

#define PRINTER_LATCH_PORT          GPIOA
#define PRINTER_LATCH_PIN           GPIO_Pin_11
#define PRINTER_LATCH_SET_STATE		Bit_RESET
#define PRINTER_LATCH_RESET_STATE	Bit_SET

#define PRINTER_DATA_PORT           GPIOB
#define PRINTER_DATA_PINS           GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14


// ******** BUTTON pin ***********
#define BUTTON_PORT                 GPIOA
#define BUTTON_PIN                  GPIO_Pin_1


#endif /* PINS_H_ */

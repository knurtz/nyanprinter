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
#include "notes.h"

#include "image_nyan_start.h"
//#include "image_test.h"

#define FRAMERATE				25
#define FRAME_PERIOD			40
#define BREATHING_PERIOD		4

#define MUSIC_BPM				120
const int thirtysecond = (60000 / MUSIC_BPM) >> 5;		// amount of milliseconds for one thirtysecond note

int frametimer = 0;				// stores duration since last frame update, counts up
int musictimer = 0;				// stores the duration until the next note, counts down

uint8_t current_frame = 0;

uint8_t breathing_table[] = {
	3, 3, 3, 3, 4, 4, 5, 6, 8, 9, 11, 13, 15, 17, 20,
	23, 26, 29, 33, 37, 42, 46, 52, 57, 64, 70, 77, 84,
	92, 100, 109, 118, 127, 136, 146, 155, 165, 175, 185,
	194, 203, 212, 220, 227, 234, 240, 245, 248, 251, 253,
	254, 253, 251, 248, 245, 240, 234, 227, 220, 212, 203,
	194, 185, 175, 165, 155, 146, 136, 127, 118, 109, 100,
	92, 84, 77, 70, 64, 57, 52, 46, 42, 37, 33, 29, 26, 23,
	20, 17, 15, 13, 11, 9, 8, 6, 5, 4, 4, 3, 3, 3
};


int main(void) {

	SystemInit();


	// initialize some stuff
	delay_init(DELAY_RESOLUTION_1_MS);				// configures systick and systick handler, restarts uptime counter
	gpio_init();
	printer_clk_init();
	motor_step_init();
	printer_strobe_init();
	//image_dma_init(image_test, image_test_length);
	image_dma_init(image_nyan_start, image_nyan_start_length);
	led_spi_init();

	// enable motor
	GPIO_WriteBit(MOTOR_EN_PORT, MOTOR_EN_PIN, MOTOR_EN_SET_STATE);
	set_motor_freq(NOTE_D);
	TIM_Cmd(TIM2, ENABLE);

	while(1) {

		// wait until another interval of delay resolution has passed
		while(!systick_int);
		systick_int = 0;

		// if (musictimer >= music_interval)
		// get next note / pause from song array
		// set pitch or disable motor for a pause
		// calculate new value for musictimer
		// musictimer = thirtysecond << 2		// example for one sixteenth length

		if (frametimer >= FRAME_PERIOD) {
			frametimer = 0;
			spi_led_send(4, breathing_table[current_frame++]);			// update debug led attached to P4
			if (current_frame >= (FRAMERATE * BREATHING_PERIOD)) current_frame = 0;
		}

		musictimer--;
		frametimer++;			// systick configured to 1000 Hz -> counts milliseconds
	}

	return 0;
}

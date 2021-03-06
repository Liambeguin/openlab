/*
 * This file is part of DronolabSTM32-embedded
 *
 * DronolabSTM32-embedded is a student project designed
 * for flying drones.
 *
 * Go to < dronolab.etsmtl.ca > to find out more about
 * the project
 *
 *
 */

/*
 * tlc59116.c
 *
 *  Created on: Jan 21, 2014
 *      Author: liam <liambeguin.at.gmail.com>
 */


#include <stdint.h>
#include "printf.h"
#define NO_DEBUG_HEADER
#include "debug.h"

#include "platform.h"
#include "tlc59116.h"
#define PAUSE 10
#define MAX_COLOR 0xFF


void set_all_led(uint8_t r, uint8_t g, uint8_t b);

int main(void){

	signed portBASE_TYPE ret;

	//init platform
	platform_init();

    // Set initial values
    leds_off(0xff);

	printf("\n\n TLC59116 task test"
			"\n -------------------\n");

	uint8_t x;

	while (1)
	{
		// Keep red and add green
		for (x = 0; x < MAX_COLOR; x ++){
			set_all_led(MAX_COLOR, x, 0);
			soft_timer_delay_ms(PAUSE);
		}

		// Keep green and remove red
		for (x = 0; x < MAX_COLOR; x ++)
		{
			set_all_led(MAX_COLOR-x, MAX_COLOR, 0);
			soft_timer_delay_ms(PAUSE);
		}

		// Keep green and add blue
		for (x = 0; x < MAX_COLOR; x ++)
		{
			set_all_led(0, MAX_COLOR, x);
			soft_timer_delay_ms(PAUSE);
		}

		// Keep blue and remove green
		for (x = 0; x < MAX_COLOR; x ++)
		{
			set_all_led(0, MAX_COLOR-x, MAX_COLOR);
			soft_timer_delay_ms(PAUSE);
		}

		// Keep blue and add red
		for (x = 0; x < MAX_COLOR; x ++)
		{
			set_all_led(x, 0, MAX_COLOR);
			soft_timer_delay_ms(PAUSE);
		}

		// Bonus: add green
		for (x = 0; x < MAX_COLOR; x ++)
		{
			set_all_led(MAX_COLOR, x, MAX_COLOR);
			soft_timer_delay_ms(PAUSE);
		}

		// Bonus: remove green
		for (x = 0; x < MAX_COLOR; x ++)
		{
			set_all_led(MAX_COLOR, MAX_COLOR-x, MAX_COLOR);
			soft_timer_delay_ms(PAUSE);
		}

		// Keep red and remove blue
		for (x = 0; x < MAX_COLOR; x ++)
		{
			set_all_led(MAX_COLOR, 0, MAX_COLOR-x);
			soft_timer_delay_ms(PAUSE);
		}
	}

}

void set_all_led(uint8_t r, uint8_t g, uint8_t b){

	tlc59116_color_t color = {r, g, b};

	tlc59116_set_led_color(TLC59116_LED0, color);
	tlc59116_set_led_color(TLC59116_LED1, color);
	tlc59116_set_led_color(TLC59116_LED2, color);
	tlc59116_set_led_color(TLC59116_LED3, color);
	tlc59116_set_led_color(TLC59116_LED4, color);
}








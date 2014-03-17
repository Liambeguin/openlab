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
 * tlc59116.h
 *
 *  Created on: Jan 21, 2014
 *      Author: liam <liambeguin.at.gmail.com>
 */

#ifndef TLC59116_H_
#define TLC59116_H_

#include <stdint.h>

#include "i2c.h"

typedef struct{
	i2c_t i2c;
	uint8_t addr;
}tlc59116_t;


typedef enum{
	TLC59116_LED0,
	TLC59116_LED1,
	TLC59116_LED2,
	TLC59116_LED3,
	TLC59116_LED4,
}tlc59116_led_t;

typedef struct{
	uint8_t red;
	uint8_t green;
	uint8_t blue;

}tlc59116_color_t;


// to get the colors, refer to : 	http://web.njit.edu/~kevin/rgb.txt.html


#define		RGB_OFF		(tlc59116_color_t){.red = 0, .green = 0, .blue = 0}
#define		RGB_WHITE	(tlc59116_color_t){.red = 255, .green = 255, .blue = 255}

#define		RGB_RED		(tlc59116_color_t){.red = 255, .green = 0, .blue = 0}
#define		RGB_GREEN	(tlc59116_color_t){.red = 0, .green = 255, .blue = 0}
#define		RGB_BLUE	(tlc59116_color_t){.red = 0, .green = 0, .blue = 255}

#define		RGB_PURPLE	(tlc59116_color_t){.red = 148, .green = 0, .blue = 211}
#define		RGB_ORANGE	(tlc59116_color_t){.red = 255, .green = 69, .blue = 0}		//255;127;36 to try
#define		RGB_YELLOW	(tlc59116_color_t){.red = 255, .green = 255, .blue = 0}

void tlc59116_config(i2c_t i2c, uint8_t addr);
void tlc59116_init();

void tlc59116_get_led_color(tlc59116_led_t led);
//void tlc59116_get_led_color(tlc59116_led_t led, tlc59116_color_t *color)
void tlc59116_set_led_color(tlc59116_led_t led, tlc59116_color_t color);

#endif /* TLC59116_H_ */

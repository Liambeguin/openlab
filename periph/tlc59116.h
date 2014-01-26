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

void tlc59116_config(i2c_t i2c, uint8_t addr);
void tlc59116_init();

void tlc59116_get_led_color(tlc59116_led_t led);
void tlc59116_set_led_color(tlc59116_led_t led, uint8_t red, uint8_t green, uint8_t blue);

#endif /* TLC59116_H_ */

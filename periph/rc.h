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
 * rc.h
 *
 *  Created on: Nov 15, 2013
 *      Author: liam <liambeguin.at.gmail.com>
 */

#ifndef RC_H_
#define RC_H_

//external interrupt
#include "exti.h"

typedef struct{
	gpio_t port;
	gpio_pin_t pin;

}channel_t;

//fonction config qui prend un tableau de channels
void rc_config(channel_t trigger);
void rc_trigger();
void rc_clear();

#endif /* RC_H_ */

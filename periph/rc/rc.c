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
 * rc.c
 *
 *  Created on: Nov 15, 2013
 *      Author: liam <liambeguin.at.gmail.com>
 *
 *
 *      to use exti you have to add isr in drivers
 */

#include "platform.h"
#include "debug.h"

#include "rc.h"
#include "gpio.h"

channel_t _trigger;
void rc_config(channel_t trigger){
	_trigger = trigger;
}

void rc_trigger(){
	gpio_pin_set(_trigger.port, _trigger.pin);
}

void rc_clear(){
	gpio_pin_clear(_trigger.port, _trigger.pin);
}

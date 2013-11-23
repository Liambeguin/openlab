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
#include "stm32f4xx/syscfg.h"

typedef struct {
	gpio_t port;
	gpio_pin_t pin;
	syscfg_port_t syscfg_port;

	uint32_t last_time;
	uint32_t value;
}channel_t;

void rc_config_channel(channel_t *channels);
void rc_print_channel_values();

#endif /* RC_H_ */

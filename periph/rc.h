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

#include <stdbool.h>

//external interrupt
#include "exti.h"
#include "stm32f4xx/syscfg.h"


#define RC_FACTOR_THRUST	1.25f //
#define RC_FACTOR_ROLLPITCH	0.52f // 30 degrés
#define RC_FACTOR_YAW		3     //


typedef struct {
	gpio_t 			port;
	gpio_pin_t 		pin;
	syscfg_port_t 	syscfg_port;

	uint32_t 		last_time;
	uint32_t 		value;
}channel_t;


typedef struct {

	uint32_t throttle;

	uint32_t roll;
	uint32_t pitch;
	uint32_t yaw;

	bool kill_switch;
	bool manual_switch;

	bool isAlive;
}rc_t;

rc_t radioController;

void rc_config_channel(channel_t *channels);
void rc_print_channel_values();

void rc_periodical();

#endif /* RC_H_ */

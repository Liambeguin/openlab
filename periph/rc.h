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



typedef struct {
	gpio_t 			port;
	gpio_pin_t 		pin;
	syscfg_port_t 	syscfg_port;

	uint32_t 		last_time;
	uint32_t 		value;
}channel_t;


typedef struct {

	struct{
		float RC_FACTOR_THRUST;
		float RC_FACTOR_ROLLPITCH;
		float RC_FACTOR_YAW;
	};

	struct{
		float throttle;

		float roll;
		float pitch;
		float yaw;

		bool kill_switch;
		bool manual_switch;

		bool isAlive;
	};

}rc_t;

rc_t drone_radioController;

void rc_config_channel(timer_t timer, channel_t *channels);
void rc_print_channel_values();
// Convert on  [0; 1] scale
float get_power(uint32_t channel_value);
// Convert on [-1; 1] scale
float get_rad  (uint32_t channel_value);

void rc_periodical();

#endif /* RC_H_ */

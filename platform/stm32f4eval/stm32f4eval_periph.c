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
 * stm32f4eval_periph.c
 *
 *  Created on: 2013-10-13
 *      Author: liam <liambeguin.at.gmail.com>
 */

#include "stm32f4eval.h"

#include "spi.h"
#include "gpio.h"


#include "debug.h"
#include "stm32f4xx/nvic_.h"
#include "stm32f4xx/syscfg.h"


/** SPI_2 setup **/
static void spi_setup();
#define CS_PORT GPIO_C
#define CS_PIN 	GPIO_PIN_3

/** PWM setup **/
#include "motors.h"
static void motors_setup();

/** RC setup **/
#include "rc.h"
static void rc_setup();




void platform_periph_setup()
{
//	spi_setup();
	motors_setup();
	rc_setup();
}

static void spi_setup(){

	gpio_enable(CS_PORT);
	gpio_set_output(CS_PORT, CS_PIN);
	gpio_config_output_type(CS_PORT, CS_PIN, GPIO_TYPE_PUSH_PULL);
	gpio_config_pull_up_down(CS_PORT, CS_PIN, GPIO_PULL_UP);

	//set Chip Select
	gpio_pin_set(CS_PORT, CS_PIN);
}

static void motors_setup(){


	//array containing the configuration of the different motors
	motor_t motors[] = {
			{
					.timer 		= TIM_4,
					.channel	= TIMER_CHANNEL_1,
					.alternate 	= GPIO_AF_2,
					.port 		= GPIO_B,
					.pin 		= GPIO_PIN_6
			},
			{
					.timer 		= TIM_4,
					.channel	= TIMER_CHANNEL_2,
					.alternate 	= GPIO_AF_2,
					.port 		= GPIO_B,
					.pin 		= GPIO_PIN_7
			},
			{
					.timer 		= TIM_4,
					.channel	= TIMER_CHANNEL_3,
					.alternate 	= GPIO_AF_2,
					.port 		= GPIO_B,
					.pin 		= GPIO_PIN_8
			},
			{
					.timer 		= TIM_4,
					.channel	= TIMER_CHANNEL_4,
					.alternate 	= GPIO_AF_2,
					.port 		= GPIO_B,
					.pin 		= GPIO_PIN_9
			}

	};


	/**
	 * Enabling GPIOs related to the motors
	 */
	uint8_t i;
	for(i=0; i<sizeof(motors)/sizeof(motors[0]); i++){
		gpio_enable(motors[i].port);
		gpio_set_timer_output(motors[i].port, motors[i].pin, (gpio_af_t)motors[i].alternate);
	}

	motors_config(motors);
	motors_idle();
}

static void rc_setup(){

	channel_t channels[] = {
			{
					.port = GPIO_B,
					.pin = GPIO_PIN_10,
					.syscfg_port = SYSCFG_PORT_B,
					.last_time = 0,
					.value = 0,
			},
			{
					.port = GPIO_B,
					.pin = GPIO_PIN_11,
					.syscfg_port = SYSCFG_PORT_B,
					.last_time = 0,
					.value = 0,
			},
			{
					.port = GPIO_B,
					.pin = GPIO_PIN_12,
					.syscfg_port = SYSCFG_PORT_B,
					.last_time = 0,
					.value = 0,
			},
			{
					.port = GPIO_B,
					.pin = GPIO_PIN_13,
					.syscfg_port = SYSCFG_PORT_B,
					.last_time = 0,
					.value = 0,
			},
			{
					.port = GPIO_B,
					.pin = GPIO_PIN_14,
					.syscfg_port = SYSCFG_PORT_B,
					.last_time = 0,
					.value = 0,
			},
			{
					.port = GPIO_B,
					.pin = GPIO_PIN_15,
					.syscfg_port = SYSCFG_PORT_B,
					.last_time = 0,
					.value = 0,
			},

	};

	rc_config_channel(channels);
}


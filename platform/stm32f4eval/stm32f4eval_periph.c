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
static void accelero_setup();
#define CHIP_SELECT_PORT	GPIO_E
#define CHIP_SELECT_PIN		GPIO_PIN_3

/** PWM setup **/
#include "motors.h"
static void motors_setup();

/** RC setup **/
#include "rc.h"
static void rc_setup();

//static void gx3_setup();

void platform_periph_setup()
{
//	accelero_setup();
//	motors_setup();
	rc_setup();
}

static void accelero_setup(){

	lis302dl_config(SPI_1, CHIP_SELECT_PORT, CHIP_SELECT_PIN);
	lis302dl_init();
}

static void motors_setup(){

#define TIM_TEST
#ifdef TIM_TEST
	motor_t motors[] = {
			{
					.timer 		= TIM_1,
					.channel	= TIMER_CHANNEL_1,
					.alternate 	= GPIO_AF_1,
					.port 		= GPIO_E,
					.pin 		= GPIO_PIN_9
			},
			{
					.timer 		= TIM_1,
					.channel	= TIMER_CHANNEL_2,
					.alternate 	= GPIO_AF_1,
					.port 		= GPIO_E,
					.pin 		= GPIO_PIN_11
			},
			{
					.timer 		= TIM_1,
					.channel	= TIMER_CHANNEL_3,
					.alternate 	= GPIO_AF_1,
					.port 		= GPIO_E,
					.pin 		= GPIO_PIN_13
			},
			{
					.timer 		= TIM_1,
					.channel	= TIMER_CHANNEL_4,
					.alternate 	= GPIO_AF_1,
					.port 		= GPIO_E,
					.pin 		= GPIO_PIN_14
			}
	};
#else
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
#endif


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


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
 * behemoth_periph.c
 *
 *  Created on: 2013-10-13
 *      Author: liam <liambeguin.at.gmail.com>
 */

#include "behemoth.h"

#include "spi.h"
#include "gpio.h"

#define NO_DEBUG_HEADER
#include "debug.h"
#include "stm32f4xx/nvic_.h"
#include "stm32f4xx/syscfg.h"


/** PWM setup **/
#include "motors.h"
static void motors_setup();

// PE8  - MUX SELECT
// allows to chose if motors are driven by FPGA or MCU
#define MUX_SELECT_PORT	GPIO_E
#define MUX_SELECT_PIN 	GPIO_PIN_8

// PE15 - MUX ENABLE
#define MUX_ENABLE_PORT	GPIO_E
#define MUX_ENABLE_PIN 	GPIO_PIN_15

// PE12 - LEVEL SHIFTER ENABLE
#define LEVELSHIFTER_EN_PORT	GPIO_E
#define LEVELSHIFTER_EN_PIN		GPIO_PIN_12


/** RC setup **/
#include "rc.h"
static void rc_setup();

/** IMU setup **/
#include "gx3.h"
static void gx3_setup();
#define GX3_BAUDRATE	230400
//#define GX3_BAUDRATE	115200

/** LED DRIVER setup **/
#include "tlc59116.h"
static void led_driver_setup();
//#define LED_DRIVER_ADDRESS 0x68
// All Call Address
#define LED_DRIVER_ADDRESS 0xD0


void platform_periph_setup()
{
	motors_setup();
	rc_setup();
	led_driver_setup();
	gx3_setup();
}

static void led_driver_setup(){

	tlc59116_config(I2C_2, LED_DRIVER_ADDRESS);
	log_info("[I2C] writing on 0x%0x", LED_DRIVER_ADDRESS);
	tlc59116_init();
}

static void motors_setup(){

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

	/**
	 * Setting up LEVEL_SHIFTER
	 */
	gpio_enable(LEVELSHIFTER_EN_PORT);
	gpio_set_output(LEVELSHIFTER_EN_PORT, LEVELSHIFTER_EN_PIN);
	gpio_pin_set(LEVELSHIFTER_EN_PORT, LEVELSHIFTER_EN_PIN);


	/**
	 * Setting up MUX
	 */
	gpio_enable(MUX_ENABLE_PORT);
	gpio_set_output(MUX_ENABLE_PORT, MUX_ENABLE_PIN);
	gpio_pin_clear(MUX_ENABLE_PORT, MUX_ENABLE_PIN);

	gpio_enable(MUX_SELECT_PORT);
	gpio_set_output(MUX_SELECT_PORT, MUX_SELECT_PIN);
	gpio_pin_set(MUX_SELECT_PORT, MUX_SELECT_PIN);
	/**/

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
					.port = GPIO_C,
					.pin = GPIO_PIN_6,
					.syscfg_port = SYSCFG_PORT_C,
					.last_time = 0,
					.value = 0,
			},
			{
					.port = GPIO_C,
					.pin = GPIO_PIN_7,
					.syscfg_port = SYSCFG_PORT_C,
					.last_time = 0,
					.value = 0,
			},
			{
					.port = GPIO_B,
					.pin = GPIO_PIN_0,
					.syscfg_port = SYSCFG_PORT_B,
					.last_time = 0,
					.value = 0,
			},
			{
					.port = GPIO_B,
					.pin = GPIO_PIN_1,
					.syscfg_port = SYSCFG_PORT_B,
					.last_time = 0,
					.value = 0,
			},
			{
					.port = GPIO_D,
					.pin = GPIO_PIN_12,
					.syscfg_port = SYSCFG_PORT_D,
					.last_time = 0,
					.value = 0,
			},
			{
					.port = GPIO_D,
					.pin = GPIO_PIN_13,
					.syscfg_port = SYSCFG_PORT_D,
					.last_time = 0,
					.value = 0,
			},
			{
					.port = GPIO_D,
					.pin = GPIO_PIN_14,
					.syscfg_port = SYSCFG_PORT_D,
					.last_time = 0,
					.value = 0,
			},
//			{
//					.port = GPIO_D,
//					.pin = GPIO_PIN_15,
//					.syscfg_port = SYSCFG_PORT_D,
//					.last_time = 0,
//					.value = 0,
//			},

	};

	rc_config_channel(channels);
}

static void gx3_setup(){

	// Enable the uart
	gpio_set_uart_tx(GPIO_D, GPIO_PIN_8);
	gpio_set_uart_rx(GPIO_D, GPIO_PIN_9);

	uart_enable(UART_3, GX3_BAUDRATE);

	//initializing GX3 communication
	GX3_init(UART_3);
}



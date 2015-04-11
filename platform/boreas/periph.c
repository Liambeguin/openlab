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
 * boreas_periph.c
 *
 *  Created on: Aug 01, 2014
 *      Author: liam <liambeguin.at.gmail.com>
 */

#include <string.h>
#include "boreas.h"

#include "spi.h"
#include "gpio.h"

#define NO_DEBUG_HEADER
#include "debug.h"
#include "boot_log.h"
#include "stm32f4xx/nvic_.h"
#include "stm32f4xx/syscfg.h"


/** PWM setup **/
#include "motors.h"
static void motors_setup();

// PE12 - LEVEL SHIFTER ENABLE
#define LEVELSHIFTER_EN_PORT	GPIO_E
#define LEVELSHIFTER_EN_PIN	GPIO_PIN_10


/** RC setup **/
#include "rc.h"
static void rc_setup();

/** IMU setup **/
#include "vn200.h"
static void vn200_setup();

/** LED DRIVER setup **/
#include "tlc59116.h"
static void led_driver_setup();
// All Call Address
#define LED_DRIVER_ADDRESS 0xC0
#define LED_ALLCALLADDR	   0xD0

/** DataLink setup **/
#include "datalink.h"
static void datalink_setup();

void platform_periph_setup()
{
//	datalink_setup();
	motors_setup();
	rc_setup();
 	led_driver_setup();
	vn200_setup();
}

static void led_driver_setup(){

	tlc59116_config(I2C_2, LED_DRIVER_ADDRESS);

	if(tlc59116_init())
		boot_failure("LED driver sent back an error \n");
	else
		boot_success("TLC59116 initialized on I2C_2 (turning LEDs off)\n");
}

static void motors_setup(){

	_motor_t motors[] = {
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
	 * Enabling GPIOs related to the motors
	 */
	uint8_t i;
	for(i=0; i<sizeof(motors)/sizeof(motors[0]); i++){
		gpio_enable(motors[i].port);
		gpio_set_timer_output(motors[i].port, motors[i].pin,
					(gpio_af_t)motors[i].alternate);
	}

	motors_config(motors);
	motors_idle();
	boot_success("Motors initialized and set to idle speed !\n");
}

static void rc_setup(){

	channel_t channels[] = {
		{
			.port = GPIO_D,
			.pin = GPIO_PIN_14,
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
			.pin = GPIO_PIN_12,
			.syscfg_port = SYSCFG_PORT_D,
			.last_time = 0,
			.value = 0,
		},
		{
			.port = GPIO_D,
			.pin = GPIO_PIN_11,
			.syscfg_port = SYSCFG_PORT_D,
			.last_time = 0,
			.value = 0,
		},
		{
			.port = GPIO_D,
			.pin = GPIO_PIN_10,
			.syscfg_port = SYSCFG_PORT_D,
			.last_time = 0,
			.value = 0,
		},
		{
			.port = GPIO_D,
			.pin = GPIO_PIN_9,
			.syscfg_port = SYSCFG_PORT_D,
			.last_time = 0,
			.value = 0,
		},
		{
			.port = GPIO_D,
			.pin = GPIO_PIN_8,
			.syscfg_port = SYSCFG_PORT_D,
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
		{
			.port = GPIO_B,
			.pin = GPIO_PIN_14,
			.syscfg_port = SYSCFG_PORT_B,
			.last_time = 0,
			.value = 0,
		},

	};

	if (rc_config_channel(TIM_2, channels))
		boot_failure("RC maybe exti or nvic problem \n");
	else
		boot_success("RC configured \n");
}

/* DataLink connected to U1 */
static void datalink_setup(){

	datalink_config(UART_3);

	uart_transfer(UART_3,(uint8_t*)"hello world!!\n", 15);
	boot_success("DataLink initialized on U1\n");
}

/* VN200 on RS232-1, RS232-2 or UART_5 */
#define VNPORT_RS232_2
static void vn200_setup() {

	uint8_t* ret;

#if defined(VNPORT_RS232_1)

	vn200_init(UART_4);
	boot_success("VN200 initializing on RS232-1... \n");

#elif defined(VNPORT_RS232_2)

	vn200_init(UART_2);
	boot_success("VN200 initializing on RS232-2... \n");

#elif defined(VNPORT_U2)

	vn200_init(UART_5);
	boot_success("VN200 initializing on UART_5... \n");

#else
	boot_failure("VN200 bad port definition !\n");
	return;
#endif
	ret = vn200_getModel();

	if (!strcmp((char*)ret, "error")) {
      		boot_failure("VN200 failed to getModel, sensor may not be connected ... \n");
	}else{
		boot_success("%s initialized !! \n", ret);
	}
}

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
#define LEVELSHIFTER_EN_PIN	GPIO_PIN_12


/** RC setup **/
#include "rc.h"
static void rc_setup();

/** IMU setup **/
#include "vn200.h"
static void vn200_setup();
#define VN200_BAUDRATE	460800

/** LED DRIVER setup **/
#include "tlc59116.h"
static void led_driver_setup();
// All Call Address
#define LED_DRIVER_ADDRESS 0xC0
#define LED_ALLCALLADDR	   0xD0

/** DataLink setup **/
#include "datalink.h"
static void datalink_setup();
#define DATALINK_BAUDRATE 9600

void platform_periph_setup()
{
	datalink_setup();
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
			.pin = GPIO_PIN_12,
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

	};

	if (rc_config_channel(TIM_2, channels))
		boot_failure("RC (maybe exti or nvic problem \n");
	else
		boot_success("RC configured \n");
}

/* DataLink connected to U1 */
static void datalink_setup(){

	// Enable the uart
	gpio_set_uart_tx(GPIO_C, GPIO_PIN_10);
	gpio_set_uart_rx(GPIO_C, GPIO_PIN_11);

	uart_enable(UART_3, DATALINK_BAUDRATE);

	datalink_config(UART_3);

	boot_success("DataLink initialized on U1\n");
}

/* VN200 on RS232-1 */
static void vn200_setup() {

	// Enable the uart
	gpio_set_uart_tx(GPIO_A, GPIO_PIN_0);
	gpio_set_uart_rx(GPIO_A, GPIO_PIN_1);

	uart_enable(UART_4, VN200_BAUDRATE);

	vn200_init(UART_4);
	vn200_getModel();

	if (vn200_getModel()){
		boot_failure("IMU \n");
	}else{
		boot_success("IMU initialized on RS232-1 \n");
	}
}

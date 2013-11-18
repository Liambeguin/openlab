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

/** SPI_2 setup **/
static void spi_setup();
#define CS_PORT GPIO_C
#define CS_PIN 	GPIO_PIN_3

/** PWM setup **/
#include "motors.h"
static void motors_setup();

/** RC setup **/
#include "rcc.h"
#include "exti.h"
#include "syscfg.h"
#include "nvic.h"
#include "nvic_.h"
#include "rc.h"
static void rc_setup();



void platform_periph_setup()
{
//	spi_setup();
//	motors_setup();
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


void test(){
	static uint32_t last_time;

	if(gpio_pin_read(GPIO_B, GPIO_PIN_10)){
//		log_error("UP!!");
		last_time = soft_timer_time();
	}
	else{
//		log_error("DOWN !!");
//		printf("time : %d                   \r", soft_timer_ticks_to_ms(soft_timer_time()-last_time));
		printf("time : %d                   \r", soft_timer_time()-last_time);
	}

}

static void rc_setup(){


	log_debug("rc_setup");

	//test on orange LED to trigger irq
	channel_t trigger = {
			.port = GPIO_D,
			.pin = GPIO_PIN_13,
	};
	rc_config(trigger);

	gpio_enable(trigger.port);
	gpio_set_output(trigger.port, trigger.pin);


	//enabling IRQ
	gpio_set_input(GPIO_B, GPIO_PIN_10);
	exti_set_handler(EXTI_LINE_Px10, test, NULL);
	exti_enable_interrupt_line(EXTI_LINE_Px10, EXTI_TRIGGER_BOTH);


	syscfg_select_exti_pin(EXTI_LINE_Px10, SYSCFG_PORT_B);
	nvic_enable_interrupt_line(NVIC_IRQ_LINE_EXTI15_10);
}




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
 */

#include "platform.h"
#include "debug.h"
#include <stdint.h>
#include <stdbool.h>

#include "rc.h"
#include "gpio.h"
#include "exti.h"
#include "stm32f4xx/syscfg.h"
#include "stm32f4xx/nvic_.h"
#include "timer.h"

#include "FreeRTOS.h"
#include "task.h"

#define TASK_RC_PERIOD				(configTICK_RATE_HZ*10/1000)	// 10 ms


//Start sequence on radio controller (down-left on two joysticks)
#define START_SEQUENCE		(drone_radioController.throttle <=  0.1f	&& \
							 drone_radioController.yaw		<= -2.0f 	&& \
							 drone_radioController.pitch	>=  0.4f	&& \
							 drone_radioController.roll 	<= -0.3f  	)




/**
 * To change span, change timer frequency in <platform>_lib.c
 */

channel_t _channel[6];
timer_t _timer;

void generic_exti_handler(void *arg){

	if(gpio_pin_read(_channel[(uint32_t)arg].port, _channel[(uint32_t)arg].pin)){
		_channel[(uint32_t)arg].last_time = soft_timer_time();
	}
	else{
		_channel[(uint32_t)arg].value = soft_timer_time()-_channel[(uint32_t)arg].last_time;
	}
}

void rc_config_channel(timer_t timer, channel_t *channels){

	_timer = timer;
	exti_line_t exti_line;
	nvic_irq_line_t nvic_line;


	drone_radioController.RC_FACTOR_THRUST		= 1.25f; //
	drone_radioController.RC_FACTOR_ROLLPITCH	= 0.52f; // 30 degrés
	drone_radioController.RC_FACTOR_YAW 		= 3;     //


	uint32_t i;
	for(i=0; i<sizeof(_channel)/sizeof(_channel[0]); i++){

		_channel[i] = channels[i];

		switch (_channel[i].pin){
		case GPIO_PIN_0:
			exti_line = EXTI_LINE_Px0;
			nvic_line = NVIC_IRQ_LINE_EXTI0;
			break;

		case GPIO_PIN_1:
			exti_line = EXTI_LINE_Px1;
			nvic_line = NVIC_IRQ_LINE_EXTI1;
			break;
		case GPIO_PIN_2:
			exti_line = EXTI_LINE_Px2;
			nvic_line = NVIC_IRQ_LINE_EXTI2;
			break;
		case GPIO_PIN_3:
			exti_line = EXTI_LINE_Px3;
			nvic_line = NVIC_IRQ_LINE_EXTI3;
			break;
		case GPIO_PIN_4:
			exti_line = EXTI_LINE_Px4;
			nvic_line = NVIC_IRQ_LINE_EXTI4;
			break;
		case GPIO_PIN_5:
			exti_line = EXTI_LINE_Px5;
			nvic_line = NVIC_IRQ_LINE_EXTI9_5;
			break;
		case GPIO_PIN_6:
			exti_line = EXTI_LINE_Px6;
			nvic_line = NVIC_IRQ_LINE_EXTI9_5;
			break;
		case GPIO_PIN_7:
			exti_line = EXTI_LINE_Px7;
			nvic_line = NVIC_IRQ_LINE_EXTI9_5;
			break;
		case GPIO_PIN_8:
			exti_line = EXTI_LINE_Px8;
			nvic_line = NVIC_IRQ_LINE_EXTI9_5;
			break;
		case GPIO_PIN_9:
			exti_line = EXTI_LINE_Px9;
			nvic_line = NVIC_IRQ_LINE_EXTI9_5;
			break;
		case GPIO_PIN_10:
			exti_line = EXTI_LINE_Px10;
			nvic_line = NVIC_IRQ_LINE_EXTI15_10;
			break;
		case GPIO_PIN_11:
			exti_line = EXTI_LINE_Px11;
			nvic_line = NVIC_IRQ_LINE_EXTI15_10;
			break;
		case GPIO_PIN_12:
			exti_line = EXTI_LINE_Px12;
			nvic_line = NVIC_IRQ_LINE_EXTI15_10;
			break;
		case GPIO_PIN_13:
			exti_line = EXTI_LINE_Px13;
			nvic_line = NVIC_IRQ_LINE_EXTI15_10;
			break;
		case GPIO_PIN_14:
			exti_line = EXTI_LINE_Px14;
			nvic_line = NVIC_IRQ_LINE_EXTI15_10;
			break;
		case GPIO_PIN_15:
			exti_line = EXTI_LINE_Px15;
			nvic_line = NVIC_IRQ_LINE_EXTI15_10;
			break;
		default:
			log_error("exti_line not defined");
			exti_line = -1;
			nvic_line = -1;
			break;
		}

		if(exti_line != -1 && nvic_line != -1){
			gpio_set_input(_channel[i].port, _channel[i].pin);
			exti_set_handler(exti_line, generic_exti_handler, (void*)i);
			exti_enable_interrupt_line(exti_line, EXTI_TRIGGER_BOTH);
			syscfg_select_exti_pin(exti_line, _channel[i].syscfg_port);
			nvic_enable_interrupt_line(nvic_line);
		}
	}

	log_debug("[RC] span : %d", 0);
	log_debug("[RC] config OK !");
}


/** DEBUG method **/
void rc_print_channel_values(){
	uint32_t i;
	for(i=0; i<6; i++){
		printf("CH%d:%d\t", i+1, _channel[i].value);
	}
	printf("\n");
}



// Convert on  [0; 1] scale
float get_power(uint32_t channel_value){
	return 1000.0f*(float)channel_value/(float)timer_get_frequency(_timer)-1.0f;
}
// Convert on [-1; 1] scale
float get_rad  (uint32_t channel_value){
	return (1000.0f*(float)channel_value/(float)timer_get_frequency(_timer)-1.0f)*2.0f-1.0f;
}



void rc_periodical(void *arg){

	while(1){

	static uint32_t last_timer_time = 0;

	drone_radioController.throttle 	= ( get_power(_channel[0].value) *drone_radioController.RC_FACTOR_THRUST) - 0.15f;

	drone_radioController.isAlive	= true;

	drone_radioController.roll 		= get_rad(_channel[1].value) * drone_radioController.RC_FACTOR_ROLLPITCH;
	drone_radioController.pitch 	= get_rad(_channel[2].value) * drone_radioController.RC_FACTOR_ROLLPITCH;
	drone_radioController.yaw 		= get_rad(_channel[3].value) * drone_radioController.RC_FACTOR_YAW;


	drone_radioController.kill_switch	= (get_power(_channel[4].value)<0.5f)? true:false ;
	drone_radioController.manual_switch = (get_power(_channel[5].value)<0.5f)? true:false ;


	if (START_SEQUENCE){

		last_timer_time = soft_timer_time();
		drone_radioController.start_sequence = 1;
	}
	else if (soft_timer_time()-last_timer_time >= 500)
		drone_radioController.start_sequence = 0;


	vTaskDelay(TASK_RC_PERIOD);
	}
}






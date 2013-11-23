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

#include "rc.h"
#include "gpio.h"
#include "exti.h"
#include "stm32f4xx/syscfg.h"
#include "stm32f4xx/nvic_.h"


/** TODO
 *
 *  voir pour augmenter la frequence de la
 *  clock liee aux exti pour avoir une plus
 *  grande sensibilite sur la manette
 *
 *  voir dans stm32f4eval.c pour les clocks du sys
 *
 */

channel_t _channel[6];

void generic_exti_handler(void *arg){

	if(gpio_pin_read(_channel[(uint32_t)arg].port, _channel[(uint32_t)arg].pin)){
		_channel[(uint32_t)arg].last_time = soft_timer_time();
	}
	else{
		_channel[(uint32_t)arg].value = soft_timer_time()-_channel[(uint32_t)arg].last_time;
	}
}

void rc_config_channel(channel_t *channels){

	exti_line_t exti_line;
	nvic_irq_line_t nvic_line;

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
}

void rc_print_channel_values(){
	uint32_t i;
	for(i=0; i<6; i++){
		printf("CH%d:%d\t", i+1, _channel[i].value);
	}
	printf("\n");
}

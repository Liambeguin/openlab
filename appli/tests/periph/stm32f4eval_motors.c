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
 * stm32f4eval_motors.c
 *
 *  Created on: 2013-10-28
 *      Author: liam <liambeguin.at.gmail.com>
 */



#define NO_DEBUG_HEADER

#include <stdint.h>
#include "printf.h"
#include "debug.h"

#include "platform.h"
#include "motors.h"

#include "FreeRTOS.h"
#include "task.h"

static void char_rx(handler_arg_t arg, uint8_t c);
void pwm_dummy_task(void *arg);

uint16_t timer_compare = 0;


int main(void){

    signed portBASE_TYPE ret;

	//init platform
	platform_init();

	printf("\n\n Test of dummy PWM on STM32F4eval "
				"\n -------------------\n");

	// Set led to show device is on
//    leds_on(F4_LED_RED);

    ret =   xTaskCreate(pwm_dummy_task, (const signed char * const) "PWM_dummy",
                configMINIMAL_STACK_SIZE, NULL, 1, NULL);

    switch (ret)
    {
        case errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY:
            printf("Error: Could not allocate required memory\n");
            while (1);
            return 0;

        default:
            printf("PWM_dummy task created successfully\n");
    }

    // Start the scheduler
    platform_run();
}

static void char_rx(handler_arg_t arg, uint8_t c){


//	log_error("entered : %c", c);
	if(c == 'z' && timer_compare < (0xffff))
		timer_compare ++;
	if(c == 'x' && timer_compare > 0)
		timer_compare --;
	if(c == '5')
		ppm_update(0.5);
	if(c == '0')
		motors_idle();

}

void pwm_dummy_task(void *arg){

    // arg is not used
    (void) arg;
    uint32_t last_time = 0;

    leds_on(F4_LED_GREEN);

    uart_set_rx_handler(uart_print, char_rx, NULL);

    printf("Handler set ! \n\n");

    while (1){

    	printf("timer_compare value : 0x%04x \r", timer_compare);
    	dummy_timer(timer_compare);

    	vTaskDelay(configTICK_RATE_HZ/4);
    }
}




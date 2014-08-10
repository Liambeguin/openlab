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
void ppm_dummy_task(void *arg);

float ratio = 0;


int main(void){

    signed portBASE_TYPE ret;

	//init platform
	platform_init();

	printf("\n\n Test of dummy PPM on STM32F4eval "
				"\n -------------------\n");

	// Set led to show device is on
    leds_on(0xff);

    ret =   xTaskCreate(ppm_dummy_task, (const signed char * const) "PPM_dummy",
                configMINIMAL_STACK_SIZE, NULL, 1, NULL);

    switch (ret)
    {
        case errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY:
            printf("Error: Could not allocate required memory\n");
            while (1);
            return 0;

        default:
            printf("PWM_dummy task created successfully\n");
            break;
    }
    printf("\e[2J Usage :\n"
           "=======================================\n"
           "  press \e[92mz\e[0m to augment and \e[92mx\e[0m to lower\n"
           "  the ratio sent to the motors\n"
           "========================================\n\n");

    // Start the scheduler
    platform_run();
}

static void char_rx(handler_arg_t arg, uint8_t c){

//	log_error("entered : %c", c);
	if(c == 'z')
		ratio += 0.01;
	if(c == 'x')
		ratio -= 0.01;

//	checking range of ratio
	ratio = (ratio <= 0 ? 0 : (ratio >= 1 ? 1 : ratio));
}

void ppm_dummy_task(void *arg){

    // arg is not used
    (void) arg;

    leds_off(0xff);
    leds_on(LED_1);

    uart_set_rx_handler(uart_print, char_rx, NULL);

    while (1){

    	printf("ratio : %f                    \r", ratio);
    	motors_ratio(ratio, ratio, ratio, ratio);

    	vTaskDelay(configTICK_RATE_HZ/2);
    }
}




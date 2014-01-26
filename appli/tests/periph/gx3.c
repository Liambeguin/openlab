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
 * gx3.c
 *
 *  Created on: Jan 22, 2014
 *      Author: liam <liambeguin.at.gmail.com>
 */


#include <stdint.h>
#include "printf.h"
#define NO_DEBUG_HEADER
#include "debug.h"

#include "platform.h"

#include "FreeRTOS.h"
#include "task.h"


void gx3_dummy_task(void *arg);

int main(void){

    signed portBASE_TYPE ret;

	//init platform
	platform_init();
	printf("\n\n Test of dummy GX3"
				"\n -------------------\n");

	// Set led to show device is on
    leds_on(LED_0);

    // Create a task to read whoami
    ret =   xTaskCreate(gx3_dummy_task, (const signed char * const) "GX3_dummy",
                configMINIMAL_STACK_SIZE, NULL, 1, NULL);

    switch (ret)
    {
        case errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY:
            printf("Error: Could not allocate required memory\n");
            while (1);
            return 0;

        default:
            printf("GX3_dummy task created successfully\n");
    }

    // Start the scheduler
    platform_run();
}

void gx3_dummy_task(void *arg){

    // arg is not used
    (void) arg;

    uint8_t test = 'a';

    while (1){
//    	uart_transfer(uart_external, &test, 1);
    	vTaskDelay(configTICK_RATE_HZ/4);

    }
}

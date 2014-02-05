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

#include "gx3.h"

void gx3_periodical_task(void *arg);
void gx3_decode_task(void *arg);

void gx3_periodical_task(void *arg){

    // arg is not used
    (void) arg;

    while (1){
    	GX3_periodical();
    	vTaskDelay(configTICK_RATE_HZ/10);

    }
//    	while(1);
}


void gx3_decode_task(void *arg){

    // arg is not used
    (void) arg;

    while (1){
    	GX3_decode_uart_rx();
    	vTaskDelay(configTICK_RATE_HZ/300);

    }
}



int main()
{
    xTaskHandle blink_handle, hello_handle;
    signed portBASE_TYPE ret;

    // Initialize the platform
    platform_init();

    printf("FreeRTOS Test Program\n");

    // Set initial values
    leds_on(LED_0);
    leds_off(LED_1);

    // Create a task to blink
    ret = xTaskCreate(gx3_decode_task, (const signed char * const) "gx3_decode_task",
                      configMINIMAL_STACK_SIZE, NULL, 1, &blink_handle);

    switch (ret)
    {
        case errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY:
            printf("Error: Could not allocate required memory\n");

            while (1)
            {
                ;
            }

            return 0;

        default:
            printf("Blink task created successfully\n");
    }

    // Create a task to print hello world
    xTaskCreate(gx3_periodical_task, (const signed char * const) "gx3_periodical_task",
                configMINIMAL_STACK_SIZE, NULL, 1, &hello_handle);

    switch (ret)
    {
        case errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY:
            printf("Error: Could not allocate required memory\n");

            while (1)
            {
                ;
            }

            return 0;

        default:
            printf("Hello task created successfully\n");
    }

    // Start the scheduler
    platform_run();

    return 0;
}


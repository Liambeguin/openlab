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
 * stm32f4eval_accelero.c
 *
 *  Created on: 2013-10-13
 *      Author: liam <liambeguin.at.gmail.com>
 */

#include <stdint.h>

#include "platform.h"
#include "lis302dl.h"
#include "printf.h"
#define NO_DEBUG_HEADER
#include "debug.h"

#include "FreeRTOS.h"
#include "task.h"

void acc_whoami_task(void *arg);

int main(void){

    signed portBASE_TYPE ret;

	//init platform
	platform_init();
	printf("Test of default accelerometer on STM32F4eval "
				"\n -------------------\n");

	// Set led to show device is on
    leds_on(LED_0);

    // Create a task to read whoami
    ret =   xTaskCreate(acc_whoami_task, (const signed char * const) "WHOAMI",
                configMINIMAL_STACK_SIZE, NULL, 1, NULL);

    switch (ret)
    {
        case errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY:
            printf("Error: Could not allocate required memory\n");
            while (1);
            return 0;

        default:
            printf("WHOAMI task created successfully\n");
    }

    // Start the scheduler
    platform_run();
}

void acc_whoami_task(void *arg)
{
    // arg is not used
    (void) arg;



    while (1)
    {
    	lis302dl_read_whoami();
//        printf("WhoAmI value [0x%02x] \n", data);
        vTaskDelay(configTICK_RATE_HZ);
    }
}

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
 * void.c
 *
 *  Created on: Jan 24, 2014
 *      Author: liam <liambeguin.at.gmail.com>
 */




#include <stdint.h>
#include "printf.h"
#define NO_DEBUG_HEADER
#include "debug.h"

#include "platform.h"

#include "FreeRTOS.h"
#include "task.h"

void void_task(void *arg);

int main(void){

	signed portBASE_TYPE ret;

	//init platform
	platform_init();

    // Set initial values
    leds_off(0xff);

	printf("\n\n VOID task test"
			"\n -------------------\n");


	/**
	 * Creating Empty task to test driver and/or periph stack
	 */
	ret =   xTaskCreate(void_task, (const signed char * const) "void",
			configMINIMAL_STACK_SIZE, NULL, 1, NULL);

	switch (ret)
	{
	case errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY:
		printf("Error: Could not allocate required memory\n");
		while (1);
		return 0;

	default:
		printf("VOID task created successfully\n");
	}

	// Start the scheduler
	platform_run();
}

void void_task(void *arg){

	// arg is not used
	(void) arg;
    uint8_t count = 0;


	while (1)
	{
		leds_off(0xFF);
		leds_on(count++);
    	vTaskDelay(configTICK_RATE_HZ/4);
	}
}





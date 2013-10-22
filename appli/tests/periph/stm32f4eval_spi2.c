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
 *
 *  LED color code : RED 	= not OK
 *  				 GREEN	= OK
 */

#include <stdint.h>
#include "printf.h"
#define NO_DEBUG_HEADER
#include "debug.h"

#include "platform.h"

#include "FreeRTOS.h"
#include "task.h"


void spi_dummy_task(void *arg);

int main(void){

    signed portBASE_TYPE ret;

	//init platform
	platform_init();
	printf("\n\n Test of dummy SPI_2 on STM32F4eval "
				"\n -------------------\n");

	// Set led to show device is on
    leds_on(F4_LED_RED);

    // Create a task to read whoami
    ret =   xTaskCreate(spi_dummy_task, (const signed char * const) "SPI_dummy",
                configMINIMAL_STACK_SIZE, NULL, 1, NULL);

    switch (ret)
    {
        case errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY:
            printf("Error: Could not allocate required memory\n");
            while (1);
            return 0;

        default:
            printf("SPI_dummy task created successfully\n");
    }

    // Start the scheduler
    platform_run();
}

void spi_dummy_task(void *arg){

    // arg is not used
    (void) arg;

    uint8_t txbuf = 0;

    while (1){

    	printf("txbuf : %02x \t rxbuf : %02x \n", txbuf, platform_spi_single(txbuf));

    	vTaskDelay(configTICK_RATE_HZ);

    	txbuf ++;
        if (txbuf >= 0xff)
        	txbuf = 0;
    }
}

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
 * full_uart.c
 *
 *  Created on: Jan 30, 2014
 *      Author: liam <liambeguin.at.gmail.com>
 */




#include <stdint.h>
#include "printf.h"
#define NO_DEBUG_HEADER
#include "debug.h"

#include "platform.h"


uart_handler_t rx_handler(handler_arg_t arg, uint8_t c){
	log_debug("%c", c);
}

int main(void){

	signed portBASE_TYPE ret;

	//init platform
	platform_init();

    // Set initial values
    leds_off(0xff);
    uint8_t count = 0;

	printf("\n\n uart chain test task"
			"\n -------------------\n");

    uart_set_rx_handler(uart_external, rx_handler, NULL);

    uint8_t test = 'a';

	while (1)
	{
		leds_off(0xFF);
		leds_on(count++);
		uart_transfer(uart_external, &test, 1);

        int i;

        for (i = 0; i < 0x800000; i++)
        {
            __asm__("nop");
        }

	}

	return 0;
}

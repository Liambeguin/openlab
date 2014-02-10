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





#include "printf.h"

#include <stdint.h>
#include "platform.h"
#include "attitudeController.h"

int main()
{
    // Initialize the platform
    platform_init();

    // Set initial values
    leds_off(0xff);
	printf("\n\n VOID task"
			"\n -------------------\n");

    uint8_t count = 0;

    while (1);
    {
        int i;

        for (i = 0; i < 0x80000; i++)
        {
            __asm__("nop");
        }

        leds_off(0xFF);
        leds_on(count++);
    }

    return 0;
}

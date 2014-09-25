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
#include "debug.h"

#include "platform.h"

#include "FreeRTOS.h"
#include "task.h"

#include "vn200.h"

int main()
{
	// Initialize the platform
	platform_init();
	
	printf("VN200 Test Program\n");
	
	// Set initial values
	leds_off(0xff);
	leds_on(0x01);

	//vn200_getModel();
	
	// Start the scheduler
	platform_run();
	
	return 0;
}

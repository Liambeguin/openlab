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

void create_task(char* name, void* func, int param);

void dummy(void){

	while(1){
		vn200_readReg(8);
		vTaskDelay(2*configTICK_RATE_HZ);
	}
}

void dumm2(){

	uint8_t count = 0; 
	while (1){

		vn200_decodeUartRx();
		leds_off(0xFF);
		leds_on(count++);
		vTaskDelay(configTICK_RATE_HZ/100);
	}
}

int main()
{
	// Initialize the platform
	platform_init();

	printf("VN200 Test Program\n");

	// Set initial values
	leds_off(0xff);
	leds_on(0x01);

	/* testing errors 3:O Meuh! */
	vn200_sendRequest((uint8_t*)"$VNRRG,99*XX\r\n",15); // Bad register
	vn200_sendRequest((uint8_t*)"$VNRRG,01*00\r\n",15); // Invalud ChkSum
	vn200_sendRequest((uint8_t*)"$VNARG,01*XX\r\n",15); // Invalid Command
	vn200_sendRequest((uint8_t*)"$VNWRG,05*XX\r\n",15); // Not Enough Param
//	vn200_readReg(USERTAG);
//	vn200_readReg(1);
//	vn200_readReg(2);
//	vn200_readReg(3);
//	vn200_readReg(4);
//	vn200_readReg(5);

//	create_task("sendcmd", dummy, configTICK_RATE_HZ);
	create_task("recvcmd", dumm2, configTICK_RATE_HZ/90);

	// Start the scheduler
	platform_run();

	return 0;
}


void create_task(char* name, void* func, int param){
	switch (xTaskCreate(func, (const signed char * const) name,
				configMINIMAL_STACK_SIZE, (void*)param, 1, NULL))
		{
		case errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY:
			log_error("Could not allocate required memory to %s", name);
			while (1);
			break;

		default:
			log_debug("%s created successfully", name);
			break;
		}
}

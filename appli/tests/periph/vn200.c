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
#include "soft_timer.h"

#include "platform.h"

#include "FreeRTOS.h"
#include "task.h"

#include "vn200.h"

#define ERROR_TEST 1

void vn_send(void *arg)
{

#if ERROR_TEST
	/* Testing Error codes... 3:O Meuh! */

	// 1: Hard Fault
	// 2: Serial Buffer Overflow
	// 3: Invalud ChkSum
	vn200_sendRequest((uint8_t*)"$VNRRG,01*00\r\n",15);
	// 4: Invalid Command
	vn200_sendRequest((uint8_t*)"$VNARG,01*XX\r\n",15);
	// 5: Not Enough Param
	vn200_sendRequest((uint8_t*)"$VNWRG,05*XX\r\n",15);
	// 6: Too Many Param
	vn200_sendRequest((uint8_t*)"$VNWRG,32,0,0,0,0,0,0,0,0,0,0*XX\r\n",40);
	// 7: Invalid  Param
	vn200_sendRequest((uint8_t*)"$VNWRG,05,111111*XX\r\n",25);
	// 8: Invalid register
	vn200_sendRequest((uint8_t*)"$VNRRG,99*XX\r\n",15);
	// 9: Unauthorized Access
	vn200_sendRequest((uint8_t*)"$VNWRG,01*XX\r\n",15);

	/*   ***   */
#endif

	vn200_getReg(USERTAG);
	vn200_getReg(1);
	vn200_getReg(2);
	vn200_getReg(3);
	vn200_getReg(4);
	vn200_getReg(5);

	while (1);
}

int main()
{
	signed portBASE_TYPE ret;
	// Initialize the platform
	platform_init();

	printf("VN200 Test Program\n");

	// Set initial values
	leds_off(0xff);
	leds_on(0x01);

	ret = xTaskCreate(vn_send, (const signed char * const) "send_func",
             configMINIMAL_STACK_SIZE, NULL, 1, NULL);
	switch (ret) {
  case errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY:
  	printf("Error: Could not allocate required memory\n");
    while (1);
    return 0;
	default:
    break;
	}

	// Start the scheduler
	platform_run();

	return 0;
}

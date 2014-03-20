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
 * manual.c
 *
 *  Created on: Mar 16, 2014
 *      Author: liam <liambeguin.at.gmail.com>
 */

#define NO_DEBUG_HEADER

#include <stdbool.h>
#include <stdint.h>
#include "debug.h"

#include "platform.h"
#include "rc.h"
#include "gx3.h"
#include "motors.h"
#include "attitudeController.h"
#include "tlc59116.h"

#include "FreeRTOS.h"
#include "task.h"


//Task Periods [ms]
#define TASK_IMU_PERIOD				(configTICK_RATE_HZ*5/1000)		// 5 ms
#define TASK_IMU_DECODE_PERIOD		(TASK_IMU_PERIOD/30)			// 5/30 ms
#define TASK_CTRL_ATT_PERIOD		(configTICK_RATE_HZ*5/1000)		// 5  ms
#define TASK_RC_PERIOD				(configTICK_RATE_HZ*10/1000)	// 10 ms

void create_task(char* name, void* func, int param);


int main(void){

	platform_init();

	printf( "\n --------------------\n"
	 		"  MANUAL APPLICATION"
		    "\n --------------------\n");

	// Set led to show device is on
	leds_on(0xff);



	create_task("RadioController_task", rc_periodical, 					TASK_RC_PERIOD);
	create_task("Attitude_ctrl_task", 	attitudeController_periodical,	TASK_CTRL_ATT_PERIOD);
	create_task("IMU_task", 			GX3_periodical, 				TASK_IMU_PERIOD);
	create_task("IMU_decod_task", 		imu_decode_periodical,			TASK_IMU_DECODE_PERIOD);

	// Start the scheduler
	platform_run();
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

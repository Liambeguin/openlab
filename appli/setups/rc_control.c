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
 * rc_control.c
 *
 *  Created on: Feb 7, 2014
 *      Author: liam <liambeguin.at.gmail.com>
 *
 */


#define NO_DEBUG_HEADER

#include <stdint.h>
#include "debug.h"

#include "platform.h"
#include "rc.h"
#include "gx3.h"
#include "motors.h"
#include "attitudeController.h"

#include "FreeRTOS.h"
#include "task.h"

//Task Periods [ms]
#define TASK_IMU_PERIOD				soft_timer_ms_to_ticks(5)
#define TASK_CTRL_ATT_PERIOD		soft_timer_ms_to_ticks(5)
#define TASK_RC_PERIOD				soft_timer_ms_to_ticks(10)

void create_task(char* name, void* func);


void rc_task(void *arg){

	uint32_t test = 0;

	while (1){

		rc_periodical();

		if(!drone_radioController.kill_switch)
			log_error("kill mode");
		else if (!drone_radioController.manual_switch)
			log_error("auto mode");
		else if(!drone_radioController.isAlive)
			log_error("RC not alive");
		else
		{}

//		test = soft_timer_time();
		vTaskDelay(10);
//		printf("rc[%d](%d)\n", soft_timer_ticks_to_ms(soft_timer_time()-test), soft_timer_time()-test);

	}
}

void att_ctrl_task(void *arg){

	attitudeController_init();

	uint32_t test = 0;
	while(1){

		if(drone_radioController.isAlive && drone_radioController.kill_switch
				&&drone_radioController.manual_switch){

			attitudeController_periodical();
		}
		else
			motors_kill();
//		vTaskDelay(TASK_CTRL_ATT_PERIOD);
//		test = soft_timer_time();
		vTaskDelay(5);
//		printf("[%d](%d)", soft_timer_ticks_to_ms(soft_timer_time()-test), soft_timer_time()-test);

	}
}
void imu_decode_task(void *arg){

    // arg is not used
    (void) arg;

    while (1){
    	GX3_decode_uart_rx();

    	soft_timer_delay_us(5000/30);
    }
}


void imu_task(void *arg){

	while(1){

		/*** Critical pitch angles ***/
		if (drone_gx3.pitch > 1.5f){
			log_error("PITCH TOO HIGH");
			motors_kill();
		}
		else if (drone_gx3.pitch < -1.5f){
			log_error("PITCH TOO LOW");
			motors_kill();
		}

		/*** Critical roll  angles ***/
		else if (drone_gx3.roll > 1.5f){
			log_error("ROLL TOO HIGH");
			motors_kill();
		}
		else if (drone_gx3.roll < -1.5f){
			log_error("ROLL TOO LOW");
			motors_kill();
		}

//		//IMU Alive ?
//		else if (!drone_gx3.imu_alive)
//			log_error("IMU not alive");

		else
			GX3_periodical();


//		vTaskDelay(TASK_IMU_PERIOD);
		vTaskDelay(5);
	}
}


int main(void){

	//init platform
	platform_init();

	printf("\n\n First RC controlled Application "
			"\n -------------------\n");

	// Set led to show device is on
	leds_on(0xff);



	// Create a task for RC
	create_task("RadioController_task", rc_task);

	// Create a task for RC
	create_task("Attitude_ctrl_task", att_ctrl_task);


	// Create a task for IMU
	create_task("IMU_task", imu_task);
	create_task("IMU_decod_task", imu_decode_task);

	// Start the scheduler
	platform_run();
}


void create_task(char* name, void* func){
	switch (xTaskCreate(func, (const signed char * const) name,
				configMINIMAL_STACK_SIZE, NULL, 1, NULL))
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

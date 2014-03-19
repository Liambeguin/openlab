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

typedef enum{
	STATE_PLATFORM_IDLE,
	STATE_MOTORS_WaitFor_START_SEQ,
	STATE_AUTO,
	STATE_ReadyTo_FLY,
	STATE_GO,

}state_t;

//Start sequence on radio controller (down-left on two joysticks)
#define START_SEQUENCE		(drone_radioController.throttle <=  0.1f	&& \
							 drone_radioController.yaw		<= -2.0f 	&& \
							 drone_radioController.pitch	>=  0.4f	&& \
							 drone_radioController.roll 	<= -0.3f  	)

//idle motors timeout 5 seconds
#define START_TIMEOUT		(soft_timer_time()-last_time > soft_timer_s_to_ticks(3))

//Task Periods [ms]
#define TASK_IMU_PERIOD				(configTICK_RATE_HZ*5/1000)		// 5 ms
#define TASK_IMU_DECODE_PERIOD		(TASK_IMU_PERIOD/30)			// 5/30 ms
#define TASK_CTRL_ATT_PERIOD		(configTICK_RATE_HZ*5/1000)		// 5  ms
#define TASK_RC_PERIOD				(configTICK_RATE_HZ*10/1000)	// 10 ms

void create_task(char* name, void* func);

bool okToFly = false;

void rc_task(void *arg){

	while (1){

		rc_periodical();

		vTaskDelay(TASK_RC_PERIOD);
	}
}

void att_ctrl_task(void *arg){

	attitudeController_init();

	static state_t motors_state = STATE_PLATFORM_IDLE;
	static uint32_t last_time = 0, last_go_time = 0;

	while(1){

		tlc59116_set_led_color(TLC59116_LED0, RGB_OFF);

		/*****************************************/

		if (drone_radioController.kill_switch)
			motors_state = STATE_PLATFORM_IDLE;

		switch(motors_state){

		case STATE_PLATFORM_IDLE:

			motors_kill();
			tlc59116_set_led_color( TLC59116_LED0, RGB_RED);


			if (drone_radioController.manual_switch && !drone_radioController.kill_switch)
				motors_state = STATE_MOTORS_WaitFor_START_SEQ;
			else
				motors_state = STATE_PLATFORM_IDLE;
			break;

//		case STATE_AUTO:
//
//			motors_kill();
//			tlc59116_set_led_color(TLC59116_LED0, RGB_PURPLE);
//
//			if (drone_radioController.manual_switch)
//				motors_state = STATE_PLATFORM_IDLE;
//			else
//				//AUTO mode not yet implemented, this is MANUAL mode
//				motors_state = STATE_AUTO;
//				motors_state = STATE_PLATFORM_IDLE;
//			break;

		case STATE_MOTORS_WaitFor_START_SEQ:

			motors_kill();
			tlc59116_set_led_color(TLC59116_LED0, RGB_ORANGE);

			if (START_SEQUENCE && soft_timer_time()-last_time >= soft_timer_s_to_ticks(3)){

				last_time = soft_timer_time();
				motors_state = STATE_ReadyTo_FLY;
			}
			else
				motors_state = STATE_MOTORS_WaitFor_START_SEQ;

			break;

		case STATE_ReadyTo_FLY:

			// TODO : Make motors rotate without lifting drone
			// CHECK motors_idle()
			motors_idle();
			tlc59116_set_led_color(TLC59116_LED0, RGB_BLUE);

			if(drone_radioController.throttle >= 0.1f)
				motors_state = STATE_GO;

			else{
				if (START_SEQUENCE)
					motors_state = STATE_MOTORS_WaitFor_START_SEQ;
				if(!START_TIMEOUT)
					motors_state = STATE_ReadyTo_FLY;
				else
					motors_state = STATE_MOTORS_WaitFor_START_SEQ;
			}
			break;

		case STATE_GO:

			tlc59116_set_led_color(TLC59116_LED0, RGB_GREEN);

			if(okToFly)
				attitudeController_periodical();
			else
				motors_kill();


			if (drone_radioController.throttle <= 0.1f){

				last_go_time ++;
				motors_idle();
				if (last_go_time >= 300 || START_SEQUENCE){
					motors_state = STATE_PLATFORM_IDLE;
				}
			}
			else{
				motors_state = STATE_GO;
				last_go_time = 0;
			}

			break;

		}

		/*****************************************/


		vTaskDelay(TASK_CTRL_ATT_PERIOD);
	}
}
void imu_decode_task(void *arg){

	// arg is not used
	(void) arg;

	while (1){
		GX3_decode_uart_rx();

		//FIXME : repalce with vTaskDelay
		//vTaskDelay(TASK_IMU_DECODE_PERIOD);
		soft_timer_delay_us(5000/30);
    }
}


void imu_task(void *arg){

	while(1){

		GX3_periodical();

		/*** Critical pitch angles ***/
		if (drone_gx3.pitch > 1.5f){
			log_error("PITCH TOO HIGH");
			okToFly = false;
		}
		else if (drone_gx3.pitch < -1.5f){
			log_error("PITCH TOO LOW");
			okToFly = false;
		}

		/*** Critical roll  angles ***/
		else if (drone_gx3.roll > 1.5f){
			log_error("ROLL TOO HIGH");
			motors_kill();
		}
		else if (drone_gx3.roll < -1.5f){
			log_error("ROLL TOO LOW");
			okToFly = false;
		}
		else
			okToFly = true;


		vTaskDelay(TASK_IMU_PERIOD);
	}
}


int main(void){

	platform_init();

	printf( "\n --------------------\n"
	 		"  MANUAL APPLICATION"
		    "\n --------------------\n");

	// Set led to show device is on
	leds_on(0xff);



	create_task("RadioController_task", rc_task);

	create_task("Attitude_ctrl_task", att_ctrl_task);


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

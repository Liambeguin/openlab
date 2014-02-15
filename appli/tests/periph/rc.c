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
 * stm32f4eval_rc.c
 *
 *  Created on: Nov 15, 2013
 *      Author: liam <liambeguin.at.gmail.com>
 */





#define NO_DEBUG_HEADER

#include <stdint.h>
#include "printf.h"
#include "debug.h"

#include "platform.h"
#include "rc.h"
#include "attitudeController.h"

#include "FreeRTOS.h"
#include "task.h"

void rc_dummy_task(void *arg);

float ratio = 0;


int main(void){

    signed portBASE_TYPE ret;

	//init platform
	platform_init();

	printf("\n\n Test of dummy RC on STM32F4eval "
				"\n -------------------\n");

	// Set led to show device is on
    leds_on(0xff);

    ret =   xTaskCreate(rc_dummy_task, (const signed char * const) "rc_dummy",
                configMINIMAL_STACK_SIZE, NULL, 1, NULL);

    switch (ret)
    {
        case errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY:
            printf("Error: Could not allocate required memory\n");
            while (1);
            return 0;

        default:
            printf("RC_dummy task created successfully\n");
            break;
    }

    // Start the scheduler
    platform_run();
}



void rc_dummy_task(void *arg){

    // arg is not used
    (void) arg;

    leds_off(0xff);
    leds_on(F4_LED_GREEN);



    attitudeController_init();

    while(1){
//    	rc_print_channel_values();
    	rc_periodical();

    	attitudeController_periodical();

    	if(drone_radioController.kill_switch)
    		log_debug("%f", drone_radioController.throttle);
    	else
    		log_error("%f", drone_radioController.throttle);
    	vTaskDelay(configTICK_RATE_HZ/4);
    }


}

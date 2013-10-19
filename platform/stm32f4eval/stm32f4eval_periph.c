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
 * stm32f4eval_periph.c
 *
 *  Created on: 2013-10-13
 *      Author: liam
 */

#include "stm32f4eval.h"
#include "lis302dl/lis302dl_.h"

/** accelerometer setup */
static void acc_setup();


void platform_periph_setup()
{
	acc_setup();

}


static void acc_setup(){

	//config
	lis302dl_config(SPI_1, GPIO_E, GPIO_PIN_3);

	//init
	lis302dl_init();
}

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

#include "spi.h"
#include "gpio.h"


/** SPI_2 setup */
static void spi_setup();
#define CS_PORT GPIO_C
#define CS_PIN 	GPIO_PIN_3


void platform_periph_setup()
{
	spi_setup();
}

static void spi_setup(){

	gpio_enable(CS_PORT);
	gpio_set_output(CS_PORT, CS_PIN);
	gpio_config_output_type(CS_PORT, CS_PIN, GPIO_TYPE_PUSH_PULL);
	gpio_config_pull_up_down(CS_PORT, CS_PIN, GPIO_PULL_UP);

	//set Chip Select
	gpio_pin_set(CS_PORT, CS_PIN);
}









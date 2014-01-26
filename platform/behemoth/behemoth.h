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
 * behemoth.h
 *
 *  Created on: Jan 16, 2014
 *      Author: liam <liambeguin.at.gmail.com>
 */

#ifndef BEHEMOTH_H_
#define BEHEMOTH_H_

#include "platform.h"
#include "stm32f4xx.h"

void platform_drivers_setup();
void platform_leds_setup();
void platform_button_setup();

void platform_periph_setup();
void platform_lib_setup();
void platform_net_setup();

//todo should be removed but is ued in spi_dummy test
uint8_t platform_spi_single(uint8_t tx_buffer);

#endif /* BEHEMOTH_H_ */

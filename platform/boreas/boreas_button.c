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
 * boreas_button.c
 *
 *  Created on: Aug 01, 2014
 *      Author: liam <liambeguin.at.gmail.com>
 */



#include "platform.h"
#include "boreas.h"

#include "gpio_.h"

#define BUTTON_GPIO_PORT GPIO_A
#define BUTTON_GPIO_PIN GPIO_PIN_10


void platform_button_setup()
{
    gpio_set_output(BUTTON_GPIO_PORT, BUTTON_GPIO_PIN);
}

uint32_t button_state()
{
    return gpio_pin_read(BUTTON_GPIO_PORT, BUTTON_GPIO_PIN);
}


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
 * boreas_lib.c
 *
 *  Created on: Aug 01, 2014
 *      Author: liam <liambeguin.at.gmail.com>
 */

#include "boreas.h"
#include "softtimer/soft_timer_.h"
#include "event.h"

void platform_lib_setup()
{
    // Setup the software timer
    soft_timer_config(TIM_2, TIMER_CHANNEL_1);
    timer_start(TIM_2, 0xFFFF, soft_timer_update, NULL, TIMER_MODE_CLOCK);

    // Setup the event library
    event_init();
}

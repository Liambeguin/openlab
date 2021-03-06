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
 * pitch.h
 *
 *  Created on: Feb 10, 2014
 *      Author: liam <liambeguin.at.gmail.com>
 */

#ifndef PITCH_H_
#define PITCH_H_

#include "motors.h"
#include "gx3.h"

void  att_ctrl_pitch_init(float dt ,float K1, float K2, float lambda);
float att_ctrl_pitch_update(float cmd);


#endif /* PITCH_H_ */

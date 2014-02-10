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
 * roll.h
 *
 *  Created on: Feb 10, 2014
 *      Author: liam <liambeguin.at.gmail.com>
 */

#ifndef ROLL_H_
#define ROLL_H_

#include "motors.h"
#include "gx3.h"

void  att_ctrl_roll_init(float dt ,float K1, float K2, float lambda);
float att_ctrl_roll_update(float cmd);


#endif /* ROLL_H_ */

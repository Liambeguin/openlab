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
 * yaw_.h
 *
 *  Created on: Feb 10, 2014
 *      Author: liam <liambeguin.at.gmail.com>
 */

#ifndef YAW__H_
#define YAW__H_


float yaw_kp;
float yaw_ki;
float yaw_kd;
float dot_yaw;

float error_last = 0;



#endif /* YAW__H_ */

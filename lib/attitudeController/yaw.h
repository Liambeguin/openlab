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
 * yaw.h
 *
 *  Created on: Feb 9, 2014
 *      Author: liam <liambeguin.at.gmail.com>
 */

#ifndef YAW_H_
#define YAW_H_



#define DT_FROM_PARAM	0.005// 5ms => 200 Hz

void att_ctrl_yaw_init(float kp_gain, float ki_gain, float kd_gain, imu_t *imu);
float att_ctrl_yaw_update(float cmd);


#endif /* YAW_H_ */

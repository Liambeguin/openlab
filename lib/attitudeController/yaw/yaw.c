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
 * yaw.c
 *
 *  Created on: Feb 7, 2014
 *      Author: liam <liambeguin.at.gmail.com>
 */

#include "gx3.h"

#include "yaw_.h"
#include "yaw.h"


void att_ctrl_yaw_init(float kp_gain, float ki_gain, float kd_gain, imu_t* imu){
	yaw_kp = kp_gain;
	yaw_ki = ki_gain;
	yaw_kd = kd_gain;

	dot_yaw = imu->dot_yaw;

	error_last = 0;
}

float att_ctrl_yaw_update(float cmd) {
	float error_p = cmd - dot_yaw;
	float error_i = (error_last + error_p) * DT_FROM_PARAM / 2;
	float error_d = (error_last - error_p) * DT_FROM_PARAM;
	float U = yaw_kp * error_p + yaw_ki * error_i + yaw_kd * error_d;
	error_last = error_p;

	return U;
}


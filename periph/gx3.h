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
 * gx3.h
 *
 *  Created on: Nov 26, 2013
 *      Author: liam <liambeguin.at.gmail.com>
 */

#ifndef GX3_H_
#define GX3_H_

#include <stdbool.h>
#include "GX3/gx3_.h"
#include "GX3/gx3_msg.h"

#define EOS (-1)


//TODO use parameter.h

// Constante de gravité du GX3
#define PHYSICS_GX3 9.80665f

void	GX3_init();
int8_t	GX3_get_message_length(const uint8_t id);
void	GX3_process_complete_message(const uint8_t id);
//void	GX3_decode_uart_rx(void);
void 	GX3_write_bias();
void 	GX3_calibrate();
void	GX3_periodical(void);
void	GX3_send_request(uint8_t const *request, uint8_t size);



typedef struct{

	float roll;
	float pitch;
	float yaw;

	float dot_roll;
	float dot_pitch;
	float dot_yaw;

	float accel_x;
	float accel_y;
	float accel_z;

	float gyro_bias_x;
	float gyro_bias_y;
	float gyro_bias_z;

	float accel_bias_x;
	float accel_bias_y;
	float accel_bias_z;

	bool imu_alive;

}imu_t;


/* GX3 "Datastore" */
imu_t drone_gx3;


#endif /* GX3_H_ */

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
 * gx3_msg.h
 *
 *  Created on: Nov 26, 2013
 *      Author: liam <liambeguin.at.gmail.com>
 *
 *
 *      Ref: http://files.microstrain.com/3DM-GX3-25%20Single%20Byte%20Data%20Communications%20Protocol.pdf
 *
 *      GX3 ENDIANESS : Big Endian
 */

#ifndef GX3_MSG_H_
#define GX3_MSG_H_

#define UNKNOWN_MESSAGE_ID (-1)

#define GX3_GET_NAME_SIZE													2
#define GX3_CAPTURE_GYRO_BIAS_SIZE 											5
#define GX3_WRITE_ACCEL_BIAS_SIZE											3
#define GX3_WRITE_GYRO_BIAS_SIZE 											3
#define GX3_EULER_ANGLES_AND_ANGULAR_RATES_REQUEST_SIZE 					1
#define GX3_ACCELERATIONS_AND_ANGULAR_RATES_REQUEST_SIZE 					1
#define GX3_ORIENTATION_MATRIX_REQUEST_SIZE 								1
#define GX3_ACCELERATION_ANGULAR_RATE_ORIENTATION_MATRIX_REQUEST_SIZE 		1
#define GX3_SCALED_MAGNETOMETER_VECTOR_REQUEST_SIZE 						1
#define GX3_EULER_ANGLES_REQUEST_SIZE 										1


// Capture Gyro Bias
static const uint8_t GX3_CAPTURE_GYRO_BIAS[] = {0xCD, 0xC1, 0x29, 0x27, 0x10};
typedef struct {
	float gyro_bias_x;
	float gyro_bias_y;
	float gyro_bias_z;
	uint32_t timer;
}gx3_capture_gyro_response_t;

// Write Accel Bias
static const uint8_t GX3_WRITE_ACCEL_BIAS[] = {0xC9, 0xB7, 0x44};
typedef struct {
	float accel_bias_x;
	float accel_bias_y;
	float accel_bias_z;
	uint32_t timer;
}gx3_write_accel_response_t;


// Write Gyro Bias
static const uint8_t GX3_WRITE_GYRO_BIAS[] = {0xCA, 0x12, 0xA5};
typedef struct {
	float gyro_bias_x;
	float gyro_bias_y;
	float gyro_bias_z;
	uint32_t timer;
}gx3_write_gyro_response_t;

// GX3 request and response data structure
static const uint8_t GX3_EULER_ANGLES_AND_ANGULAR_RATES_REQUEST[] = {0xCF};
typedef struct {
	float roll;
	float pitch;
	float yaw;
	float dot_x; // roll
	float dot_y; // pitch
	float dot_z; // yaw
	uint32_t timer;
}gx3_euler_angles_and_angular_rates_response_t ;

// GX3 request and response data structure
static const uint8_t GX3_ACCELERATIONS_AND_ANGULAR_RATES_REQUEST[] = {0xC2};
typedef struct {
	float accel_x;
	float accel_y;
	float accel_z;
	float dot_x; // vit roll
	float dot_y; // vit pitch
	float dot_z; // vit yaw
	uint32_t timer;
}gx3_accelerations_and_angular_rates_response_t;

static const uint8_t GX3_ORIENTATION_MATRIX_REQUEST[] = {0xC5};
typedef struct {
	float m11;
	float m12;
	float m13;
	float m21;
	float m22;
	float m23;
	float m31;
	float m32;
	float m33;
	uint32_t timer;
}gx3_orientation_matrix_response_t;

static const uint8_t GX3_ACCELERATION_ANGULAR_RATE_ORIENTATION_MATRIX_REQUEST[] = {0xC8};
typedef struct {
	float accel_x;
	float accel_y;
	float accel_z;
	float dot_x; // roll
	float dot_y; // pitch
	float dot_z; // yaw
	float m11;
	float m12;
	float m13;
	float m21;
	float m22;
	float m23;
	float m31;
	float m32;
	float m33;
	uint32_t timer;
}gx3_acceleration_angular_rate_orientation_matrix_response_t ;

static const uint8_t GX3_SCALED_MAGNETOMETER_VECTOR_REQUEST[] = {0xC7};
typedef struct {
	float mag_x;
	float mag_y;
	float mag_z;
	uint32_t timer;
}gx3_scaled_magnetometer_vector_t;

static const uint8_t GX3_EULER_ANGLES_REQUEST[] = {0xCE};
typedef struct {
	float roll;
	float pitch;
	float yaw;
	uint32_t timer;
}gx3_euler_angles_response_t;

static const uint8_t GX3_GET_NAME[] = {0xEA, 0x02};
typedef struct {
	uint8_t echo;
	char name[16];
}gx3_get_name_response_t ;

#endif /* GX3_MSG_H_ */

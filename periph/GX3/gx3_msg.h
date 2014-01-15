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
 */

#ifndef GX3_MSG_H_
#define GX3_MSG_H_

// Capture Gyro Bias
// Send message in this order !!!
static const uint8_t  GX3_CAPTURE_GYRO_BIAS	   = 0xCD;
static const uint8_t  GX3_CAPTURE_GYRO_BIAS_C1 = 0xC1;
static const uint8_t  GX3_CAPTURE_GYRO_BIAS_C2 = 0x29;
static const uint16_t GX3_CAPTURE_TIME_MS	   = 10000; //<------ Time to send for the calibration


struct gx3_capture_gyro_response {
	float gyro_bias_x;
	float gyro_bias_y;
	float gyro_bias_z;
	uint32_t timer;
};

// Write Accel Bias
static const uint8_t GX3_WRITE_ACCEL_BIAS	 = 0xC9;
static const uint8_t GX3_WRITE_ACCEL_BIAS_C1 = 0xB7;
static const uint8_t GX3_WRITE_ACCEL_BIAS_C2 = 0x44;

struct gx3_write_accel_response {
	float accel_bias_x;
	float accel_bias_y;
	float accel_bias_z;
	uint32_t timer;
};


// Write Gyro Bias
static const uint8_t GX3_WRITE_GYRO_BIAS	= 0xCA;
static const uint8_t GX3_WRITE_GYRO_BIAS_C1 = 0x12;
static const uint8_t GX3_WRITE_GYRO_BIAS_C2 = 0xA5;

struct gx3_write_gyro_response {
	float gyro_bias_x;
	float gyro_bias_y;
	float gyro_bias_z;
	uint32_t timer;
};




// GX3 request and response data structure
static const uint8_t GX3_EULER_ANGLES_AND_ANGULAR_RATES_REQUEST = 0xCF;
struct gx3_euler_angles_and_angular_rates_response {
	float roll;
	float pitch;
	float yaw;
	float dot_x; // roll
	float dot_y; // pitch
	float dot_z; // yaw
	uint32_t timer;
};
// GX3 request and response data structure
static const uint8_t GX3_ACCELERATIONS_AND_ANGULAR_RATES_REQUEST = 0xC2;
struct gx3_accelerations_and_angular_rates_response {
	float accel_x;
	float accel_y;
	float accel_z;
	float dot_x; // vit roll
	float dot_y; // vit pitch
	float dot_z; // vit yaw
	uint32_t timer;
};
static const uint8_t GX3_ORIENTATION_MATRIX_REQUEST = 0xC5;
struct gx3_orientation_matrix_response {
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
};
static const uint8_t GX3_ACCELERATION_ANGULAR_RATE_ORIENTATION_MATRIX_REQUEST = 0xC8;
struct gx3_acceleration_angular_rate_orientation_matrix_response {
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
};
static const uint8_t GX3_SCALED_MAGNETOMETER_VECTOR_REQUEST = 0xC7;
struct gx3_scaled_magnetometer_vector {
	float mag_x;
	float mag_y;
	float mag_z;
	uint32_t timer;
};
static const uint8_t GX3_EULER_ANGLES_REQUEST = 0xCE;
struct gx3_euler_angles_response {
	float roll;
	float pitch;
	float yaw;
	uint32_t timer;
};

#endif /* GX3_MSG_H_ */

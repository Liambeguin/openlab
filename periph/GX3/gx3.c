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
 * gx3.c
 *
 *  Created on: Nov 26, 2013
 *      Author: liam <liambeguin.at.gmail.com>
 */


#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "gx3.h"
#include "gx3_.h"
#include "gx3_msg.h"

#include "circular_buff.h"
#include "SCP/scp_parser.h"
#include "endianess.h"

#include "sender.h"


#define NO_DEBUG_HEADER
#include "debug.h"
#include "uart.h"
#include "handler.h"

#define GX3_TIME_CONVERT (0.000016f) //GX3 doc p.51 (1/62500)


uart_t _gx3_uart;

uint8_t gx3_alive_check;

//Monitoring data
uint32_t gx3_time_cmd_sent_us;

uint32_t gx3_ang_cmd_sent_count;
uint32_t gx3_ang_sum_time_reception_us;

uint32_t gx3_acc_cmd_sent_count;
uint32_t gx3_acc_sum_time_reception_us;

bool is_gyro_calibration_completed;
bool is_accel_calibration_sent;
bool is_accel_calibration_completed;
uint16_t accel_calibration_nb;

circular_buff_t rx_buffer;

void GX3_init(uart_t uart){

	_gx3_uart = uart;

	gx3_alive_check					= false;
	gx3_time_cmd_sent_us			= 0;
	gx3_ang_cmd_sent_count			= 0;
	gx3_ang_sum_time_reception_us	= 0;
	gx3_acc_cmd_sent_count			= 0;
	gx3_acc_sum_time_reception_us	= 0;
	is_gyro_calibration_completed	= false;
	is_accel_calibration_sent		= false;
	is_accel_calibration_completed	= false;
	accel_calibration_nb			= 0;

	SCP_init();

	//initializing reception circular buffer
	rx_buffer = cb_init();

	//Setting reception handler used to decode
	uart_set_rx_handler(_gx3_uart, GX3_rx_handler, NULL);



	log_info("[GX3] instance created \n\r");
}


void GX3_rx_handler(handler_arg_t arg, uint8_t c)
{
	cb_write(&rx_buffer, &c);
}

void GX3_decode_uart_rx(void)
{
	uint8_t data=-1;
	cb_read(&rx_buffer, &data);
	if(data!=-1){
//		log_warning("0x%02x", data);
		SCP_decode(data);
	}
}

void GX3_send_request(uint8_t const *request, uint8_t size)
{
	uart_transfer(_gx3_uart, request, size);
}


/*
 * rewrite in other class (generic) with switch case
 *
 * is_***_alive is a hack to speed up the protocol... => can be removed
 */
int8_t GX3_get_message_length(const uint8_t id)
{
	// payload size only
	if(id == GX3_EULER_ANGLES_AND_ANGULAR_RATES_REQUEST[0])
	{
		return sizeof(gx3_euler_angles_and_angular_rates_response_t);
	}
	else if(id == GX3_ACCELERATIONS_AND_ANGULAR_RATES_REQUEST[0])
	{
		return sizeof(gx3_accelerations_and_angular_rates_response_t);
	}
	else if(!is_gyro_calibration_completed && id == GX3_CAPTURE_GYRO_BIAS[0])
	{
		return sizeof(gx3_capture_gyro_response_t);
	}
	else if(!is_accel_calibration_completed && id == GX3_WRITE_ACCEL_BIAS[0])
	{
		return sizeof(gx3_write_accel_response_t);
	}
	else if(!is_accel_calibration_completed && id == GX3_GET_NAME[0])
	{
		return sizeof(gx3_get_name_response_t);
	}

	return UNKNOWN_MESSAGE_ID;
}

void GX3_process_complete_message(const uint8_t id)
{

	if (id == GX3_EULER_ANGLES_AND_ANGULAR_RATES_REQUEST[0])
	{
		gx3_euler_angles_and_angular_rates_response_t r;


		SCP_get(&r, sizeof(gx3_euler_angles_and_angular_rates_response_t));

		//Writing ROLL, PITCH, YAW, DOT_ROLL, DOT_PITCH, DOT_YAW to "datastore"
		bigendian2host(&(r.roll));	drone_gx3.roll	= r.roll;
		bigendian2host(&(r.pitch));	drone_gx3.pitch	= r.pitch;
		bigendian2host(&(r.yaw));	drone_gx3.yaw 	= r.yaw;

		bigendian2host(&(r.dot_x));	drone_gx3.dot_roll	= r.dot_x;
		bigendian2host(&(r.dot_y));	drone_gx3.dot_pitch	= r.dot_y;
		bigendian2host(&(r.dot_z));	drone_gx3.dot_yaw		= r.dot_z;

//		Software killswtich
		drone_gx3.imu_alive = true;
		gx3_alive_check = 5;

		//Monitoring
		gx3_ang_cmd_sent_count++;
		gx3_ang_sum_time_reception_us += (soft_timer_ticks_to_us(soft_timer_time()) - gx3_time_cmd_sent_us);

	}
	else if (id == GX3_ACCELERATIONS_AND_ANGULAR_RATES_REQUEST[0])
	{

		gx3_accelerations_and_angular_rates_response_t r;
		SCP_get(&r, sizeof(gx3_accelerations_and_angular_rates_response_t));

		if(is_accel_calibration_sent)
		{
			is_accel_calibration_sent = false;
			accel_calibration_nb++;

			// Writing ACCEL_BIAS_X, ACCEL_BIAS_Y, ACCEL_BIAS_Z to datastore
			bigendian2host(&(r.accel_x));	drone_gx3.accel_bias_x += r.accel_x;
			bigendian2host(&(r.accel_y));	drone_gx3.accel_bias_y += r.accel_y;
			bigendian2host(&(r.accel_z));	drone_gx3.accel_bias_z += r.accel_z + 1.0f ;

		}else{
			// Writing ACCEL_X, ACCEL_Y, ACCEL_Z to datastore
			bigendian2host(&(r.accel_x));	drone_gx3.accel_x = r.accel_x * PHYSICS_GX3;
			bigendian2host(&(r.accel_y));	drone_gx3.accel_y = r.accel_y * PHYSICS_GX3;
			bigendian2host(&(r.accel_z));	drone_gx3.accel_z = r.accel_z * PHYSICS_GX3;

		}

//		KillSwtich
		drone_gx3.imu_alive = true;
		gx3_alive_check = 5;

		//Monitoring
		gx3_acc_cmd_sent_count++;
		gx3_acc_sum_time_reception_us += (soft_timer_ticks_to_us(soft_timer_time()) - gx3_time_cmd_sent_us);

		return;
	}
	else if(id == GX3_CAPTURE_GYRO_BIAS[0])
	{
		gx3_capture_gyro_response_t r;
		SCP_get(&r, sizeof(gx3_capture_gyro_response_t));

		// Writing GYRO_BIAS_X, GYRO_BIAS_Y, GYRO_BIAS_Z to Datastore
		bigendian2host(&(r.gyro_bias_x));	drone_gx3.gyro_bias_x = r.gyro_bias_x;
		bigendian2host(&(r.gyro_bias_y));	drone_gx3.gyro_bias_y = r.gyro_bias_y;
		bigendian2host(&(r.gyro_bias_z));	drone_gx3.gyro_bias_z = r.gyro_bias_z;

		//GX3 confirm that calibration is completed
		is_gyro_calibration_completed = true;
	}

	else if(id == GX3_WRITE_ACCEL_BIAS[0])
	{
		gx3_write_accel_response_t r;
		SCP_get(&r, sizeof(gx3_write_accel_response_t));

		// Writing ACCEL_BIAS_X, ACCEL_BIAS_Y, ACCEL_BIAS_Z to Datastore
		bigendian2host(&(r.accel_bias_x));	drone_gx3.accel_bias_x = r.accel_bias_x;
		bigendian2host(&(r.accel_bias_y));	drone_gx3.accel_bias_y = r.accel_bias_y;
		bigendian2host(&(r.accel_bias_z));	drone_gx3.accel_bias_z = r.accel_bias_z;

		//GX3 confirm that calibration is completed
		is_accel_calibration_completed = true;
	}
	else if(id == GX3_GET_NAME[0])
	{
		gx3_get_name_response_t r;
		SCP_get(&r, sizeof(gx3_get_name_response_t));
		int i=0;
		for(i=0; i<sizeof(r.name); i++)
			printf("[%c]", r.name[i]);
		printf("\n");

	}

}

void GX3_periodical()
{
	if(gx3_alive_check)
		gx3_alive_check--;

	else
		drone_gx3.imu_alive = false;

	GX3_send_request(GX3_EULER_ANGLES_AND_ANGULAR_RATES_REQUEST,
			GX3_EULER_ANGLES_AND_ANGULAR_RATES_REQUEST_SIZE);

	GX3_send_request(GX3_ACCELERATIONS_AND_ANGULAR_RATES_REQUEST,
			GX3_ACCELERATIONS_AND_ANGULAR_RATES_REQUEST_SIZE);


	//Monitoring
	gx3_time_cmd_sent_us = soft_timer_ticks_to_us(soft_timer_time());
}


//commented in initial code
void GX3_write_bias()
{
	/*

	GX3_send_request(GX3_WRITE_GYRO_BIAS, GX3_WRITE_GYRO_BIAS_SIZE);

	SCP_send_float(0.0f);
	SCP_send_float(0.0f);
	SCP_send_float(0.0f);

	is_gyro_calibration_completed  = true;



	GX3_send_request(GX3_WRITE_ACCEL_BIAS, GX3_WRITE_ACCEL_BIAS_SIZE);
	SCP_send_float(100.0f);
	SCP_send_float(200.0f);
	SCP_send_float(17.0f);
*/

	//is_accel_calibration_sent	   = true;
	//is_accel_calibration_completed = true;
}

void GX3_calibrate()
{
/*
	log_info("[GX3] Gyro and Accel bias calibration ... Please don't move the drone for the next 20s ...");

	GX3_send_request(GX3_CAPTURE_GYRO_BIAS, GX3_CAPTURE_GYRO_BIAS_SIZE);

	log_debug("1");
	const uint32_t start = soft_timer_ticks_to_us(soft_timer_time());

	while(! is_gyro_calibration_completed)
	{
		//const uint32_t elapse = Singleton::get()->timer0.get_us_time() - start;
//		GX3_decode_uart_rx();
			soft_timer_delay_us(100);
		//imu_print_dbg("Wait... IMU gyro bias calibration for 10000 : " << elapse << "\n\r");
		//log_infoln(elapse);
	}
	log_debug("2");

	log_info("[GX3] Gyro calibrated with :");
	log_info("[GX3] GYRO_BIAS_X = %f", gx3.gyro_bias_x);

	log_info("[GX3] GYRO_BIAS_Y = %f", gx3.gyro_bias_y);

	log_info("[GX3] GYRO_BIAS_Z = %f", gx3.gyro_bias_z);

	log_info("[GX3] Measuring acceleration for 10s ...");

	//Execute Accel calibration
	const uint32_t end = (soft_timer_ticks_to_us(soft_timer_time()) + 10000000); //Run for 10s

	while(soft_timer_ticks_to_us(soft_timer_time()) < end)
	{
		if(!is_accel_calibration_sent)
		{
			is_accel_calibration_sent = true;
			GX3_send_request(GX3_ACCELERATIONS_AND_ANGULAR_RATES_REQUEST, GX3_ACCELERATIONS_AND_ANGULAR_RATES_REQUEST_SIZE);
		}

		GX3_decode_uart_rx();
		soft_timer_delay_us(100);
	}

	float accel_bias_x = 0;
	float accel_bias_y = 0;
	float accel_bias_z = 0;

	if(accel_calibration_nb != 0)
	{
		accel_bias_x = gx3.accel_bias_x / (float)(accel_calibration_nb);
		accel_bias_y = gx3.accel_bias_y / (float)(accel_calibration_nb);
		accel_bias_z = gx3.accel_bias_z / (float)(accel_calibration_nb);
	}

	GX3_send_request(GX3_WRITE_ACCEL_BIAS, GX3_WRITE_ACCEL_BIAS_SIZE);
//	SCP_send_float(accel_bias_x);
//	SCP_send_float(accel_bias_y);
//	SCP_send_float(accel_bias_z);

	while(! is_accel_calibration_completed)
	{
		GX3_decode_uart_rx();
		soft_timer_delay_us(100);
	}

	log_info("[GX3] Accel calibrated with : ");
	log_info("[GX3] ACCEL_BIAS_X = %f", gx3.accel_bias_x);

	log_info("[GX3] ACCEL_BIAS_Y = %f", gx3.accel_bias_y);

	log_info("[GX3] ACCEL_BIAS_Z = %f", gx3.accel_bias_z);

	//Finish calibration
	is_gyro_calibration_completed  = true;
	is_accel_calibration_sent	   = true;
	is_accel_calibration_completed = true;

	log_info("[GX3] Calibration done");

*/
}

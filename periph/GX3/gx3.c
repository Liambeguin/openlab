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

#include "gx3.h"
#include "gx3_.h"
#include "gx3_msg.h"

#include "circular_buff.h"
#include "SCP/scp_parser.h"

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

void GX3_decode_uart_rx()
{
	uint8_t data=-1;
	cb_read(&rx_buffer, &data);
	if(data!=-1){
		SCP_decode(data);
//		log_error("0x%02x", data);
	}
}

void GX3_send_request(uint8_t *request, uint8_t size)
{
	uart_transfer(_gx3_uart, request, size);
}

int8_t GX3_get_message_length(const uint8_t id)
{
	// payload size only
	if(id == GX3_EULER_ANGLES_AND_ANGULAR_RATES_REQUEST[0])
	{
		return sizeof(struct gx3_euler_angles_and_angular_rates_response);
	}
	else if(id == GX3_ACCELERATIONS_AND_ANGULAR_RATES_REQUEST[0])
	{
		return sizeof(struct gx3_accelerations_and_angular_rates_response);
	}
	else if(!is_gyro_calibration_completed && id == GX3_CAPTURE_GYRO_BIAS[0])
	{
		return sizeof(struct gx3_capture_gyro_response);
	}
	else if(!is_accel_calibration_completed && id == GX3_WRITE_ACCEL_BIAS[0])
	{
		return sizeof(struct gx3_write_accel_response);
	}

	return UNKNOWN_MESSAGE_ID;
}

void GX3_process_complete_message(const uint8_t id)
{
	log_error("blavla");
	if (id == GX3_EULER_ANGLES_AND_ANGULAR_RATES_REQUEST[0])
	{
		struct gx3_euler_angles_and_angular_rates_response r;
		memcpy(&r, SCP_get_payload(), sizeof(struct gx3_euler_angles_and_angular_rates_response));
		log_not_implemented("%f", r.roll);

		//TODO Write ROLL, PITCH, YAW, DOT_ROLL, DOT_PITCH, DOT_YAW to datastore

//		Software killswtich
		//TODO Write IMU_ALIVE = 1 to datastore
		gx3_alive_check = 5;

		//Monitoring
		gx3_ang_cmd_sent_count++;
		gx3_ang_sum_time_reception_us += (soft_timer_ticks_to_us(soft_timer_time()) - gx3_time_cmd_sent_us);

	}
	else if (id == GX3_ACCELERATIONS_AND_ANGULAR_RATES_REQUEST[0])
	{

		struct gx3_accelerations_and_angular_rates_response r;
		memcpy(&r, SCP_get_payload(), sizeof(struct gx3_accelerations_and_angular_rates_response));

		if(is_accel_calibration_sent)
		{
			is_accel_calibration_sent = false;
			accel_calibration_nb++;
			//TODO write ACCEL_BIAS_X, ACCEL_BIAS_Y, ACCEL_BIAS_Z to datastore
		}else{
			//TODO write ACCEL_X, ACCEL_Y, ACCEL_Z to datastore
		}


//		KillSwtich
		//TODO Write IMU_ALIVE = 1 to datastore
		gx3_alive_check = 5;

		//Monitoring
		gx3_acc_cmd_sent_count++;
		gx3_acc_sum_time_reception_us += (soft_timer_ticks_to_us(soft_timer_time()) - gx3_time_cmd_sent_us);

		return;
	}
	else if(id == GX3_CAPTURE_GYRO_BIAS[0])
	{
		struct gx3_capture_gyro_response r;
		memcpy(&r, SCP_get_payload(), sizeof(struct gx3_capture_gyro_response));

		//TODO Write GYRO_BIAS_X, GYRO_BIAS_Y, GYRO_BIAS_Z to Datastore

		//GX3 confirm that calibration is completed
		is_gyro_calibration_completed = true;
	}

	else if(id == GX3_WRITE_ACCEL_BIAS[0])
	{
		struct gx3_write_accel_response r;
		memcpy(&r, SCP_get_payload(), sizeof(struct gx3_write_accel_response));

		//TODO Write ACCEL_BIAS_X, ACCEL_BIAS_Y, ACCEL_BIAS_Z to Datastore

		//GX3 confirm that calibration is completed
		is_accel_calibration_completed = true;
	}

}

void GX3_periodical()
{
	if(gx3_alive_check)
		gx3_alive_check--;
	else{
//		TODO write IMU_ALIVE = 0 to datastore
	}

	uart_transfer(_gx3_uart, GX3_EULER_ANGLES_AND_ANGULAR_RATES_REQUEST,
			sizeof(GX3_EULER_ANGLES_AND_ANGULAR_RATES_REQUEST));
	uart_transfer(_gx3_uart, GX3_ACCELERATIONS_AND_ANGULAR_RATES_REQUEST,
			sizeof(GX3_ACCELERATIONS_AND_ANGULAR_RATES_REQUEST));

//	log_not_implemented("%f", gx3_euler_angles_and_angular_rates_response);


//
//	//Monitoring
//	gx3_time_cmd_sent_us = soft_timer_ticks_to_us(soft_timer_time());
}


void GX3_write_bias(){
	/*
	sender <<  (char) GX3_WRITE_GYRO_BIAS;
	sender <<  (char) GX3_WRITE_GYRO_BIAS_C1;
	sender <<  (char) GX3_WRITE_GYRO_BIAS_C2;
	sender <<  0.0f;
	sender <<  0.0f;
	sender <<  0.0f;
	is_gyro_calibration_completed  = true;



	sender <<  (char) GX3_WRITE_ACCEL_BIAS;
	sender <<  (char) GX3_WRITE_ACCEL_BIAS_C1;
	sender <<  (char) GX3_WRITE_ACCEL_BIAS_C2;
	sender <<  100.0f;
	sender <<  200.0f;
	sender <<  17.0f;
	*/

	//is_accel_calibration_sent	   = true;
	//is_accel_calibration_completed = true;
}

void GX3_calibrate()
{

	log_info("[GX3] Gyro and Accel bias calibration ... Please don't move the drone for the next 20s ... \n\r");

	GX3_send_request(GX3_CAPTURE_GYRO_BIAS, GX3_CAPTURE_GYRO_BIAS_SIZE);

	const uint32_t start = soft_timer_ticks_to_us(soft_timer_time());

	while(! is_gyro_calibration_completed)
	{
		//const uint32_t elapse = Singleton::get()->timer0.get_us_time() - start;
		decode_uart_rx();
		soft_timer_delay_us(100);
		//imu_print_dbg("Wait... IMU gyro bias calibration for 10000 : " << elapse << "\n\r");
		//log_infoln(elapse);
	}

	log_info("[GX3] Gyro calibrated with : \n\r");
	log_info("[GX3] GYRO_BIAS_X = ");
//	log_info(Singleton::get()->data.get(DataStore::GYRO_BIAS_X));
	log_info("\n\r");

	log_info("[GX3] GYRO_BIAS_Y = ");
//	log_info(Singleton::get()->data.get(DataStore::GYRO_BIAS_Y));
	log_info("\n\r");

	log_info("[GX3] GYRO_BIAS_Z = ");
//	log_info(Singleton::get()->data.get(DataStore::GYRO_BIAS_Z));
	log_info("\n\r");


	log_info("[GX3] Measuring acceleration for 10s ... \n\r");

	//Execute Accel calibration
	const uint32_t end = (soft_timer_ticks_to_us(soft_timer_time()) + 10000000); //Run for 10s

	while(soft_timer_ticks_to_us(soft_timer_time()) < end)
	{
		if(!is_accel_calibration_sent)
		{
			is_accel_calibration_sent = true;

			uart_transfer(_gx3_uart, (char)GX3_ACCELERATIONS_AND_ANGULAR_RATES_REQUEST,
					sizeof( (char)GX3_ACCELERATIONS_AND_ANGULAR_RATES_REQUEST));
		}

		decode_uart_rx();
		soft_timer_delay_us(100);
	}

	float accel_bias_x = 0;
	float accel_bias_y = 0;
	float accel_bias_z = 0;

	if(accel_calibration_nb != 0)
	{
//		accel_bias_x = Singleton::get()->data.get(DataStore::ACCEL_BIAS_X) / (float)(accel_calibration_nb);
//		accel_bias_y = Singleton::get()->data.get(DataStore::ACCEL_BIAS_Y) / (float)(accel_calibration_nb);
//		accel_bias_z = Singleton::get()->data.get(DataStore::ACCEL_BIAS_Z) / (float)(accel_calibration_nb);
	}

//	sender <<  (char) GX3_WRITE_ACCEL_BIAS;
//	sender <<  (char) GX3_WRITE_ACCEL_BIAS_C1;
//	sender <<  (char) GX3_WRITE_ACCEL_BIAS_C2;
//	sender <<  accel_bias_x;
//	sender <<  accel_bias_y;
//	sender <<  accel_bias_z;

	while(! is_accel_calibration_completed)
	{
		decode_uart_rx();
				soft_timer_delay_us(100);
	}

	log_info("[GX3] Accel calibrated with : \n\r");
	log_info("[GX3] ACCEL_BIAS_X = ");
//	log_info(Singleton::get()->data.get(DataStore::ACCEL_BIAS_X));
	log_info("\n\r");

	log_info("[GX3] ACCEL_BIAS_Y = ");
//	log_info(Singleton::get()->data.get(DataStore::ACCEL_BIAS_Y));
	log_info("\n\r");

	log_info("[GX3] ACCEL_BIAS_Z = ");
//	log_info(Singleton::get()->data.get(DataStore::ACCEL_BIAS_Z));
	log_info("\n\r");

	//Finish calibration
	is_gyro_calibration_completed  = true;
	is_accel_calibration_sent	   = true;
	is_accel_calibration_completed = true;

	log_info("[GX3] Calibration done \n\r");

}

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


#include "gx3.h"


#include "gx3_msg.h"
#include <util/endianess/endianess.h>

#define GX3_TIME_CONVERT (0.000016f) //GX3 doc p.51 (1/62500)

#if DEBUG_FLAG_DRIVER_IMU
#define imu_print_dbg(x) (Singleton::get()->console << (x))
#define imu_print_dbg_hex(x) (Singleton::get()->console << _HEX(x))
#else
#define imu_print_dbg(x)
#define imu_print_dbg_hex(x)
#endif

//
//GX3::GX3(Uart * uart) ://
//	Task(Singleton::get()->parameters.get(Parameters::DT) * 1000.0f /* ms */)
//	 , FFPParser((cksumfn) gx3sum)
//	 , uart(uart)
//	 , sender(uart)
//	 , imu_alive_check(false)
//	 , imu_time_cmd_sent_us(0)
//	 , imu_ang_cmd_sent_count(0)
//	 , imu_ang_sum_time_reception_us(0)
//	 , imu_acc_cmd_sent_count(0)
//	 , imu_acc_sum_time_reception_us(0)
//	 , is_gyro_calibration_completed(false)
//	 , is_accel_calibration_sent(false)
//	 , is_accel_calibration_completed(false)
//	 , accel_calibration_nb(0)
//{
//	print_dbg("\t[Imu] instance created \n\r");
//}
//

int8_t GX3_get_message_length(const uint8_t id)
{
	// payload size only
	if(id == GX3_EULER_ANGLES_AND_ANGULAR_RATES_REQUEST)
	{
		return sizeof(gx3_euler_angles_and_angular_rates_response);
	}
	else if(id == GX3_ACCELERATIONS_AND_ANGULAR_RATES_REQUEST)
	{
		return sizeof(gx3_accelerations_and_angular_rates_response);
	}
	else if(!is_gyro_calibration_completed && id == GX3_CAPTURE_GYRO_BIAS)
	{
		return sizeof(gx3_capture_gyro_response);
	}
	else if(!is_accel_calibration_completed && id == GX3_WRITE_ACCEL_BIAS)
	{
		return sizeof(gx3_write_accel_response);
	}

	return UNKNOWN_MESSAGE_ID;
}


void GX3_process_complete_message(const uint8_t id)
{
	if (id == GX3_EULER_ANGLES_AND_ANGULAR_RATES_REQUEST)
	{
		struct gx3_euler_angles_and_angular_rates_response r;
		memcpy(&r, get_payload(), sizeof(gx3_euler_angles_and_angular_rates_response));
//
		Singleton::get()->data.set(DataStore::ROLL,					bigendian2host(r.roll));
		Singleton::get()->data.set(DataStore::PITCH,				bigendian2host(r.pitch));
		Singleton::get()->data.set(DataStore::YAW,					bigendian2host(r.yaw));
		Singleton::get()->data.set(DataStore::DOT_ROLL,				bigendian2host(r.dot_x));
		Singleton::get()->data.set(DataStore::DOT_PITCH,			bigendian2host(r.dot_y));
		Singleton::get()->data.set(DataStore::DOT_YAW,				bigendian2host(r.dot_z));

		//Software killswtich
		Singleton::get()->data.set(DataStore::IMU_ALIVE, 1);
		imu_alive_check = 5;

		//Monitoring
		imu_ang_cmd_sent_count++;
		imu_ang_sum_time_reception_us += (Singleton::get()->timer0.get_us_time() - imu_time_cmd_sent_us);

	}
	else if (id == GX3_ACCELERATIONS_AND_ANGULAR_RATES_REQUEST)
	{

		struct gx3_accelerations_and_angular_rates_response r;
		memcpy(&r, get_payload(), sizeof(gx3_accelerations_and_angular_rates_response));

		if(is_accel_calibration_sent)
		{
			is_accel_calibration_sent = false;
			accel_calibration_nb++;

			const float accel_bias_x = Singleton::get()->data.get(DataStore::ACCEL_BIAS_X) + bigendian2host(r.accel_x);
			const float accel_bias_y = Singleton::get()->data.get(DataStore::ACCEL_BIAS_Y) + bigendian2host(r.accel_y);
			const float accel_bias_z = Singleton::get()->data.get(DataStore::ACCEL_BIAS_Z) + (bigendian2host(r.accel_z) + 1.0f);

			Singleton::get()->data.set(DataStore::ACCEL_BIAS_X , accel_bias_x);
			Singleton::get()->data.set(DataStore::ACCEL_BIAS_Y , accel_bias_y);
			Singleton::get()->data.set(DataStore::ACCEL_BIAS_Z , accel_bias_z);

		}
		else
		{
			Singleton::get()->data.set(DataStore::ACCEL_X,		  bigendian2host(r.accel_x) * PHYSICS_GX3);
			Singleton::get()->data.set(DataStore::ACCEL_Y,		  bigendian2host(r.accel_y) * PHYSICS_GX3);
			Singleton::get()->data.set(DataStore::ACCEL_Z,		  bigendian2host(r.accel_z) * PHYSICS_GX3);
		}


		//KillSwtich
		Singleton::get()->data.set(DataStore::IMU_ALIVE, 1);
		imu_alive_check = 5;

		//Monitoring
		imu_acc_cmd_sent_count++;
		imu_acc_sum_time_reception_us += (Singleton::get()->timer0.get_us_time() - imu_time_cmd_sent_us);

		return;
	}
	else if(id == GX3_CAPTURE_GYRO_BIAS)
	{
		struct gx3_capture_gyro_response r;
		memcpy(&r, get_payload(), sizeof(gx3_capture_gyro_response));

		Singleton::get()->data.set(DataStore::GYRO_BIAS_X , bigendian2host(r.gyro_bias_x));
		Singleton::get()->data.set(DataStore::GYRO_BIAS_Y,	bigendian2host(r.gyro_bias_y));
		Singleton::get()->data.set(DataStore::GYRO_BIAS_Z ,	bigendian2host(r.gyro_bias_z));

		//GX3 confirm that calibration is completed
		is_gyro_calibration_completed = true;
	}
	else if(id == GX3_WRITE_ACCEL_BIAS)
	{
		struct gx3_write_accel_response r;
		memcpy(&r, get_payload(), sizeof(gx3_write_accel_response));

		Singleton::get()->data.set(DataStore::ACCEL_BIAS_X , bigendian2host(r.accel_bias_x));
		Singleton::get()->data.set(DataStore::ACCEL_BIAS_Y,	 bigendian2host(r.accel_bias_y));
		Singleton::get()->data.set(DataStore::ACCEL_BIAS_Z , bigendian2host(r.accel_bias_z));

		//GX3 confirm that calibration is completed
		is_accel_calibration_completed = true;
	}

}

//void GX3::periodical()
//{
//	if(imu_alive_check)
//		imu_alive_check--;
//	else
//		Singleton::get()->data.set(DataStore::IMU_ALIVE, 0);
//
//	*uart << (char) GX3_EULER_ANGLES_AND_ANGULAR_RATES_REQUEST;
//    *uart << (char) GX3_ACCELERATIONS_AND_ANGULAR_RATES_REQUEST;
//
//	//Monitoring
//	imu_time_cmd_sent_us = Singleton::get()->timer0.get_us_time();
//}

void GX3_decode_uart_rx()
{
	int16_t c = 0;
	while (1)
	{
		*uart >> c;

		if(c == EOS) {
			return;
		}

		//Monitoring
		uint32_t begin_us = Singleton::get()->timer0.get_us_time();

		FFPParser::decode(c);

		//Monitoring
		sum_time_receiving_us += Singleton::get()->timer0.get_us_time() - begin_us;
	}

}

void GX3_write_bias()
{
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

	Singleton::get()->console.print("[IMU] Gyro and Accel bias calibration ... Please don't move the drone for the next 20s ... \n\r");

	//Execute Gyro calibration
	sender <<  (char) GX3_CAPTURE_GYRO_BIAS;
	sender <<  (char) GX3_CAPTURE_GYRO_BIAS_C1;
	sender <<  (char) GX3_CAPTURE_GYRO_BIAS_C2;
	sender <<   GX3_CAPTURE_TIME_MS;

	const uint32_t start = Singleton::get()->timer0.get_us_time();

	while(! is_gyro_calibration_completed)
	{
		//const uint32_t elapse = Singleton::get()->timer0.get_us_time() - start;
		decode_uart_rx();
		delay_us(100);
		//imu_print_dbg("Wait... IMU gyro bias calibration for 10000 : " << elapse << "\n\r");
		//Singleton::get()->console.println(elapse);
	}

	Singleton::get()->console.print("[IMU] Gyro calibrated with : \n\r");
	Singleton::get()->console.print("[IMU] GYRO_BIAS_X = ");
	Singleton::get()->console.print(Singleton::get()->data.get(DataStore::GYRO_BIAS_X));
	Singleton::get()->console.print("\n\r");

	Singleton::get()->console.print("[IMU] GYRO_BIAS_Y = ");
	Singleton::get()->console.print(Singleton::get()->data.get(DataStore::GYRO_BIAS_Y));
	Singleton::get()->console.print("\n\r");

	Singleton::get()->console.print("[IMU] GYRO_BIAS_Z = ");
	Singleton::get()->console.print(Singleton::get()->data.get(DataStore::GYRO_BIAS_Z));
	Singleton::get()->console.print("\n\r");


	Singleton::get()->console.print("[IMU] Measuring acceleration for 10s ... \n\r");

	//Execute Accel calibration
	const uint32_t end = Singleton::get()->timer0.get_us_time() + 10000000; //Run for 10s

	while(Singleton::get()->timer0.get_us_time() < end)
	{
		if(!is_accel_calibration_sent)
		{
			is_accel_calibration_sent = true;
			*uart << (char) GX3_ACCELERATIONS_AND_ANGULAR_RATES_REQUEST;
		}

		decode_uart_rx();
		delay_us(100);
		//imu_print_dbg("Wait... IMU accel bias calibration run for 10s, nb of it : " << accel_calibration_nb << "\n\r");
		//Singleton::get()->console.print(end);
		//Singleton::get()->console.print(" > ");
		//Singleton::get()->console.print(Singleton::get()->timer0.get_us_time());
		//Singleton::get()->console.print("\n\r");
	}

	float accel_bias_x = 0;
	float accel_bias_y = 0;
	float accel_bias_z = 0;

	if(accel_calibration_nb != 0)
	{
		accel_bias_x = Singleton::get()->data.get(DataStore::ACCEL_BIAS_X) / (float)(accel_calibration_nb);
		accel_bias_y = Singleton::get()->data.get(DataStore::ACCEL_BIAS_Y) / (float)(accel_calibration_nb);
		accel_bias_z = Singleton::get()->data.get(DataStore::ACCEL_BIAS_Z) / (float)(accel_calibration_nb);
	}

	sender <<  (char) GX3_WRITE_ACCEL_BIAS;
	sender <<  (char) GX3_WRITE_ACCEL_BIAS_C1;
	sender <<  (char) GX3_WRITE_ACCEL_BIAS_C2;
	sender <<  accel_bias_x;
	sender <<  accel_bias_y;
	sender <<  accel_bias_z;

	while(! is_accel_calibration_completed)
	{
		decode_uart_rx();
		delay_us(100);
	}

	Singleton::get()->console.print("[IMU] Accel calibrated with : \n\r");
	Singleton::get()->console.print("[IMU] ACCEL_BIAS_X = ");
	Singleton::get()->console.print(Singleton::get()->data.get(DataStore::ACCEL_BIAS_X));
	Singleton::get()->console.print("\n\r");

	Singleton::get()->console.print("[IMU] ACCEL_BIAS_Y = ");
	Singleton::get()->console.print(Singleton::get()->data.get(DataStore::ACCEL_BIAS_Y));
	Singleton::get()->console.print("\n\r");

	Singleton::get()->console.print("[IMU] ACCEL_BIAS_Z = ");
	Singleton::get()->console.print(Singleton::get()->data.get(DataStore::ACCEL_BIAS_Z));
	Singleton::get()->console.print("\n\r");

	//Finish calibration
	is_gyro_calibration_completed  = true;
	is_accel_calibration_sent	   = true;
	is_accel_calibration_completed = true;

	Singleton::get()->console.print("[IMU] Calibration done \n\r");


	Singleton::get()->data.set(DataStore::DEBUG_0 , Singleton::get()->data.get(DataStore::ACCEL_BIAS_X));
	Singleton::get()->data.set(DataStore::DEBUG_1 , Singleton::get()->data.get(DataStore::ACCEL_BIAS_Y));
	Singleton::get()->data.set(DataStore::DEBUG_2 , Singleton::get()->data.get(DataStore::ACCEL_BIAS_Z));
	Singleton::get()->data.set(DataStore::DEBUG_3 , Singleton::get()->data.get(DataStore::GYRO_BIAS_X));
	Singleton::get()->data.set(DataStore::DEBUG_4 , Singleton::get()->data.get(DataStore::GYRO_BIAS_Y));
	Singleton::get()->data.set(DataStore::M1      , Singleton::get()->data.get(DataStore::GYRO_BIAS_Z));


}

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
 * motors.c
 *
 *  Created on: 2013-10-28
 *      Author: liam <liambeguin.at.gmail.com>
 */

/**
 * TODO continue implementing
 * motors_datastore defined in motors.h
 *
 * functions called fill motors_datastore then
 * call ppm_update to actualize the value
 *
 **/

#include <math.h>


#include "platform.h"
#include "motors.h"

#define NO_DEBUG_HEADER
#include "debug.h"

#include "gpio.h"
#include "timer.h"

//Global declaration corresponding to the drone's motors
static _motor_t _motors[4];

uint16_t _value_0, _value_100, _span;


void motors_config(_motor_t *motors){

	log_info("[MOTORS] initializing motors ...");


	/** Setting up constants linked to the motors **/
	drone_motors.MOTOR_SPEED_MAX	= 750.00f;
	drone_motors.IDLE_POURC_ESC		=   0.06f;

	drone_motors.PHYSICS_B	= 0.00001418713138124377f;
	drone_motors.PHYSICS_D	= 0.00000184f;

	drone_motors.U1_MAX	 	= (4.0f * drone_motors.MOTOR_SPEED_MAX * drone_motors.MOTOR_SPEED_MAX * drone_motors.PHYSICS_B);
	drone_motors.BIAS_U2	= 0.0f;
	drone_motors.BIAS_U3	= 0.0f;
	drone_motors.BIAS_U4	= 0.0f;
	drone_motors.BIAS_ROLL	= 0.0f;
	drone_motors.BIAS_PITCH	= 0.0f;
	/** 			*** 						**/

	uint16_t timer_arr;

	uint8_t i;
	for(i=0; i<sizeof(_motors)/sizeof(_motors[0]); i++)
		_motors[i] = motors[i];

	// Configure the General Purpose Timers
	timer_enable(motors[0].timer);

	//setting timer limits only doing it once because it's the same timer for all motors
	timer_arr 	= (2.5/1000)*timer_get_frequency(motors[0].timer);
	_value_0 	= (1.0/1000)*timer_get_frequency(motors[0].timer);
	_value_100 	= (2.0/1000)*timer_get_frequency(motors[0].timer);
	_span 		= _value_100 - _value_0;

	log_info("[MOTORS] timer_frequency : %d Hz\t end count : 0x%04x", timer_get_frequency(motors[0].timer), timer_arr);
	log_info("\t 0   %% : 0x%04x \n\t 100 %% : 0x%04x \n\t Span  : %d values", _value_0, _value_100, _value_100-_value_0);



	/**************************
	 *  ALL MOTORS SHOULD BE ON SAME TIMER
	 *  Starting timer at the frequency set in <platform>_drivers.c
	 **************************/
	timer_start(motors[0].timer, timer_arr, NULL, NULL, TIMER_MODE_CLOCK);

	for(i=0; i<sizeof(_motors)/sizeof(_motors[0]); i++){
		timer_set_channel_compare(motors[i].timer, motors[i].channel, 0, NULL, NULL);
		timer_activate_channel_output(motors[i].timer, motors[i].channel, TIMER_OUTPUT_MODE_PWM1);
	}
	log_info("[MOTORS] initialized !!");
}

void ppm_update(_motor_t motor, float ratio){

	if (ratio < 0.0f || ratio > 1.0f){
		log_error("PPM Value out of range");
		return;
	}

	timer_update_channel_compare(motor.timer, motor.channel, (uint16_t)((float)_span * ratio + (float)_value_0));
}


void motors_ratio(float ratio_0, float ratio_1, float ratio_2, float ratio_3){

	ppm_update(_motors[0], ratio_0);
	ppm_update(_motors[1], ratio_1);
	ppm_update(_motors[2], ratio_2);
	ppm_update(_motors[3], ratio_3);

}



/**
 * Fonction obtain with measure from Guigz, feb. 2013
 *
 * Tests results are at : /home/MÉCANIQUE/WIP/MOTOR BENCH TEST/Test des moteurs/PPM/Config -
 * drone 4_1 (big MAMA) Turnigy aerodrive - Maytech ESC / Mesure vitesse de moteur Dronolab V4 - 14 fevrier 2013
 */
float motors_rad_to_percent(float v) {

	if(v > drone_motors.MOTOR_SPEED_MAX)
		return 0.85f; //This is the maximum ...
	else if(v <= 0)
		return 0.0f;
	else
		return  0.01 * ( 0.00011424106 * (v*v) + 0.02670861842 * v ); // equation return in %
}

void motors_speeds(const float M1, const float M2, const float M3, const float M4){

	//Register motor speed in rad/s
	drone_motors.M1 = M1;
	drone_motors.M2 = M2;
	drone_motors.M3 = M3;
	drone_motors.M4 = M4;


	//Transform in ppm duty, if speed is to low, idle motor !!!!
	const float V1 = ( M1 > 5 ? motors_rad_to_percent(M1) : drone_motors.IDLE_POURC_ESC);
	const float V2 = ( M2 > 5 ? motors_rad_to_percent(M2) : drone_motors.IDLE_POURC_ESC);
	const float V3 = ( M3 > 5 ? motors_rad_to_percent(M3) : drone_motors.IDLE_POURC_ESC);
	const float V4 = ( M4 > 5 ? motors_rad_to_percent(M4) : drone_motors.IDLE_POURC_ESC);


	ppm_update(_motors[0], V1);
	ppm_update(_motors[1], V2);
	ppm_update(_motors[2], V3);
	ppm_update(_motors[3], V4);


}

void motors_idle(){

	ppm_update(_motors[0], drone_motors.IDLE_POURC_ESC);
	ppm_update(_motors[1], drone_motors.IDLE_POURC_ESC);
	ppm_update(_motors[2], drone_motors.IDLE_POURC_ESC);
	ppm_update(_motors[3], drone_motors.IDLE_POURC_ESC);
}

void motors_kill(){

//	log_info("[MOTORS] killing motors...");

	ppm_update(_motors[0], 0.0f);
	ppm_update(_motors[1], 0.0f);
	ppm_update(_motors[2], 0.0f);
	ppm_update(_motors[3], 0.0f);

//	log_info("[MOTORS] motors killed !!");
}

void motors_update(float U1, float U2, float U3, float U4) {

	// Puissance faible, il ne faut pas que les motors arretent
	if (U1 < 0.001) {
		motors_idle();
		return;
	}

	// Correction du biais
	U2 -= drone_motors.BIAS_U2;
	U3 -= drone_motors.BIAS_U3;
	U4 -= drone_motors.BIAS_U4;

	// Transformations U_i => Omega_i (1/3)

	U1 /= drone_motors.PHYSICS_B;
	U2 /= drone_motors.PHYSICS_B;
	U3 /= drone_motors.PHYSICS_B;

	U4 /= drone_motors.PHYSICS_D;

	// Transformations U_i => Omega_i (2/3)
	float O1 = (U1 + U4 + 2 * U3) / 4;
	float O2 = (U1 - U4 - 2 * U2) / 4;
	float O3 = (U1 + U4 - 2 * U3) / 4;
	float O4 = (U1 - U4 + 2 * U2) / 4;

	// Transformations U_i => Omega_i (3/3)
	O1 = O1 > 0 ? sqrt(O1) : 0;
	O2 = O2 > 0 ? sqrt(O2) : 0;
	O3 = O3 > 0 ? sqrt(O3) : 0;
	O4 = O4 > 0 ? sqrt(O4) : 0;

	// Enregistrement
	drone_motors.OMEGA_R = (O1 + O3 - O2 - O4);


	// Envoi de la commande
	motors_speeds(O1, O2, O3, O4);

}

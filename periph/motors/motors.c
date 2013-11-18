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
static motor_t _motors[4];

uint16_t _value_0, _value_100, _span;


void motors_config(motor_t *motors){

	log_info("[MOTORS] initializing motors ...");
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
	timer_start(motors[0].timer, timer_arr, NULL, NULL);

	for(i=0; i<sizeof(_motors)/sizeof(_motors[0]); i++){
		timer_set_channel_compare(motors[i].timer, motors[i].channel, 0, NULL, NULL);
		timer_activate_channel_output(motors[i].timer, motors[i].channel, TIMER_OUTPUT_MODE_PWM1);
	}
	log_info("[MOTORS] initialized !!");
}

void ppm_update(motor_t motor, float ratio){

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

        if(v > MOTOR_SPEED_MAX)
                return 0.85f; //This is the maximum ...
        else if(v <= 0)
                return 0.0f;
        else
                return  0.01 * ( 0.00011424106 * (v*v) + 0.02670861842 * v ); // equation return in %
}

void motors_speeds(const float M0, const float M1, const float M2, const float M3){

        //Register motor speed in rad/s
//        Singleton::get()->data.set(DataStore::M1, M1);
//        Singleton::get()->data.set(DataStore::M2, M2);
//        Singleton::get()->data.set(DataStore::M3, M3);
//        Singleton::get()->data.set(DataStore::M4, M4);

		motors_datastore.M0 = M0;
		motors_datastore.M1 = M1;
		motors_datastore.M2 = M2;
		motors_datastore.M3 = M3;


        //Transform in ppm duty, if speed is to low, idle motor !!!!
        const float V0 = ( M0 > 5 ? motors_rad_to_percent(M0) : IDLE_POURC_ESC);
        const float V1 = ( M1 > 5 ? motors_rad_to_percent(M1) : IDLE_POURC_ESC);
        const float V2 = ( M2 > 5 ? motors_rad_to_percent(M2) : IDLE_POURC_ESC);
        const float V3 = ( M3 > 5 ? motors_rad_to_percent(M3) : IDLE_POURC_ESC);


        ppm_update(_motors[0], V0);
        ppm_update(_motors[1], V1);
        ppm_update(_motors[2], V2);
        ppm_update(_motors[3], V3);

}

void motors_idle(){

        ppm_update(_motors[0], IDLE_POURC_ESC);
        ppm_update(_motors[1], IDLE_POURC_ESC);
        ppm_update(_motors[2], IDLE_POURC_ESC);
        ppm_update(_motors[3], IDLE_POURC_ESC);
}

void motors_kill(){

	log_info("[MOTORS] killing motors...");

	ppm_update(_motors[0], 0.0f);
    ppm_update(_motors[1], 0.0f);
    ppm_update(_motors[2], 0.0f);
    ppm_update(_motors[3], 0.0f);

    log_info("[MOTORS] motors killed !!");
}

void motors_update(float U0, float U1, float U2, float U3) {

        // Puissance faible, il ne faut pas que les motors arretent
        if (U1 < 0.001) {

                //Hack to counter the fact that the control can put the motor to stop in flight ....
                //Singleton::get()->motors.speeds(0, 0, 0, 0);
                motors_idle();
                return;
        }

//        // Correction du biais
//        U2 -= Singleton::get()->parameters.get(Parameters::BIAS_U2);
//        U3 -= Singleton::get()->parameters.get(Parameters::BIAS_U3);
//        U4 -= Singleton::get()->parameters.get(Parameters::BIAS_U4);
//
//        // Transformations U_i => Omega_i (1/3)
//
//        U1 /= Singleton::get()->parameters.get(Parameters::PHYSICS_B);
//        U2 /= Singleton::get()->parameters.get(Parameters::PHYSICS_B);
//        U3 /= Singleton::get()->parameters.get(Parameters::PHYSICS_B);
//        U4 /= Singleton::get()->parameters.get(Parameters::PHYSICS_D);

        // Transformations U_i => Omega_i (2/3)
        float O0 = (U0 + U3 + 2 * U2) / 4;
        float O1 = (U0 - U3 - 2 * U1) / 4;
        float O2 = (U0 + U3 - 2 * U2) / 4;
        float O3 = (U0 - U3 + 2 * U1) / 4;

        // Transformations U_i => Omega_i (3/3)
        O0 = O0 > 0 ? sqrt(O0) : 0;
        O1 = O1 > 0 ? sqrt(O1) : 0;
        O2 = O2 > 0 ? sqrt(O2) : 0;
        O3 = O3 > 0 ? sqrt(O3) : 0;

        // Enregistrement
//        Singleton::get()->data.set(DataStore::OMEGA_R, O1 + O3 - O2 - O4);


        // Envoi de la commande
        motors_speeds(O0, O1, O2, O3);

}

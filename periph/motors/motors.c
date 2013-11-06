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

#include <math.h>

#include "platform.h"
#include "motors.h"

#define NO_DEBUG_HEADER
#include "debug.h"

#include "gpio.h"
#include "timer.h"

static motor_t _motor;


/**
 *
 * todo definir un tableau de structure contenant les differents moteurs
 * faire passer deux param timer_config(timer, freq) et motor_config
 *
 */
void motors_config(motor_t motor){

	log_info("[MOTORS] initializing motors ...");

	_motor = motor;

	// Configure the General Purpose Timers
	timer_enable(motor.timer);

	// Start timer
	timer_start(motor.timer, PPM_PERIOD, NULL, NULL);

	timer_set_channel_compare(motor.timer, motor.channel, 0, NULL, NULL);
	timer_activate_channel_output(motor.timer, motor.channel, TIMER_OUTPUT_MODE_PWM1);

	log_info("[MOTORS] initialized !!");
}

void motors_init(){
	/**
	 *
	 * todo make motors beep !! and set to off
	 *
	 */



}

void dummy_timer(uint16_t value){

	timer_update_channel_compare(_motor.timer, _motor.channel, value);
}

void ppm_update(float ratio){

	uint16_t pulse_value = 0;
		if (ratio < 0.0f || ratio > 1.0f)
		{
			//printf("PPM Value out of range\r\n  ratio = %.2f\r\n  channel = %d\r\n", ratio, channel);
			printf("PPM Value out of range\r\n");
			return;
		}

		pulse_value = PPM_PROPORTION * ratio + PPM_MINIMUM;

		log_warning("writing 0x%04x to compare", pulse_value);

		timer_update_channel_compare(_motor.timer, _motor.channel, pulse_value);
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


void motors_speeds(const float M1, const float M2, const float M3, const float M4) {

//        //Register motor speed in rad/s
//        Singleton::get()->data.set(DataStore::M1, M1);
//        Singleton::get()->data.set(DataStore::M2, M2);
//        Singleton::get()->data.set(DataStore::M3, M3);
//        Singleton::get()->data.set(DataStore::M4, M4);
//
//
//        //Transform in ppm duty, if speed is to low, idle motor !!!!
//        const float V1 = ( M1 > 5 ? rad_to_percent(M1) : IDLE_POURC_ESC);
//        const float V2 = ( M2 > 5 ? rad_to_percent(M2) : IDLE_POURC_ESC);
//        const float V3 = ( M3 > 5 ? rad_to_percent(M3) : IDLE_POURC_ESC);
//        const float V4 = ( M4 > 5 ? rad_to_percent(M4) : IDLE_POURC_ESC);
//
//        Singleton::get()->ppm0.update(V3); //On BOARD : M1 => O3
//        Singleton::get()->ppm1.update(V4); //On BOARD : M2 => O4
//        Singleton::get()->ppm2.update(V1); //On BOARD : M3 => O1
//        Singleton::get()->ppm3.update(V2); //ON BOARD : M4 => O2

}

void motors_idle()
{

        ppm_update(IDLE_POURC_ESC);
//        Singleton::get()->ppm1.update(IDLE_POURC_ESC);
//        Singleton::get()->ppm2.update(IDLE_POURC_ESC);
//        Singleton::get()->ppm3.update(IDLE_POURC_ESC);

}


void motors_kill()
{
	log_info("motors killed !!");
//	set all 4 PWMs to 0

//        Singleton::get()->ppm0.update(0.0f); //On BOARD : M1 => O3
//        Singleton::get()->ppm1.update(0.0f); //On BOARD : M2 => O4
//        Singleton::get()->ppm2.update(0.0f); //On BOARD : M3 => O1
//        Singleton::get()->ppm3.update(0.0f); //ON BOARD : M4 => O2
}



void motors_update(float U1, float U2, float U3, float U4) {

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
//        Singleton::get()->data.set(DataStore::OMEGA_R, O1 + O3 - O2 - O4);


        // Envoi de la commande
        motors_speeds(O1, O2, O3, O4);

}

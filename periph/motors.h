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
 * motors.h
 *
 *  Created on: 2013-10-28
 *      Author: liam <liambeguin.at.gmail.com>
 *
 *
 *  Les moteurs sont controles par PPM.
 *  Le protocole PPM est base sur une longueur d'impulsion 1ms = 0% , 2ms = 100%.
 *  La fonction update, utilise un duty_% base sur la plage [1ms et 2ms].
 *  Typiquement le signal PWM a une frequence de 400hz.
 */

#ifndef MOTORS_H_
#define MOTORS_H_

#include "gpio.h"
#include "timer.h"

/**
 * definition of a type corresponding to a motor
 */
typedef struct {

	timer_t			timer;
	timer_channel_t	channel;
	uint8_t 		alternate;
	gpio_t 			port;
	gpio_pin_t 		pin;

}_motor_t;



/** Defining a type that contains all parameters and data linked to the motors **/
typedef struct{

	/** Parameters **/
	struct{
		float MOTOR_SPEED_MAX;
		float IDLE_POURC_ESC;

		float U1_MAX;
		float BIAS_U2;
		float BIAS_U3;
		float BIAS_U4;
		float BIAS_ROLL;
		float BIAS_PITCH;

		// Coéfficient de poussée vefirie avec O1=O2=O3=O4 = (environ) 455 rad/s, m = 1.198kg, g = 9.806647326217 m/s^2
		float PHYSICS_B;

		// Coéfficient de trainée (drag)
		float PHYSICS_D;
	};

	/** Data **/
	struct{
		float M1;				//!< vitesse calcule de rotation du motor 1 [rad/s]
		float M2;				//!< vitesse calcule de rotation du motor 2 [rad/s]
		float M3;				//!< vitesse calcule de rotation du motor 3 [rad/s]
		float M4;				//!< vitesse calcule de rotation du motor 4 [rad/s]
		float OMEGA_R;			//!< vitesse residuelle calcule des moteurs M0-M1+M2-M3 [rad/s]
	};
}motors_t;


motors_t drone_motors;


/** Configure motor array
 * Function used to initialize an array of motors.
 *
 * note		: All motors should be on the same timer
 */
void motors_config(_motor_t *motor);

/** Test for a multiple motor application
 * Function used to set a ratio on each motor individually
 *
 * \param *ratio value corresponding to the speed to apply on
 * 				 each motor.
 *
 * note : ratio_0 goes on motors[0]
 */
void motors_ratio(float ratio_0, float ratio_1, float ratio_2, float ratio_3);


void ppm_update(_motor_t motor, float ratio);

/**
 * Convertit de radians par secondes en commande % de plage PPM (1ms a 2ms)
 *
 */
float motors_rad_to_percent(float v);


/*! Motor control inversion mapping from control input
 * @param U1 Volonté liée au thrust
 * @param U2 Volonté liée au roll
 * @param U3 Volonté liée au pitch
 * @param U4 Volonté liée au yaw
 */
void motors_update(float U1, float U2, float U3, float U4);


/*!
 * Définit les vitesse des moteurs
 */
void motors_speeds(const float M1, const float M2, const float M3, const float M4);


/*! Put motor in slowest speed */
void motors_idle();

/*! Motors are disabled */
void motors_kill();

#endif /* MOTORS_H_ */

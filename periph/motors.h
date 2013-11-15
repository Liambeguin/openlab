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

#define DUTY_PERCENT_LIM	(  0.95f)
#define MOTOR_SPEED_MAX		(750.00f)
#define IDLE_POURC_ESC		(  0.06f)

/**
 * definition of a type corresponding to a motor
 */
typedef struct {

	timer_t			timer;
	timer_channel_t	channel;
	uint8_t 		alternate;
	gpio_t 			port;
	gpio_pin_t 		pin;

}motor_t;


/** Configure motor array
 * Function used to initialize an array of motors.
 *
 * note		: All motors should be on the same timer
 * todo 	: Check if motors can be placed on different timer
 */
void motors_config(motor_t *motor);

/** Test for a multiple motor application
 * Function used to set a ratio on each motor individually
 *
 * \param *ratio value corresponding to the speed to apply on
 * 				 each motor.
 *
 * note : ratio_0 goes on motors[0]
 */
void motors_ratio(float ratio_0, float ratio_1, float ratio_2, float ratio_3);


void ppm_update(motor_t motor, float ratio);

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

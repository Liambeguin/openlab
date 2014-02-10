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
 * roll.c
 *
 *  Created on: Feb 7, 2014
 *      Author: liam <liambeguin.at.gmail.com>
 */

#include "roll_.h"
#include "derivative.h"
#include "integrate.h"

#include "gx3.h"
#include "motors.h"
#include "attitudeController.h"

derivative_t roll_deriv;
integrate_t  roll_inte;


void att_ctrl_roll_init(float dt ,float K1, float K2, float lambda){
	roll_dt = dt;
	roll_K1 = K1;
	roll_K2 = K2;
	roll_lambda = lambda;

	derivative_init(&roll_deriv, dt);
	integrate_init(&roll_inte, dt);
}

float att_ctrl_roll_update(float cmd) {

	const float roll		 = drone_gx3.roll;
	const float dot_roll	 = drone_gx3.dot_roll;

	derivative_update(&roll_deriv, cmd);

	//Dynamique de l'erreur
	const float e1 = cmd - roll;

	integrate_update(&roll_inte, e1);

	//Erreur de la commande virtuel
	const float e2 = dot_roll - roll_deriv.derivative - roll_K1 * e1 - roll_lambda * roll_inte.integrale;


	float U = e1;
	U -= drone_control.A1 * drone_gx3.dot_pitch * drone_gx3.dot_yaw;
	U -= drone_control.A2 * drone_gx3.dot_pitch * drone_motors.OMEGA_R;
	U -= roll_K1 * (e2 + roll_K1 * e1 + roll_lambda * roll_inte.integrale);
	U += roll_lambda * e1;
	U -= roll_K2 * e2;

	//! @todo Transform this into a multiplication
	return U / drone_control.B1;

}

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
 * pitch.c
 *
 *  Created on: Feb 10, 2014
 *      Author: liam <liambeguin.at.gmail.com>
 */

#include "pitch_.h"
#include "derivative.h"
#include "integrate.h"

#include "gx3.h"
#include "motors.h"
#include "attitudeController.h"

derivative_t pitch_deriv;
integrate_t  pitch_inte;


void att_ctrl_pitch_init(float dt ,float K1, float K2, float lambda){
	pitch_dt = dt;
	pitch_K1 = K1;
	pitch_K2 = K2;
	pitch_lambda = lambda;

	derivative_init(&pitch_deriv, dt);
	integrate_init (&pitch_inte,  dt);

}

float att_ctrl_pitch_update(float cmd) {

	const float pitch		 = drone_gx3.pitch;
	const float dot_pitch	 = drone_gx3.dot_pitch;

	derivative_update(&pitch_deriv, cmd);

	//Dynamique de l'erreur
	const float e1 = cmd - pitch;

	integrate_update(&pitch_inte, e1);


	//Erreur de la commande virtuel
	const float e2 = dot_pitch - pitch_deriv.derivative - pitch_K1 * e1 - pitch_lambda * pitch_inte.integrale;

	float U = e1;
	U -= drone_control.A3 * drone_gx3.dot_roll * drone_gx3.dot_yaw;
	U -= drone_control.A4 * drone_gx3.dot_roll * drone_motors.OMEGA_R;
	U -= pitch_K1 * (e2 + pitch_K1 * e1 + pitch_lambda * pitch_inte.integrale);
	U += pitch_lambda * e1;
	U -= pitch_K2 * e2;


	return U / drone_control.B2;

}

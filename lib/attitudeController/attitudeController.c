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
 * attitudeController.c
 *
 *  Created on: Feb 7, 2014
 *      Author: liam <liambeguin.at.gmail.com>
 */
#include "debug.h"
#include "attitudeController.h"

#include "motors.h"
#include "rc.h"
#include "gx3.h"


#define DT 		0.005// 5ms => 200 Hz

#include "roll.h"
#include "pitch.h"
#include "yaw.h"




void attitudeController_init(){

	//TODO check if theses are at the right place and make them const ...
	drone_control.PHYSICS_JR	= 0.000078187598;
	drone_control.PHYSICS_IXX	= 0.013;
	drone_control.PHYSICS_IYY	= 0.013;
	drone_control.PHYSICS_IZZ	= 0.024;

	drone_control.PHYSICS_L	= 0.2125f;				    // Distance d'un moteur au centre de masse en m
	drone_control.PHYSICS_B	= 0.00001418713138124377f; 	// Coéfficient de poussée vefirie avec O1=O2=O3=O4 = (environ) 455 rad/s, m = 1.198kg, g = 9.806647326217 m/s^2
	drone_control.PHYSICS_D	= 0.00000184;			    // Coéfficient de trainée (drag)
	drone_control.PHYSICS_M	= 1.10;						// Masse en kg

	drone_control.A1 =  drone_control.PHYSICS_IYY - (drone_control.PHYSICS_IZZ/drone_control.PHYSICS_IXX);
	drone_control.A2 =  drone_control.PHYSICS_JR  /  drone_control.PHYSICS_IXX;
	drone_control.A3 = (drone_control.PHYSICS_IZZ -  drone_control.PHYSICS_IXX)/drone_control.PHYSICS_IYY;
	drone_control.A4 =  drone_control.PHYSICS_JR  /  drone_control.PHYSICS_IYY;
	drone_control.A5 = (drone_control.PHYSICS_IXX - drone_control.PHYSICS_IYY)/drone_control.PHYSICS_IZZ;

	drone_control.B1 = drone_control.PHYSICS_L / drone_control.PHYSICS_IXX;
	drone_control.B2 = drone_control.PHYSICS_L / drone_control.PHYSICS_IYY;
	drone_control.B3 = 1.0f / drone_control.PHYSICS_IZZ;



	att_ctrl_roll_init (DT /*s*/ , 10.0f, 3.0f, 1.0f);
	att_ctrl_pitch_init(DT /*s*/ , 10.0f, 3.0f, 1.0f);

	att_ctrl_yaw_init(1.0f , 0.0f , 0.0f , &drone_gx3);

}

void attitudeController_rc_control(){

	float throttle = drone_radioController.throttle;

		//Shutdown of Drone
		if (throttle < 0.02) {

//			ctrl_reset(); TODO
			motors_kill();

		}
		else
		{
			float U1 = throttle * drone_motors.U1_MAX;	// Scaling RC input for ctrl
			float U2 = att_ctrl_roll_update(drone_radioController.roll);
			float U3 = att_ctrl_pitch_update(drone_radioController.pitch);
			float U4 = att_ctrl_yaw_update(drone_radioController.yaw);

			// Enregistrement
			drone_control.CMD_U1 = U1;
			drone_control.CMD_U2 = U2;
			drone_control.CMD_U3 = U3;
			drone_control.CMD_U4 = U4;

			motors_update(U1, U2, U3, U4);
		}
	}


void attitudeController_periodical()
{
	//TODO implement is_dead()
//	if (Singleton::get()->is_dead())
//	{
//		Singleton::get()->data.set(DataStore::KILL_ENABLED, true);
//		Singleton::get()->data.set(DataStore::IDLE, false);
//		Singleton::get()->motors.kill();
//
//		this->ctrl_reset();
//	}
//	else
//	{
//		Singleton::get()->data.set(DataStore::KILL_ENABLED, false);
//		if(drone_radioController.manual_switch)
			attitudeController_rc_control();

//		else
//		{
//			log_error("not in MANUAL mode...");
//			motors_kill();
//			//control_pc();
//			TESTCTRL();
//		}
//	}
}

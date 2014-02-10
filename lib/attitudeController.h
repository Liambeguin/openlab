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
 * attitudeController.h
 *
 *  Created on: Feb 7, 2014
 *      Author: liam <liambeguin.at.gmail.com>
 */

#ifndef ATTITUDECONTROLLER_H_
#define ATTITUDECONTROLLER_H_

//TODO
typedef struct{
	struct{

		float PHYSICS_JR;
		float PHYSICS_IXX;
		float PHYSICS_IYY;
		float PHYSICS_IZZ;

		float PHYSICS_L;
		float PHYSICS_B;
		float PHYSICS_D;
		float PHYSICS_M;

		float A1;
		float A2;
		float A3;
		float A4;
		float A5;
		float B1;
		float B2;
		float B3;
	};

	struct{
		float CMD_U1;
		float CMD_U2;
		float CMD_U3;
		float CMD_U4;
	};
}control_t;

control_t drone_control;


void attitudeController_init();
void attitudeController_periodical();

#endif /* ATTITUDECONTROLLER_H_ */

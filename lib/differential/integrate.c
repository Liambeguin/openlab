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
 * integrate.h
 *
 *  		Created on: Feb 10, 2014
 *      		Author: liam <liambeguin.at.gmail.com>
 *      Implementation: Florent Touchard <jackdesbwa.at.gmail.com>
 *
 *      http://mathworld.wolfram.com/EulerForwardMethod.html
 *      http://en.wikipedia.org/wiki/Euler_method
 */

#include "integrate.h"

void integrate_init(integrate_t *integrate, float dt){
	integrate->dt_2 = (dt / 2.0f);
	integrate->integrale = 0;
	integrate->var_m1 = 0;
}

inline void integrate_update(integrate_t *integrate, const float var){
	integrate->integrale += (var + integrate->var_m1) * integrate->dt_2;
	integrate->var_m1 = var;
}

inline void integrate_reset(integrate_t *integrate) {
	integrate->integrale = 0;
}

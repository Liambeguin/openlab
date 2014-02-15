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
 * derivative.c
 *
 *  		Created on: Feb 10, 2014
 *      		Author: liam <liambeguin.at.gmail.com>
 *      Implementation: Florent Touchard <jackdesbwa.at.gmail.com>
 *
 *      http://en.wikipedia.org/wiki/Numerical_differentiation
 */

#include "derivative.h"

void derivative_init(derivative_t *derivee, float dt){
	derivee->dt = dt;
	derivee->derivative = 0;
	derivee->var_m1 = 0;
}

inline void derivative_update(derivative_t *derivee, const float var) {
	derivee->derivative = (var - derivee->var_m1) / derivee->dt;
	derivee->var_m1 = var;
}

inline void derivative_reset(derivative_t *derivee)
{
	derivee->derivative = 0;
}



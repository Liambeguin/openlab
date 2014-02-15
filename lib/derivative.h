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
 * derivative.h
 *
 *  		Created on: Feb 10, 2014
 *      		Author: liam <liambeguin.at.gmail.com>
 *      Implementation: Florent Touchard <jackdesbwa.at.gmail.com>
 *
 *      http://en.wikipedia.org/wiki/Numerical_differentiation
 */

#ifndef DERIVATIVE_H_
#define DERIVATIVE_H_

typedef struct{
	 float dt;
	 float var_m1;
	 float derivative;
}derivative_t;

/*! initialization
		@param dt La base de temps de la derive.
 */
void derivative_init(derivative_t *derivee, float dt);

/*! Calcul de la derive
		@param var valeur courante sur laquel effectue la derive
		@return la valeur de la derive
 */
inline void derivative_update(derivative_t *derivee, const float var);

/*! Remise a zero de la derivee */
inline void derivative_reset(derivative_t *derivee);

#endif /* DERIVATIVE_H_ */

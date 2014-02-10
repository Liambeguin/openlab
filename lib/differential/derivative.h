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
 *  Created on: Feb 10, 2014
 *      Author: liam <liambeguin.at.gmail.com>
 */

#ifndef DERIVATIVE_H_
#define DERIVATIVE_H_


/*! @brief Classe qui implemente une derivee numerique

	Calcul de la derivee avec la valeur passee et courante. Voir : http://en.wikipedia.org/wiki/Numerical_differentiation. \n

	Implementation par Florent Touchard : jackdesbwa@gmail.com
 */

typedef struct{
	 float dt;
	 float var_m1;
	 float derivative;
}derivative_t;

/*! initialization
		@param dt La base de temps de la derive.
 */
void derivative_init(derivative_t *derivee, float dt){
	derivee->dt = dt;
	derivee->derivative = 0;
	derivee->var_m1 = 0;
}

/*! Calcul de la derive
		@param var valeur courante sur laquel effectue la derive
		@return la valeur de la derive
 */
inline void derivative_update(derivative_t *derivee, const float var) {
	derivee->derivative = (var - derivee->var_m1) / derivee->dt;
	derivee->var_m1 = var;
//	return derivee->derivative;
}

/*! Remise a zero de la derivee */
inline void derivative_reset(derivative_t *derivee)
{
	derivee->derivative = 0;
}

#endif /* DERIVATIVE_H_ */

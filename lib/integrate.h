/*This file is part of DronolabSTM32-embedded
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
 *
 *      \brief : Classe qui implemente une integrale trapeozidale
 */

#ifndef INTEGRATE_H_
#define INTEGRATE_H_


typedef struct{
	 float dt_2;
	 float var_m1;
	 float integrale;
}integrate_t;

/*! Constructeur
		@param dt La base de temps de l'integrale.
 */
void integrate_init(integrate_t *integrate, float dt);

/*! Calcul de l'integrale
		@param var valeur courante sur laquel effectue l'integrale
		@return la valeur de l'integrale
 */
inline void integrate_update(integrate_t *integrate, const float var);

/*! Remise a zero de l'integrale */
inline void integrate_reset(integrate_t *integrate);


#endif /* INTEGRATE_H_ */

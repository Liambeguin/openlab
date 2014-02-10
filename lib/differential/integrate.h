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
 *  Created on: Feb 10, 2014
 *      Author: liam <liambeguin.at.gmail.com>
 */

#ifndef INTEGRATE_H_
#define INTEGRATE_H_


/*! @class Integrale
    @brief Classe qui implemente une integrale trapeozidale

	Calcul de l'integrale avec la valeur passee et courante. Voir : http://mathworld.wolfram.com/EulerForwardMethod.html et http://en.wikipedia.org/wiki/Euler_method. \n

	Implementation par Florent Touchard : jackdesbwa@gmail.com
 */

typedef struct{
	 float dt_2;
	 float var_m1;
	 float integrale;
}integrate_t;

/*! Constructeur
		@param dt La base de temps de l'integrale.
 */
void integrate_init(integrate_t *integrate, float dt){
	integrate->dt_2 = (dt / 2.0f);
	integrate->integrale = 0;
	integrate->var_m1 = 0;
}

/*! Calcul de l'integrale
		@param var valeur courante sur laquel effectue l'integrale
		@return la valeur de l'integrale
 */
inline void integrate_update(integrate_t *integrate, const float var){
	integrate->integrale += (var + integrate->var_m1) * integrate->dt_2;
	integrate->var_m1 = var;
}

/*! Remise a zero de l'integrale */
inline void integrate_reset(integrate_t *integrate) {
	integrate->integrale = 0;
}


#endif /* INTEGRATE_H_ */

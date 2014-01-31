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
 * endianess.h
 *
 *  Created on: Jan 18, 2014
 *      Author: liam <liambeguin.at.gmail.com>
 */

#ifndef ENDIANESS_H_
#define ENDIANESS_H_


#ifdef __AVR__
#define LITTLE_ENDIAN
#elif __AVR32__
#define BIG_ENDIAN
#else
#define LITTLE_ENDIAN
#endif

#include <stdint.h>

/*! @class Endianess
    @brief Classe qui gere le endianness lors des echanges de message

	Le endianess est la maniere dont la machine enregistre les donnes de plus d'un byte en memoire. Voir http://en.wikipedia.org/wiki/Endianness. \n\r

	Par convention toute communication par Dronolab est effectue en BIG ENDIAN. Le receveur/recepteur doit convertir les donnees du message dans le bon format. \n\r

	<B> Le systeme est en BIG ENDIAN <\B>
*/

inline uint32_t flip_endianess(const uint32_t x) {
	int ret, i;
	for(i = sizeof(uint32_t)/2; i>=0; --i) {
		*((uint8_t *) &ret + i) = *((uint8_t *) &x + sizeof(uint32_t) - i - 1);
		*((uint8_t *) &ret + sizeof(uint32_t) - i - 1) = *((uint8_t *) &x + i);
	}
	return ret;
}

#if defined(LITTLE_ENDIAN) && !defined(BIG_ENDIAN)
	#define host2littleendian(x) (x)
	#define littleendian2host(x) (x)
	#define host2bigendian(x) flip_endianess(x)
	#define bigendian2host(x) flip_endianess(x)
#elif !defined(LITTLE_ENDIAN) && defined(BIG_ENDIAN)
	#define host2littleendian(x) flip_endianess(x)
	#define littleendian2host(x) flip_endianess(x)
	#define host2bigendian(x) (x)
	#define bigendian2host(x) (x)
#else
	#error "Endianess not defined !"
#endif



#endif /* ENDIANESS_H_ */

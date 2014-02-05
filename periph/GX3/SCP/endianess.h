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

#include <stdint.h>
#include "debug.h"


#ifdef __AVR__
#define LITTLE_ENDIAN
#elif __AVR32__
#define BIG_ENDIAN
#else
#define LITTLE_ENDIAN
#endif

/*! @class Endianess
    @brief Classe qui gere le endianness lors des echanges de message

	Le endianess est la maniere dont la machine enregistre les donnes de plus d'un byte en memoire. Voir http://en.wikipedia.org/wiki/Endianness. \n\r

	Par convention toute communication par Dronolab est effectue en BIG ENDIAN. Le receveur/recepteur doit convertir les donnees du message dans le bon format. \n\r

	<B> Le systeme est en BIG ENDIAN <\B>
*/

inline void flip_endianess(void *source, int size)
{

    uint8_t *buffer[size];
    *buffer = (uint8_t *)source;

    uint8_t temp;

    /* if 16 bits */
    if(size == 2)
    {
        temp = (*buffer)[0];
        (*buffer)[0] = (*buffer)[1];
        (*buffer)[1] = temp;
        return;
    }

    /* if 32 bits */
    if(size==4)
    {
    	temp  = (*buffer)[0];
    	(*buffer)[0] = (*buffer)[3];
    	(*buffer)[3] = temp;

    	temp = (*buffer)[1];
        (*buffer)[1] = (*buffer)[2];
        (*buffer)[2] = temp;

    	return;
    }
}

#if defined(LITTLE_ENDIAN) && !defined(BIG_ENDIAN)
	#define host2littleendian(x, y) (x, y)
	#define littleendian2host(x, y) (x, y)
	#define host2bigendian(x, y) flip_endianess(x, y)
	#define bigendian2host(x, y) flip_endianess(x, y)
#elif !defined(LITTLE_ENDIAN) && defined(BIG_ENDIAN)
	#define host2littleendian(x, y) flip_endianess(x, y)
	#define littleendian2host(x, y) flip_endianess(x, y)
	#define host2bigendian(x, y) (x, y)
	#define bigendian2host(x, y) (x, y)
#else
	#error "Endianess not defined !"
#endif



#endif /* ENDIANESS_H_ */

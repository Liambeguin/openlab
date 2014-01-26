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
 * util.h
 *
 *  Created on: Jan 18, 2014
 *      Author: liam <liambeguin.at.gmail.com>
 */

#ifndef UTIL_H_
#define UTIL_H_

#include <stdint.h>

/*! @class SCPUtil
	@brief Classe qui gere le calcul du crc16 pour le module de communication

	@defgroup SCPProtocol Dronolab - Protocole de communication
    @brief Module de communication base sur l'algorithme scp avec crc16
*/


extern uint16_t const crc16_table[256];

extern uint16_t crc16(uint16_t crc, const uint8_t *buffer, uint8_t len);

static inline uint16_t crc16_byte(uint16_t crc, const uint8_t data) {
	return (crc >> 8) ^ crc16_table[(crc ^ data) & 0xff];
}


#endif /* UTIL_H_ */

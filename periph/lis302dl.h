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
 * lis302dl.h
 *
 *  Created on: 2013-10-13
 *      Author: liam
 */

//todo

#ifndef LIS302DL_H_
#define LIS302DL_H_

#include <stdint.h>

//uint8_t lis302dl_read_whoami();
uint8_t lis302dl_read_whoami();
void spiRW(uint8_t txbuf, uint8_t rxbuf);
uint8_t spiSingle(uint8_t tx);

#endif /* LIS302DL_H_ */

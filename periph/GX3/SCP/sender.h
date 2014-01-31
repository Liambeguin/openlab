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
 * sender.h
 *
 *  Created on: Jan 18, 2014
 *      Author: liam <liambeguin.at.gmail.com>
 */

#ifndef SENDER_H_
#define SENDER_H_

#include <stdint.h>
#include "uart.h"

void SCP_init(uart_t uart);

void SCP_send(const uint8_t data);
void SCP_send_n(uint8_t nb, const uint8_t * data);
void SCP_send_32(const uint32_t data0);
void SCP_send_16(const uint16_t data0);
void SCP_send_16n(uint8_t nb, const uint16_t * data);
void SCP_send_Bytes(const uint8_t * data);
void SCP_send_float(const float data0);
void SCP_send_n_float(uint8_t nb, const float * data);

#endif /* SENDER_H_ */

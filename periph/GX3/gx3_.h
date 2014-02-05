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
 * gx3_.h
 *
 *  Created on: Jan 30, 2014
 *      Author: liam <liambeguin.at.gmail.com>
 */

#ifndef GX3__H_
#define GX3__H_


#include <stdint.h>

#include "handler.h"

#include "GX3/gx3_msg.h"

void GX3_rx_handler(handler_arg_t arg, uint8_t c);
void GX3_decode_uart_rx(void);


#endif /* GX3__H_ */

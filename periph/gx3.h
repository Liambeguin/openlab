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
 * gx3.h
 *
 *  Created on: Nov 26, 2013
 *      Author: liam <liambeguin.at.gmail.com>
 */

#ifndef GX3_H_
#define GX3_H_

#include "handler.h"
#include "GX3/gx3_msg.h"

#define EOS (-1)

static void GX3_rx_handler(handler_arg_t arg, uint8_t c);

void	GX3_init();
int8_t	GX3_get_message_length(const uint8_t id);
void	GX3_process_complete_message(const uint8_t id);
void	GX3_decode_uart_rx();
void 	GX3_write_bias();
void 	GX3_calibrate();

#endif /* GX3_H_ */

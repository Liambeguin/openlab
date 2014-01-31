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
 * circular_buff.h
 *
 *  Created on: Jan 31, 2014
 *      Author: liam <liambeguin.at.gmail.com>
 */

#ifndef CIRCULAR_BUFF_H_
#define CIRCULAR_BUFF_H_

#include <stdbool.h>
#include <stdint.h>

#define GX3_BUFFER_SIZE			512


typedef struct {
	uint16_t 	size;
	uint32_t	head;
	uint32_t	tail;

	bool 		isFull;
	bool 		isEmpty;

	uint8_t 	buffer[GX3_BUFFER_SIZE];

}circular_buff_t;


circular_buff_t cb_init		(void);
void 			cb_write	(circular_buff_t *cb, uint8_t *elt);
void 			cb_read		(circular_buff_t *cb, uint8_t *elt);


#endif /* CIRCULAR_BUFF_H_ */

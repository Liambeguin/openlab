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
 * circular_buff.c
 *
 *  Created on: Jan 31, 2014
 *      Author: liam <liambeguin.at.gmail.com>
 */

#include "circular_buff.h"
#include "debug.h"

circular_buff_t cb_init(void){

	circular_buff_t circular_buff;
	circular_buff.size		= GX3_BUFFER_SIZE;
	circular_buff.head		= 0;
	circular_buff.tail		= 0;

	circular_buff.isEmpty	= true;
	circular_buff.isFull	= false;

	return circular_buff;
}

void cb_write(circular_buff_t *cb, uint8_t *elt){

	/*TODO: asign action if buffer is full overwrite ? */
	if (cb->isFull){
		log_debug("buffer is full");
	}

	    cb->isEmpty = false;
	    cb->buffer[cb->head] = *elt;
	    cb->head = (cb->head + 1) % cb->size;

	    if (cb->head == cb->tail)
	        cb->isFull = true;
}


void cb_read(circular_buff_t *cb, uint8_t *elt){

    if (cb->isEmpty){
//    	log_error("buffer is empty");
    	return;
    }

    cb->isFull = false;
    *elt = cb->buffer[cb->tail];
    cb->tail = (cb->tail + 1) % cb->size;

    if (cb->tail == cb->head)
        cb->isEmpty = true;
}

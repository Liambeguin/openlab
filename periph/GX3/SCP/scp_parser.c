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
 * scp_parser.c
 *
 *  Created on: Jan 20, 2014
 *      Author: liam <liambeguin.at.gmail.com>
 */


#include <string.h> // memcpy
#include "./scp_parser.h"
#include "../../gx3.h"
#include "util.h"


//#define DEBUG_FLAG_DRIVER_SCP_PARSER 1

#if DEBUG_FLAG_DRIVER_SCP_PARSER
#define scp_print_dbg(x...)   printf(x)
#define UNUSED
#else
#define scp_print_dbg(x...)   do { } while(0)
#define UNUSED      __attribute__((unused))
#endif


/** Taille du buffer de reception */
#define SCP_BUFF_SIZE (127 + 1 + 2)

/** Tampon de réception/décodage **/
uint8_t buff[SCP_BUFF_SIZE];
uint8_t buff_count;

/** Identifiant en cours de traitement **/
uint8_t id;

int8_t bytes_to_read;

//Monitoring fct
uint32_t sum_time_receiving_us;


void SCP_reset(){
	buff_count		= 0;
	id 				= 0;
	state 			= LOOKING_FOR_KNOWN_MESSAGE_ID;
	bytes_to_read 	= 0;
}

inline void* SCP_get_payload() {
	return buff + 1;
}

void SCP_init(){
	sum_time_receiving_us = 0;
	SCP_reset();
}

uint8_t SCP_decode(const char c) {
	switch (state) {
		case LOOKING_FOR_KNOWN_MESSAGE_ID: {
			// is the received char known ?
			bytes_to_read = GX3_get_message_length(c);
			if (bytes_to_read != UNKNOWN_MESSAGE_ID) {
				bytes_to_read += 1 + 2; // id and checksum size
				// it is a known message
				id = c;
				state = RECEIVING_KNOWN_MESSAGE;
				buff[buff_count++] = c;
			} else {

				scp_print_dbg(" Error invalid id : ");
				scp_print_dbg(id);

				// unknown message, nothing left to do
				return SCPPARSER_ERROR;
			}
			break;
		}
		case RECEIVING_KNOWN_MESSAGE: {
			buff[buff_count++] = c;
			break;
		}
	}

	return SCP_handle_completed_message();
}

uint8_t SCP_handle_completed_message() {
	uint8_t ret = SCPPARSER_OK;

//	scp_print_dbg("st: %d  bc: %d btr: %d\n",  state, buff_count, bytes_to_read);

	if (state == RECEIVING_KNOWN_MESSAGE && buff_count >= bytes_to_read) {
		// a complete message is ready to be validated.

		// calculate received checksum
		uint16_t calculatedChkSum = crc16(0x0000, buff, bytes_to_read - 2);

		// get message checksum field
		uint16_t buffChkSum = buff[bytes_to_read - 2] << 8 | buff[bytes_to_read - 1];

		uint8_t buffer_cursor_pos;

		if (buffChkSum == calculatedChkSum) {
			// valid checksum

			// process message
			GX3_process_complete_message(id);

			buffer_cursor_pos = bytes_to_read;
		} else {

			// invalid checksum
			ret = SCPPARSER_ERROR;

			buffer_cursor_pos = 1; // skip first byte, we just found out it is invalid
		}

		if (buffer_cursor_pos < buff_count) {
			// try to find a valid message in remaining buffer bytes (starting from buffer_cursor_pos)
			do {
				bytes_to_read = GX3_get_message_length(buff[buffer_cursor_pos]);
				if (bytes_to_read != UNKNOWN_MESSAGE_ID) {
					bytes_to_read += 1 + 2; // id and checksum size

					// it is a known message
					id = buff[buffer_cursor_pos];

					// move remaining byte to the begining of the buffer
					buff_count -= buffer_cursor_pos;
					memmove(buff, buff + buffer_cursor_pos, buff_count);

					// stop now so only one message checksum is calculated by function call
					return ret;
				} else {
					// unknown message. check next byte
					buffer_cursor_pos++;
				}
			} while (buffer_cursor_pos < buff_count);

			// no valid message found in remaining bytes
			ret = SCPPARSER_ERROR;
		}

		// nothing left in buffer
		// reset incomming buffer
		buff_count = 0;
		state = LOOKING_FOR_KNOWN_MESSAGE_ID;
	}

	return ret;
}

inline uint32_t SCP_sum_time_receiving()
{
	uint32_t temp = sum_time_receiving_us;
	sum_time_receiving_us = 0;

	return temp;
}

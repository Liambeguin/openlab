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
 * sender.c
 *
 *  Created on: Jan 18, 2014
 *      Author: liam <liambeguin.at.gmail.com>
 */



#include "sender.h"
#include "util.h"
#include "endianess.h"

#include "uart.h"

uart_t _scp_uart;
uint16_t bsdsum;

SCP_init(uart_t uart){

	_scp_uart = uart;
	bsdsum = 0;

}


void SCP_send(const uint8_t data) {
	bsdsum = crc16_byte(bsdsum, data);
	uart_transfer(_scp_uart, data, sizeof(data));
}

void SCP_send_n(uint8_t nb, const uint8_t * data) {
	while (nb--)
		SCP_send(*data++);
}

void SCP_send_32(const uint32_t data0) {
	const uint32_t data = host2bigendian(data0);
	uint8_t * pdata = (uint8_t *) &data;
	SCP_send(*pdata++);
	SCP_send(*pdata++);
	SCP_send(*pdata++);
	SCP_send(*pdata);
}

void SCP_send_16(const uint16_t data0) {
	const uint16_t data = host2bigendian(data0);
	uint8_t * pdata = (uint8_t *) &data;
	SCP_send(*pdata++);
	SCP_send(*pdata);
}

void SCP_send_16n(uint8_t nb, const uint16_t * data) {
	uint16_t * pdata = (uint16_t *) data;
	while (nb--)
		SCP_send_16(*pdata++);
}

void SCP_send_Bytes(const uint8_t * data) {
	while (*data)
		SCP_send(*data++);
}

void SCP_send_float(const float data0) {
	const float data = host2bigendian(data0);
	uint8_t * pdata = (uint8_t*) &data;
	SCP_send(*pdata++);
	SCP_send(*pdata++);
	SCP_send(*pdata++);
	SCP_send(*pdata);
}

void SCP_send_n_float(uint8_t nb, const float * data) {
	float * pdata = (float *) data;
	while (nb--)
		SCP_send_float(*pdata++);
}

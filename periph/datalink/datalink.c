#include <string.h>

#include "uart.h"
#include "debug.h"
#include "datalink.h"

uart_t _datalink_uart; 

void datalink_config(uart_t uart){

	_datalink_uart = uart;
	// TODO add rx_handler !!! 
}

void datalink_send (void *data, uint16_t size){

	uint8_t i;
	const uint16_t length = size/sizeof(uint8_t);
	uint8_t buffer[length];
	memcpy(&buffer, data, length);

	uart_transfer(_datalink_uart, buffer, length);
} 

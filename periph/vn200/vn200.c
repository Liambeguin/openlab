
#include "platform.h"
#include "uart.h"
#include "debug.h"

#include "vn200.h"

/* global declaration used in this file */
uart_t _vn200_uart; 


void vn200_rx_handler(handler_arg_t arg, uint8_t c){

	printf("%c", c); 
}

void vn200_init(uart_t uart) {

	_vn200_uart = uart; 
	uart_set_rx_handler(_vn200_uart, vn200_rx_handler, NULL);
}


void vn200_send_request(uint8_t const *request, uint8_t size)
{
	uart_transfer(_vn200_uart, request, size);
}

uint8_t vn200_getModel (void) {

	uint8_t request[] ={'$','V','N','R','R','G',',','0','1','*','X','X','\n'}; 
	vn200_send_request (request, 13);
	
	/* return failure since it's not done yet */ 
	return 1;
}

// static void vn200_reset(void) {}

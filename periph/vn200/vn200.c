#include <stdlib.h>
#include <string.h>

#include "platform.h"
#include "uart.h"
#include "debug.h"

#include "vn200.h"
#include "vn200_commands.h"
#include "soft_timer.h"

#define VN200_INPUT_MAX_SIZE 128
#define VN200_INPUT_MAX_ARGS 8
#define VN200_MAX_ARG_SIZE VN200_INPUT_MAX_SIZE/VN200_INPUT_MAX_ARGS
#define iscomma(c)     (c == ',')

/*commandline definitions*/
char input_line[VN200_INPUT_MAX_SIZE];
uint8_t vn200_input_buf_position = 0;
uint8_t vn200_input_ready = 1;
char *vn200_argument_list[VN200_INPUT_MAX_ARGS + 1] = {0};

/* global declaration used in this file */
uart_t _vn200_uart;


// Calculates the 8-bit checksum for the given byte sequence.
uint8_t calculateChecksum(uint8_t data[], uint8_t length)
{
	uint8_t i, cksum = 0;

	for(i=0; i<length; i++)
		cksum ^= data[i];

	return cksum;
}

static uint8_t vn200_parse_line(char *line, char *argv[])
{
	uint8_t nargs = 0;
	while (nargs < VN200_INPUT_MAX_ARGS ) {

		/* end of line, no more args */
		if (*line == '\0'){
			argv[nargs] = NULL;
			return nargs;
		}

		argv[nargs++] = line; /* begin of argument string */

		/* find end of string */
		while (*line && !iscomma(*line) && *line != '*')
			++line;

		if (*line == '\0'){ /* end of line, no more args  */
			argv[nargs] = NULL;
			return nargs;
		}

		*line++ = '\0'; /* terminate current arg   */
	}
	printf("** Too Many args (max. %d) **", VN200_INPUT_MAX_ARGS);
	return nargs;
}

static void process_line(handler_arg_t arg)
{
	uint8_t nargs = 0;
	vn200_cmd_t *ret;

	nargs = vn200_parse_line (input_line, vn200_argument_list);

	// Searching the right command and executing...
	ret = vn200_find_cmd (vn200_argument_list[0]);
	(ret->function)(nargs, vn200_argument_list);

	// flushing
	memset (vn200_argument_list, '\0', sizeof(vn200_argument_list));

	vn200_input_ready = 1;
}

void vn200_readline(handler_arg_t arg, uint8_t c)
{
	if (vn200_input_ready) {
		switch (c) {
		case '\r':
		case '\n':
			if (vn200_input_buf_position > 0) {
				vn200_input_ready = 0;
				input_line[vn200_input_buf_position] = '\0';
				vn200_input_buf_position = 0;
				event_post_from_isr(EVENT_QUEUE_APPLI, process_line, NULL);
			}
			break;
		default:
			input_line[vn200_input_buf_position++] = (char) c;
			break;
		}
	}else{
		log_error ("Not ready !");
	}
}


void vn200_sendRequest(uint8_t const *request, uint8_t size)
{
	uart_transfer(_vn200_uart, request, size);
	soft_timer_delay_ms(1);
}

void vn200_getReg(vn200_regID_t regID)
{
	uint8_t request[20], tmp[20];
	uint8_t chksum;

	sprintf((char*)tmp, "VNRRG,%02d", regID);
	chksum = calculateChecksum(tmp, 8);
	sprintf((char*)request, "$%s*%x\r\n", tmp, chksum);

	vn200_sendRequest(request, sizeof(request));
}


void vn200_init(uart_t uart)
{
	_vn200_uart = uart;
	vn200_sendRequest ((uint8_t*)"$VNRRG,01*XX\r\n", 13);

	uart_set_rx_handler(uart, vn200_readline, NULL);
}


#include <stdlib.h>
#include <string.h>

#include "platform.h"
#include "uart.h"
#include "debug.h"

#include "vn200.h"
#include "vn200_parser.h"
#include "circular_buff.h"
#include "soft_timer.h"

/* global declaration used in this file */
uart_t _vn200_uart;
circular_buff_t rx_buffer;

void vn200_decode(uint8_t c);

void vn200_rxHandler(handler_arg_t arg, uint8_t c){

/* toggle comments on the following lines to bypass circular buffer
 * this needs to be tested it may be skipping stuff since we are in
 * interrupt context
 */
	cb_write(&rx_buffer, &c);
//	vn200_decode(c);
}

void vn200_init(uart_t uart) {

	_vn200_uart = uart;

	//initializing reception circular buffer
	rx_buffer = cb_init();

	uart_set_rx_handler(uart, vn200_rxHandler, NULL);
}

void vn200_sendRequest(uint8_t const *request, uint8_t size){
	uart_transfer(_vn200_uart, request, size);
}

void vn200_decodeUartRx(void){

	uint8_t data=-1;
	if(!rx_buffer.isEmpty){
		cb_read(&rx_buffer, &data);
		vn200_decode(data);
	}
}

uint8_t* vn200_getModel (void) {

	uint8_t tmp, count=0;
	uint8_t request[] ={'$','V','N','R','R','G',',','0','1','*','X','X','\n'};
	uint8_t response[25]={0};
	uint8_t ret[11]={0};
	vn200_sendRequest (request, 13);

	/* Wainting a bit for the VN200 and reading ... */
	soft_timer_delay_ms(10);
	while (!rx_buffer.isEmpty){
		cb_read(&rx_buffer, &tmp);
		response[count]=tmp;
		count++;
	}
	memcpy(ret, response+10, 10);

	if (!strcmp((char*)ret,"VN-200T-CR"))
		return (uint8_t*)"VN-200T-CR";
	else
		return (uint8_t*)"error";
}

// Calculates the 8-bit checksum for the given byte sequence.
uint8_t calculateChecksum(uint8_t data[], uint8_t length){

	uint8_t i, cksum = 0;

	for(i=0; i<length; i++)
		cksum ^= data[i];

	return cksum;
}

/* TODO : this has to be checked the chksum may not be working */
void vn200_writeReg(vn200_regID_t regID, uint8_t* data){

	uint8_t request[100], tmp[100];
	uint8_t chksum;

	sprintf((char*)tmp, "VNWRG,%02d,%s", regID, data);
	chksum = calculateChecksum(tmp, sizeof(tmp));
	sprintf((char*)request, "$%s*%x\r\n", tmp, chksum);

	vn200_sendRequest(request, sizeof(request));
}

void vn200_readReg(vn200_regID_t regID){

	uint8_t request[20], tmp[20];
	uint8_t chksum;

	sprintf((char*)tmp, "VNRRG,%02d", regID);
	chksum = calculateChecksum(tmp, 8);
	sprintf((char*)request, "$%s*%x\r\n", tmp, chksum);

	vn200_sendRequest(request, sizeof(request));
}

/* TODO */
void vn200_writeSettings(void) {}

void vn200_decode(uint8_t c){

	static parser_state_t state = waitingForMsg;
	static uint8_t count = 0, regID = -1, chksum = 0;
	static uint8_t parserBuffer[100] = {0};

	switch (state){
		case waitingForMsg :
			if (c == '$'){
				state = getAsciiHeader;
				count = 0; memset(parserBuffer, '\0', 100);
				chksum = 0;
				regID = -1;
			}else if (0 /* TODO this would be for binary header */){

			}
			break;
		case getAsciiHeader :
			chksum ^= c;
			if (c != ',')
				parserBuffer[count++] = c;
			else {
				/* Processing headers... TODO add missing stuff */
				if (!memcmp(parserBuffer, "VNRRG", 5)){
					count = 0; memset(parserBuffer, '\0', 100);
					state = getAsciiRegID;
				}else if (!memcmp(parserBuffer, "VNWRG", 5)){
					/* Do we really need to do this ?! */
					/* Maybe to check is write went OK... */
					state = waitingForMsg;

				}else if (!memcmp(parserBuffer, "VNERR", 5)){
					count = 0; memset(parserBuffer, '\0', 100);
					state = processError;
				}else
					state = waitingForMsg;
			}
			break;
		case getAsciiRegID :
			chksum ^= c;
			if (c != ','){
				parserBuffer[count++] = c;
			}else{
				regID = atoi((char*)parserBuffer);
				count = 0; memset(parserBuffer, '\0', 100);
				state = getAsciiData;
			}
			break;
		case getAsciiData :
			if (c != '*'){
				chksum ^= c;
				parserBuffer[count++] = c;
			}
			else{
				/* Processing data depending on regID */
				vn200_processData(regID, parserBuffer);
				count = 0; memset(parserBuffer, '\0', 100);
				state = evaluateChkSum;
			}
			break;
		case evaluateChkSum :
			if (count < 2){
				parserBuffer[count++] = c;
			}else{
				if(chksum != strtol((char*)parserBuffer, NULL, 16))
					log_error("chksum don't match !");

				count = 0; memset(parserBuffer, '\0', 100);
				state = waitingForMsg;
			}
			break;
		case processError :
			if (c!= '*')
				parserBuffer[count++] = c;
			else {
				/* processing ERRORS... */
				vn200_processError(atoi((char*)parserBuffer));

				/* Ignoring the CheckSum */
				state = waitingForMsg;
			}
			break;
		default:
			state = waitingForMsg;
			break;

	}
}

#include "platform.h"
#include "debug.h"

typedef enum {
	waitingForMsg,
	getAsciiHeader,
	getAsciiRegID,
	getAsciiData,
	evaluateChkSum,
	processError
}parser_state_t;


void vn200_processData(uint8_t registerID, uint8_t* data);
void vn200_processError(uint8_t errCode);

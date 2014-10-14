#include "vn200.h"
#include "vn200_parser.h"

//TODO : write processHeader that would take the parserBuffer with the header data
//and that would return regID info

void vn200_processError(uint8_t errCode){

	switch(errCode){
		case 1:
			log_error("[VN200] HardFault");
			break;
		case 2:
			log_error("[VN200] Serial Buffer Overflow");
			break;
		case 3:
			log_error("[VN200] Invalid CheckSum");
			break;
		case 4:
			log_error("[VN200] Invalid Command");
			break;
		case 5:
			log_error("[VN200] Not Enough Parameters");
			break;
		case 6:
			log_error("[VN200] Too Many Parameters");
			break;
		case 7:
			log_error("[VN200] Invalid Parameter");
			break;
		case 8:
			log_error("[VN200] Invalid Register");
			break;
		case 9:
			log_error("[VN200] Unauthorized Access");
			break;
		case 10:
			log_error("[VN200] Watchdog Reset");
			break;
		case 11:
			log_error("[VN200] Output Buffer Overflow");
			break;
		case 12:
			log_error("[VN200] Insufficient Baudrate");
			break;
		case 255:
			log_error("[VN200] Error Buffer Overflow");
			break;
		default:
			log_error("[VN200] Unknown Error ...");
			break;
	}
}

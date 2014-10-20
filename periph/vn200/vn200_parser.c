#include <string.h>
#include <stdlib.h>

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

void vn200_printData(uint8_t registerID, uint8_t* data){}
void vn200_processData(uint8_t registerID, uint8_t* data){

	uint8_t _data[100] = {0}, parserBuffer[100]= {0};
	uint8_t count = 0;
	char * pch;
	float ypr[4] = {0};
	memcpy(_data, data, 100);
	switch (registerID){
		case USERTAG:
			printf("Tag : [%s] \n",_data);
			break;
		case MODEL_NUMBER:
			printf("Product name : %s\n", _data);
			break;
		case HARDWARE_REV:
			printf("Hardware revision : %s \n", _data);
			break;
		case SERIAL_NUMBER:
			printf("Serial Number : %s\n", _data);
			break;
		case FW_VERSION:
			printf("Firmware version : %s\n", _data);
			break;
		case SERIAL_BAUDRATE:
			printf("Serial Baudrate : %s\n", _data);
			break;
		case YAW_PITCH_ROLL:

			pch = strtok ((char*)_data,",");
			while (pch != NULL){
				ypr[count++] = strtof(pch, NULL);
				pch = strtok (NULL, ",");
			}
			printf("YPR : [%f]\t\t[%f]\t\t[%f]\n",
				ypr[0], ypr[1], ypr[2]);

			break;
		default:
			log_error("Unsupported register ID");
	}
}

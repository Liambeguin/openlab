#include "platform.h"
#include "printf.h"
#include "debug.h"
#include <string.h>
#include <stdlib.h>

#include "vn200_commands.h"

uint8_t vn200_readReg (uint8_t argc, char * const argv[]);
uint8_t vn200_processErr (uint8_t argc, char * const argv[]);

vn200_cmd_t vn200_cmds[] = {
	{"$VNRRG", vn200_readReg },
	{"$VNWRG", NULL},
	{"$VNERR", vn200_processErr},
	/* */ 
	{NULL, NULL},
};

vn200_cmd_t *vn200_find_cmd (const char * command) {

	uint8_t i = 0;
	while (vn200_cmds[i].name != NULL) {
		if (!strcmp(command, vn200_cmds[i].name)) break ;
		else i++;
	}
	return  &vn200_cmds[i];
}

uint8_t vn200_readReg (uint8_t argc, char * const argv[]){

	switch (atoi(argv[1])) {
		case 1:
			printf ("[ %d ] %s initialized ! \n", 
					soft_timer_ticks_to_ms(soft_timer_time()), argv[2]);
			break;
		default:
			log_error("reading reg%d : %s", atoi(argv[1]), argv[2]);
			break;
		}
	return 0; 
}

uint8_t vn200_processErr (uint8_t argc, char * const argv[]){
	switch (atoi(argv[1])) {
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
	return 0;
}

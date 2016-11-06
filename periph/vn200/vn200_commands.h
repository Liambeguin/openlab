#ifndef _VN200_COMMANDS_H_
#define _VN200_COMMANDS_H_

#include <stdint.h>

typedef struct {
	const char *name;
	uint8_t (*function) (uint8_t argc, char * const argv[]);
}vn200_cmd_t;

extern vn200_cmd_t vn200_cmds[];
vn200_cmd_t *vn200_find_cmd (const char * command);

#endif /* _VN200_COMMANDS_H_ */

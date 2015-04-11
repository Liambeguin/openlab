#ifndef _COMMANDS_H_
#define _COMMANDS_H_

#include "stdint.h"

// This generates the function prototypes
#define MK_CMD(x) uint8_t do_ ## x (uint8_t argc, char * const argv[])
// This constructs the command entry
#define SHELL_CMD(name, usage, help) {#name,do_ ## name, usage, help}

// function return code
// 	* 0 all good
// 	* 1 usage
// 	* ... (others)
typedef struct {
	const char *name;
	uint8_t (*function) (uint8_t argc, char * const argv[]);
	const char *usage;
	const char *help;
}cmd_entry_t;

MK_CMD(version);
MK_CMD(echo);
MK_CMD(help);
MK_CMD(leds);
MK_CMD(rgb);
MK_CMD(not_found);

extern cmd_entry_t commands[];

cmd_entry_t *find_cmd (const char * command);

#endif /* _COMMANDS_H_ */

#include "platform.h"
#include "printf.h"
#include "printf.h"
#define NO_DEBUG_HEADER
#include "debug.h"
#include <string.h>
#include <stdlib.h>
#include "_commands.h"

#include "tlc59116.h"

#define INPUT_MAX_ARGS 8
command_entry_t commands[] = {
	SHELL_CMD(help,			"help", " ............................ ", "Display this message"),
	SHELL_CMD(version,	"version", " ......................... ", "Print shell version"),
	SHELL_CMD(echo,			"echo [args...]", " .................. ", "Print text to screen"),
	SHELL_CMD(leds,			"leds <mask> <on|off|toggle>", " ..... ", "Control GPIO leds"),
	SHELL_CMD(rgb,			"rgb <id> <r> <g> <b>", " ............ ", "Control RGB  leds"),
	/* the following should always be at the end */
	SHELL_CMD(not_found, "", "", ""),
};

uint8_t do_help (uint8_t argc, char * const argv[]){
	uint8_t i = 0;
	printf ("\n USAGE                DESCRIPTION\n");
	while ( strcmp(commands[i].name, "not_found")) {
		printf ("%s%s%s\n", commands[i].usage, commands[i].spacing, commands[i].help);
		i++;
	}
	return 0;
}

uint8_t do_version (uint8_t argc, char * const argv[]){
	printf("Dronolab Shell, Version 0.1.0\nLiam BEGUIN\n");
	return 0;
}

uint8_t do_echo (uint8_t argc, char * const argv[]){
	uint8_t i;
	for (i=1; i<argc; i++)
		if (argv[i] != NULL)
			printf("%s ", argv[i]);

	printf("\n");
	return 0;
}

uint8_t do_leds (uint8_t argc, char *const argv[]) {

	if (argv[1] == NULL || argv[2] == NULL)
		return 1;
	if (!strcmp(argv[2], "on"))
		leds_on(atoi(argv[1]));

	else if (!strcmp(argv[2], "off"))
		leds_off(atoi(argv[1]));

	else if (!strcmp(argv[2], "toggle"))
		leds_toggle(atoi(argv[1]));

	else
		return -1;

	return 0;
}

uint8_t do_rgb (uint8_t argc, char *const argv[]) {

	if (argc == 5) {
		tlc59116_color_t color = {atoi(argv[2]), atoi(argv[3]), atoi(argv[4])};
		tlc59116_set_led_color((tlc59116_led_t)atoi(argv[1]), color);
	}
	else if (argc == 3) {
		
}

	return 0;

}

uint8_t do_not_found (uint8_t argc, char * const argv[]){

	if (!strcmp(argv[0], "?")) {
		do_help(argc, argv);
		return 0;
	}

	printf ("** Unknown command: %s (try 'help') **\n", argv[0]);
	return 0;
}







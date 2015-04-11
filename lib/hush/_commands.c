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
cmd_entry_t commands[] = {
	SHELL_CMD(help, "help ............ Display this message", \
"  help           - print brief description of all commands"),
	SHELL_CMD(version, "version ......... Print shell version", "  version"),
	SHELL_CMD(echo, "echo ............ echo args to console", \
"  echo [args...]     - echo args to console"),
	SHELL_CMD(leds,			"leds ............ Control GPIO leds", \
"  leds mask <on|off|toggle>     - set leds depending on mask"),
	SHELL_CMD(rgb, "rgb ............. Control RGB  leds", \
"  rgb id red green blue     - set red, green, blue on id rgb LED\n\
  rgb id color              - set id rgb LED to color. \n\
                              color can be off, white, red, green, blue,\n\
                              purple,orange,yellow"),
	/* the following should always be at the end */
	SHELL_CMD(not_found, NULL, NULL),
};

void print_usage (cmd_entry_t *command) {
	if (command->usage != NULL && command->help != NULL)
	  printf ("%s\n\nUsage : \n%s\n\n", command->usage, command->help);
}

cmd_entry_t *find_cmd (const char * command) {

	uint8_t i = 0;
	while (commands[i].usage != NULL) {
		if (!strcmp(command, commands[i].name)) break ;
		else i++;
	}
	return  &commands[i];
}

uint8_t do_help (uint8_t argc, char * const argv[]){

	uint8_t i = 0;
	cmd_entry_t *ret;

	if (argc < 2) {
		printf ("\n USAGE                DESCRIPTION\n");
		while ( strcmp(commands[i].name, "not_found")) {
			printf ("%s\n", commands[i].usage);
			i++;
		}
	} else {
		for (i=1; i<argc; i++) {
			ret = find_cmd (argv[i]);
			if (ret->usage != NULL) print_usage (ret);
			else (ret->function)(argc, argv);
		}
	}
	return 0;
}

uint8_t do_version (uint8_t argc, char * const argv[]){
	printf("Dronolab's hush Shell, Version 0.1.0\nLiam BEGUIN\n");
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
	} else if (argc == 3) {
		if (!strcmp(argv[2], "off"))
			tlc59116_set_led_color((tlc59116_led_t)atoi(argv[1]), RGB_OFF);
		else if (!strcmp(argv[2], "white"))
			tlc59116_set_led_color((tlc59116_led_t)atoi(argv[1]), RGB_WHITE);
		else if (!strcmp(argv[2], "red"))
			tlc59116_set_led_color((tlc59116_led_t)atoi(argv[1]), RGB_RED);
		else if (!strcmp(argv[2], "green"))
			tlc59116_set_led_color((tlc59116_led_t)atoi(argv[1]), RGB_GREEN);
		else if (!strcmp(argv[2], "blue"))
			tlc59116_set_led_color((tlc59116_led_t)atoi(argv[1]), RGB_BLUE);
		else if (!strcmp(argv[2], "purple"))
			tlc59116_set_led_color((tlc59116_led_t)atoi(argv[1]), RGB_PURPLE);
		else if (!strcmp(argv[2], "orange"))
			tlc59116_set_led_color((tlc59116_led_t)atoi(argv[1]), RGB_ORANGE);
		else if (!strcmp(argv[2], "yellow"))
			tlc59116_set_led_color((tlc59116_led_t)atoi(argv[1]), RGB_YELLOW);
		else {
			printf (" ** Unsupported color %s **\n", argv[2]);
			return 1;
		}
	} else {
		return 1;
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







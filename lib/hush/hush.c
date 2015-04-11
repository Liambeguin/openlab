/*
 * hush.c
 *
 *  Created on : %DATE%
 * 		  Author : %AUTHOR%
 *
 * 		  Interesting sources of info:
 * 			 * http://fundamental-code.com/interp/
 */

#include "uart.h"
#include "soft_timer.h"
#include "printf.h"
#include "event.h"
#include "hush.h"
#include "_commands.h"
#include <string.h>
#define NO_DEBUG_HEADER
#include "debug.h"

#define INPUT_MAX_SIZE 128
#define INPUT_MAX_ARGS 8
#define MAX_ARG_SIZE INPUT_MAX_SIZE/INPUT_MAX_ARGS
#define PROMPT "#/ "
#define isblank(c)     (c == ' ' || c == '\t')
#define beep()				 printf("\a")
/*commandline definitions*/
char input_line[INPUT_MAX_SIZE];
uint8_t input_buf_position = 0;
uint8_t input_ready = 1;
//char argument_list [INPUT_MAX_ARGS][MAX_ARG_SIZE];
char *argument_list[INPUT_MAX_ARGS + 1] = {0};

void print_prompt (void) {
	printf(PROMPT);
}

void print_welcome (void) {
	soft_timer_delay_ms(10);
	printf("\n \
                ._,.\n \
           \"..-..pf.\n \
          -L   ..#'\n \
        .+_L  .\"]#\n \
        ,'j' .+.j`                 -'.__..,.,p.\n \
       _~ #..<..0.                 .J-.``..._f.\n \
      .7..#_.. _f.                .....-..,`4'\n \
      ;` ,#j.  T'      ..         ..J....,'.j`\n \
     .` ..\"^.,-0.,,,,yMMMMM,.    ,-.J...+`.j@\n \
    .'.`...' .yMMMMM0M@^=`\"\"g.. .'..J..\".'.jH\n \
    j' .'1`  q'^)@@#\"^\".`\"='BNg_...,]_)'...0-\n \
   .T ...I. j\"    .'..+,_.'3#MMM0MggCBf....F.\n \
   j/.+'.{..+       `^~'-^~~\"\"\"\"'\"\"\"?\'\"``'1`\n \
   .... .y.}                  `.._-:`_...jf\n \
   g-.  .Lg'                 ..,..'-....,'.\n \
  .'.   .Y^                  .....',].._f\n \
  ......-f.                 .-,,.,.-:--&`\n \
                            .`...'..`_J`\n \
                            .~......'#'\n \
                            '..,,.,_]`\n \
                            .L..`...\n\n");
	printf("type '?' or 'help' for more informations\n");
	print_prompt();
}

static uint8_t parse_line (char *line, char *argv[]) {

	uint8_t nargs = 0;
	while (nargs < INPUT_MAX_ARGS ) {

		/* skip any white spaces */
		while (isblank(*line))
			++line;

		/* end of line, no more args */
		if (*line == '\0'){
			argv[nargs] = NULL;
			return nargs;
		}

		argv[nargs++] = line; /* begin of argument string */

		/* find end of string */
		while (*line && !isblank(*line))
			++line;


		if (*line == '\0'){ /* end of line, no more args  */
			argv[nargs] = NULL;
			return nargs;
		}

		*line++ = '\0'; /* terminate current arg   */
	}
	printf ("** Too Many args (max. %d) **", INPUT_MAX_ARGS);
	return nargs;
}

static void process_command (handler_arg_t arg){

	uint8_t i = 0;
	uint8_t nargs = 0;
	cmd_entry_t *ret;
	printf("\n");


	nargs = parse_line(input_line, argument_list);

	// Searching the right command and executing...
	ret = find_cmd (argument_list[0]);
	(ret->function)(nargs, argument_list);

	// flushing the argument buffer
	memset(argument_list, '\0', sizeof(argument_list));

	input_ready = 1;
	print_prompt();
}

static void readline (handler_arg_t arg, uint8_t c) {

	static uint8_t skip = 0;
	if (input_ready && (input_buf_position >= 0) && (input_buf_position <= INPUT_MAX_SIZE) ){

		/* if almost full exit */
		if (input_buf_position == INPUT_MAX_SIZE -1 && c != '\n'){
			beep();
			printf ("\n** Too long ! try again (max. %d) **", INPUT_MAX_SIZE);
			input_line[input_buf_position] = '\0';
			input_buf_position = 0;
			printf("\n");
			print_prompt();
			return;
		}

		if (skip){
			skip--;
			return;
		}

		switch (c) {
		case '\r':
		case '\n':
			if (input_buf_position > 0) {
 				input_ready = 0;
				input_line[input_buf_position] = '\0';
				input_buf_position = 0;
				event_post_from_isr(EVENT_QUEUE_APPLI, process_command, NULL);
			} else {
				printf("\n");
				print_prompt();
			}
			break;
		case '\t': // This could be used for auto completion...
		case '~':
			break;
		case 0x7F: // Backspace
			if (input_buf_position > 0){
				input_buf_position--;
				printf ("\b \b");
			} else {
				beep();
			}
			break;
		case 0x03:
			printf ("^C\n");
			print_prompt();
			input_buf_position = 0;
			break;
		case 0x1B:
			// skip Escape character and other
			// charracters comming after (eg. [A, [B, [C, [D)
			skip = 2;
			break;
		default:
			printf("%c", c);
			input_line[input_buf_position++] = (char) c;
			break;
		}
	}
}

void hush_init (uart_t uart) {

	uart_set_rx_handler(uart, readline, NULL);
	event_post(EVENT_QUEUE_APPLI, (handler_t) print_welcome, NULL);
}

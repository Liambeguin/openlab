/*
 * cli.c
 *
 *  Created on: 
 *      Author: 
 */

#include "platform.h"
#include "printf.h"
#include "uart.h"
#include "cli.h"
#include "event.h"

static void start (handler_arg_t arg){
	printf(" == Dronolab Command line Interface ==\n");
}

int main()
{
    // Initialize the platform
    platform_init();

		event_init();

    // Set initial values
    leds_off(0x00);
    leds_on(0x01);

		event_post(EVENT_QUEUE_APPLI, start, NULL);

    cli_init(uart_print);

		platform_run();
    return 0;
}

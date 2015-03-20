#include <stdint.h>
#include <math.h>
#include "platform.h"
#include "debug.h"
#include "uart.h"

#include "can.h"

extern can_t can;

// Task function
static void tx(handler_arg_t arg);

static soft_timer_t st;

int main()
{
    // Initialize the platform
    platform_init();
    soft_timer_init();

    soft_timer_set_handler(&st, tx, NULL);
    soft_timer_start(&st, soft_timer_s_to_ticks(1), 1);

    // Run
    platform_run();
    return 0;
}

static void tx(handler_arg_t arg)
{
    static uint8_t data[4] =
    { 'A', 'B', 'C' };
    data[3]++;
    log_printf("sending! %x\n", data[3]);
    can_transmit(can, 1, 0x123, data, 4);
}

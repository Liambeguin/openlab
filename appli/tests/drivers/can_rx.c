#include <stdint.h>
#include "platform.h"
#include "debug.h"
#include "uart.h"

#include "can.h"

extern can_t can;

static void can_rx(handler_arg_t arg, uint32_t mailbox, uint16_t id,
        const uint8_t *data, uint8_t length);

int main()
{
    // Initialize the platform
    platform_init();

    // Configure the CAN
    can_set_rx_handler(can, can_rx, NULL);

    // Run
    platform_run();
    return 0;
}

static void can_rx(handler_arg_t arg, uint32_t mailbox, uint16_t id,
        const uint8_t *data, uint8_t length)
{
    log_printf("CAN message from %x (%u): ", id, length);
    uint32_t i;
    for (i = 0; i < length; i++)
    {
        log_printf("%2x ", data[i]);
    }
    log_printf("\n");
}

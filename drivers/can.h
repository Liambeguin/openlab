/*
 * can.h
 *
 *  Created on: Dec 18, 2011
 *      Author: clem
 */

#ifndef CAN_H_
#define CAN_H_

#include <stdint.h>
#include "handler.h"

typedef const void* can_t;

typedef enum
{
    CAN_OK = 0,
    CAN_ERR_TX_FAILED = 1,
    CAN_ERR_TX_MAILBOX_FULL = 2,
    CAN_ERR_TOO_BIG = 3,
    CAN_ERR_TX_INVALID_MAILBOX = 4,
} can_status_t;

typedef void (*can_tx_handler_t)(handler_arg_t arg, uint32_t mailbox,
        can_status_t status);
typedef void (*can_rx_handler_t)(handler_arg_t arg, uint32_t mailbox,
        uint16_t id, const uint8_t *data, uint8_t length);

void can_enable(can_t can, uint32_t baudrate);
void can_disable(can_t can);

void can_set_tx_handler(can_t can, can_tx_handler_t handler, handler_arg_t arg);
void can_set_rx_handler(can_t can, can_rx_handler_t handler, handler_arg_t arg);

can_status_t can_transmit(can_t can, uint32_t mailbox, uint16_t id,
        const uint8_t* data, uint8_t length);

#endif /* CAN_H_ */

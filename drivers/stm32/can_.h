/*
 * can_.h
 *
 *  Created on: Dec 18, 2011
 *      Author: clem
 */

#ifndef CAN__H_
#define CAN__H_

#include <stdint.h>

#include "can.h"
#include "rcc.h"
#include "nvic.h"
#include "gpio.h"
#include "handler.h"

typedef struct
{
    can_tx_handler_t tx_handler;
    can_rx_handler_t rx_handler;
    handler_arg_t tx_arg, rx_arg;
} _can_data_t;

typedef struct
{
    uint32_t base_address;

    rcc_apb_bus_t apb_bus;
    rcc_apb_bit_t apb_bit;

    nvic_irq_line_t irq_line_tx, irq_line_rx0, irq_line_rx1, irq_line_sce;

    _can_data_t *data;
} _can_t;

/**
 * Initialize the CAN object, with all the required parameters.
 */
static inline void can_init(_can_t *_can, uint32_t base_address,
        rcc_apb_bus_t apb_bus, rcc_apb_bit_t apb_bit,
        nvic_irq_line_t irq_line_tx, nvic_irq_line_t irq_line_rx0,
        nvic_irq_line_t irq_line_rx1, nvic_irq_line_t irq_line_sce)
{
    _can->base_address = base_address;
    _can->apb_bus = apb_bus;
    _can->apb_bit = apb_bit;

    _can->data->tx_handler = NULL;
    _can->data->rx_handler = NULL;
}

#define CAN_INIT(name, addr, bus, bit, line_tx, line_rx0, line_rx1, line_sce) \
    static _can_data_t name##_data; \
    const _can_t name = { \
    .base_address = addr, \
    .apb_bus = bus, \
    .apb_bit = bit, \
    .irq_line_tx = line_tx, \
    .irq_line_rx0 = line_rx0, \
    .irq_line_rx1 = line_rx1, \
    .irq_line_sce = line_sce, \
    .data = &name##_data \
}

void can_handle_tx_interrupt(const _can_t *_can);
void can_handle_rx0_interrupt(const _can_t *_can);
void can_handle_rx1_interrupt(const _can_t *_can);
void can_handle_sce_interrupt(const _can_t *_can);

#endif /* CAN__H_ */

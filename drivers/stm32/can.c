/*
 * can.c
 *
 *  Created on: Dec 18, 2011
 *      Author: clem
 */
#include <stdint.h>
#include "can_.h"
#include "can_registers.h"

#define LOG_LEVEL 3
#include "debug.h"

#define ENABLE_BKPT 0

void can_enable(can_t can, uint32_t baudrate) {
    const _can_t *_can = can;

    // Enable the CAN in the RCC
    rcc_apb_enable(RCC_APB_BUS_CAN1, RCC_APB_BIT_CAN1);
    rcc_apb_enable(_can->apb_bus, _can->apb_bit);

    // Reset the CAN in APBr
    rcc_apb_reset(RCC_APB_BUS_CAN1, RCC_APB_BIT_CAN1);
    rcc_apb_reset(_can->apb_bus, _can->apb_bit);

    // Reset the CAN
    *can_get_MCR(_can) |= CAN_MCR__RESET;
    *can_get_MCR(_can) = 0;

    // Request initialization mode
    *can_get_MCR(_can) = CAN_MCR__INRQ;

    // Wait until ACKed
    while ((*can_get_MSR(_can) & CAN_MSR__INAK) == 0) {
    }

    // Set Bitrate in BTR
    uint32_t pclock =
            (_can->apb_bus == 1) ?
                    rcc_sysclk_get_clock_frequency(RCC_SYSCLK_CLOCK_PCLK1) :
                    rcc_sysclk_get_clock_frequency(RCC_SYSCLK_CLOCK_PCLK2);

    *can_get_BTR(_can) = (1 << 24) | (3 << 20) | (5 << 16)
            | (pclock / baudrate - 1);

    // Set debug options
    //    *can_get_BTR(_can) |= CAN_BTR__LBKM | CAN_BTR__SILM;

    // Set Options in MCR
    *can_get_MCR(_can) = CAN_MCR__ABOM | CAN_MCR__NART | CAN_MCR__INRQ;

    // Set the interrupts
    *can_get_IER(_can) = CAN_IER__ERRIE | CAN_IER__EPVIE | CAN_IER__TMEIE
            | CAN_IER__FOVIE0 | CAN_IER__FFIE0 | CAN_IER__FMPIE0
            | CAN_IER__FOVIE1 | CAN_IER__FFIE1 | CAN_IER__FMPIE1;

    // Leave initialization
    *can_get_MCR(_can) &= ~CAN_MCR__INRQ;

    // Wait until ACKed
    while (*can_get_MSR(_can) & CAN_MSR__INAK) {
    }

    // Set a filter for can1 and can2
    *can_get_FMR() = CAN_FMR__FINIT;
    *can_get_FMR() = (14 << 8) | CAN_FMR__FINIT;

    *can_get_FM1R() = 0;
    *can_get_FS1R() = 0;
    *can_get_FFA1R() = 0;
    *can_get_FA1R() = (1 << 14) | (1 << 0);
    *can_get_FxR1(0) = 0;
    *can_get_FxR2(0) = 0;

    // Clear the FINIT bit
    *can_get_FMR() = 0;

    // Enable interrupts
    nvic_enable_interrupt_line(_can->irq_line_tx);
    nvic_enable_interrupt_line(_can->irq_line_rx0);
    nvic_enable_interrupt_line(_can->irq_line_rx1);
    nvic_enable_interrupt_line(_can->irq_line_sce);

    log_debug("CAN debug:");
    log_debug("\tMCR: %x", *can_get_MCR(_can));
    log_debug("\tBTR: %x", *can_get_BTR(_can));
    log_debug("\tIER: %x", *can_get_IER(_can));
    log_debug("\tESR: %x", *can_get_ESR(_can));
    log_debug("\tFMR: %x", *can_get_FMR());
    log_debug("\tFM1R: %x", *can_get_FM1R());
    log_debug("\tFS1R: %x", *can_get_FS1R());
    log_debug("\tFFA1R: %x", *can_get_FFA1R());
    log_debug("\tFA1R: %x", *can_get_FA1R());
    log_debug("\tFxR1_0: %x", *can_get_FxR1(0));
    log_debug("\tFxR2_0: %x", *can_get_FxR2(0));
}
void can_disable(can_t can) {
    const _can_t *_can = can;

    // Disable the CAN in the RCC
    rcc_apb_disable(_can->apb_bus, _can->apb_bit);
}

void can_set_tx_handler(can_t can, can_tx_handler_t handler, handler_arg_t arg) {
    const _can_t *_can = can;

    // Store handler and arg
    _can->data->tx_handler = handler;
    _can->data->tx_arg = arg;
}
void can_set_rx_handler(can_t can, can_rx_handler_t handler, handler_arg_t arg) {
    const _can_t *_can = can;

    // Store handler and arg
    _can->data->rx_handler = handler;
    _can->data->rx_arg = arg;
}

can_status_t can_transmit(can_t can, uint32_t mailbox, uint16_t id,
        const uint8_t* data, uint8_t length) {
    const _can_t *_can = can;

    // Check length
    if (length > 8) {
        log_error("Length too big");
        return CAN_ERR_TOO_BIG;
    }

    // Check mailbox
    if (mailbox > 2) {
        log_error("Invalid Mailbox");
        return CAN_ERR_TX_INVALID_MAILBOX;
    }

    // Check if the first fifo is empty
    if (*can_get_TIxR(_can, mailbox) & CAN_TIxR__TXRQ) {
        log_error("TX mailbox not empty!");
        return CAN_ERR_TX_MAILBOX_FULL;
    }

    // Set the identifier
    *can_get_TIxR(_can, mailbox) = (id << 21);

    // Set the data length
    *can_get_TDTxR(_can, mailbox) = length;

    // Copy the data
    uint32_t i;

    *can_get_TDLxR(_can, mailbox) = 0;
    *can_get_TDHxR(_can, mailbox) = 0;

    for (i = 0; i < length; i++) {
        if (i < 4) {
            *can_get_TDLxR(_can, mailbox) |= ((uint32_t) data[i]) << (8 * i);
        } else {
            *can_get_TDHxR(_can, mailbox) |= ((uint32_t) data[i])
                    << (8 * (i - 4));
        }
    }

    // Request transmission
    *can_get_TIxR(_can, mailbox) |= CAN_TIxR__TXRQ;

    // Wait until sent!
    while (*can_get_TIxR(_can, mailbox) & CAN_TIxR__TXRQ) {
    }

    return CAN_OK;

}

void can_handle_tx_interrupt(const const _can_t *_can) {
    // Get the first mailbox to have triggered
    uint32_t mailbox;
    uint32_t tsr = *can_get_TSR(_can);

    if (tsr & CAN_TSR__RQCP0) {
        mailbox = 0;
    } else if (tsr & CAN_TSR__RQCP1) {
        mailbox = 1;
    } else if (tsr & CAN_TSR__RQCP2) {
        mailbox = 2;
    } else {
        // Error
        return;
    }

    log_debug("TSR: %x", tsr);

    // Set the bit to clear it
    *can_get_TSR(_can) = CAN_TSR__RQCP0 << (8 * mailbox);

    // Get status
    can_status_t status;
    if (tsr & (CAN_TSR__TXOK0 << (8 * mailbox))) {
        status = CAN_OK;
        log_info("TX mailbox %u: OK", mailbox);
    } else {
        status = CAN_ERR_TX_FAILED;
        log_error("TX mailbox %u: Failed", mailbox);
#if ENABLE_BKPT
        asm ("bkpt 0");
#endif
    }

    // Call handler if any
    if (_can->data->tx_handler) {
        _can->data->tx_handler(_can->data->tx_arg, mailbox, status);
    }
}

static void can_handle_rx_interrupt(const const _can_t *_can, uint32_t fifo) {
    volatile uint32_t *rfxr = can_get_RFxR(_can, fifo);
    uint32_t rfxr_value = *rfxr;

    if (rfxr_value & CAN_RFxR__FMP_MASK) {
        log_info("RX%u interrupt FMP: %x", fifo,
                rfxr_value & CAN_RFxR__FMP_MASK);

        // Get the parameters of the received message
        uint16_t id = (*can_get_RIxR(_can, fifo) & CAN_RIxR__STDID_MASK) >> 21;
        uint8_t length = *can_get_RDTxR(_can, fifo) & 0xF;
        uint8_t *data = (uint8_t*) (uint32_t) can_get_RDLxR(_can, fifo);

        // Call handler if any
        if (_can->data->rx_handler) {
            _can->data->rx_handler(_can->data->rx_arg, 0, id, data, length);
        }

        // Clear the message from the the RX FIFO
        *rfxr = CAN_RFxR__RFOM;
    }
    if (*rfxr & CAN_RFxR__FULL) {
        log_error("RX%u interrupt FULL0", fifo);

#if ENABLE_BKPT
        asm ("bkpt 0");
#endif

        // Set the bit to clear
        *rfxr = CAN_RFxR__FULL;
    }
    if (*rfxr & CAN_RFxR__FOVR) {
        log_info("RX%u interrupt Overrun0", fifo);
#if ENABLE_BKPT
        asm ("bkpt 0");
#endif

        // Set the bit to clear
        *rfxr = CAN_RFxR__FOVR;
    }

}
void can_handle_rx0_interrupt(const const _can_t *_can) {
    can_handle_rx_interrupt(_can, 0);
}
void can_handle_rx1_interrupt(const const _can_t *_can) {
    can_handle_rx_interrupt(_can, 1);
}
void can_handle_sce_interrupt(const const _can_t *_can) {
    uint32_t esr = *can_get_ESR(_can);
    log_info("SCE interrupt, ESR: %x, TSR %x", esr, *can_get_TSR(_can));

    if (esr & CAN_ESR__EPVF) {
        // Check if TX error
        if ((esr & CAN_ESR__TEC_MASK) >> 16 >= 128) {
            // Find the current mailbox being sent
            if (*can_get_TSR(_can) & (CAN_TSR__TERR0 | CAN_TSR__ALST0)) {
                log_error("TX 0 error, aborting");

#if ENABLE_BKPT
                asm ("bkpt 0");
#endif

                // Abort and clear all flags
                *can_get_TSR(_can) = CAN_TSR__ABRQ0;
            } else
            // Find the current mailbox being sent
            if (*can_get_TSR(_can) & (CAN_TSR__TERR1 | CAN_TSR__ALST1)) {
                log_error("TX 1 error, aborting");

#if ENABLE_BKPT
                asm ("bkpt 0");
#endif

                // Abort and clear all flags
                *can_get_TSR(_can) = CAN_TSR__ABRQ1;
            } else
            // Find the current mailbox being sent
            if (*can_get_TSR(_can) & (CAN_TSR__TERR2 | CAN_TSR__ALST2)) {
                log_error("TX 2 error, aborting");

#if ENABLE_BKPT
                asm ("bkpt 0");
#endif

                // Abort
                *can_get_TSR(_can) = CAN_TSR__ABRQ2;
            }
        }
    }

    // Clear the Error Bit
    *can_get_MSR(_can) = CAN_MSR__ERRI;
}

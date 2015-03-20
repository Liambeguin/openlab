/*
 * can_registers.h
 *
 *  Created on: Dec 18, 2011
 *      Author: clem
 */

#ifndef CAN_REGISTERS_H_
#define CAN_REGISTERS_H_

#include <stdint.h>
#include "memmap.h"

#include "can_.h"

static inline volatile uint32_t* can_get_MCR(const _can_t* _can)
{
    return mem_get_reg32(_can->base_address + CAN_MCR_OFFSET);
}

enum
{
    CAN_MCR__DBF = 0x10000,
    CAN_MCR__RESET = 0x8000,
    CAN_MCR__TTCM = 0x80,
    CAN_MCR__ABOM = 0x40,
    CAN_MCR__AWUM = 0x20,
    CAN_MCR__NART = 0x10,
    CAN_MCR__RFLM = 0x08,
    CAN_MCR__TXFP = 0x04,
    CAN_MCR__SLEEP = 0x02,
    CAN_MCR__INRQ = 0x01,
};

static inline volatile uint32_t* can_get_MSR(const _can_t* _can)
{
    return mem_get_reg32(_can->base_address + CAN_MSR_OFFSET);
}

enum
{
    CAN_MSR__RX = 0x0800,
    CAN_MSR__SAMP = 0x0400,
    CAN_MSR__RXM = 0x0200,
    CAN_MSR__TXM = 0x0100,
    CAN_MSR__SLAKI = 0x0010,
    CAN_MSR__WKUI = 0x0008,
    CAN_MSR__ERRI = 0x0004,
    CAN_MSR__SLAK = 0x0002,
    CAN_MSR__INAK = 0x0001,
};

static inline volatile uint32_t* can_get_TSR(const _can_t* _can)
{
    return mem_get_reg32(_can->base_address + CAN_TSR_OFFSET);
}

enum
{
    CAN_TSR__LOW2 = 0x80000000,
    CAN_TSR__LOW1 = 0x40000000,
    CAN_TSR__LOW0 = 0x20000000,
    CAN_TSR__TME2 = 0x10000000,
    CAN_TSR__TME1 = 0x08000000,
    CAN_TSR__TME0 = 0x04000000,
    CAN_TSR__CODE_MASK = 0x03000000,

    CAN_TSR__ABRQ2 = 0x00800000,
    CAN_TSR__TERR2 = 0x00080000,
    CAN_TSR__ALST2 = 0x00040000,
    CAN_TSR__TXOK2 = 0x00020000,
    CAN_TSR__RQCP2 = 0x00010000,

    CAN_TSR__ABRQ1 = 0x00008000,
    CAN_TSR__TERR1 = 0x00000800,
    CAN_TSR__ALST1 = 0x00000400,
    CAN_TSR__TXOK1 = 0x00000200,
    CAN_TSR__RQCP1 = 0x00000100,

    CAN_TSR__ABRQ0 = 0x00000080,
    CAN_TSR__TERR0 = 0x00000008,
    CAN_TSR__ALST0 = 0x00000004,
    CAN_TSR__TXOK0 = 0x00000002,
    CAN_TSR__RQCP0 = 0x00000001,
};

static inline volatile uint32_t* can_get_RFxR(const _can_t* _can, uint32_t fifo)
{
    if (fifo == 0)
    {
        return mem_get_reg32(_can->base_address + CAN_RF0R_OFFSET);
    }
    else
    {
        return mem_get_reg32(_can->base_address + CAN_RF1R_OFFSET);
    }
}
enum
{
    CAN_RFxR__RFOM = 0x20,
    CAN_RFxR__FOVR = 0x10,
    CAN_RFxR__FULL = 0x08,
    CAN_RFxR__FMP_MASK = 0x03,
};

static inline volatile uint32_t* can_get_IER(const _can_t* _can)
{
    return mem_get_reg32(_can->base_address + CAN_IER_OFFSET);
}

enum
{
    CAN_IER__SLKIE = 0x020000,
    CAN_IER__WKUIE = 0x010000,
    CAN_IER__ERRIE = 0x8000,
    CAN_IER__LECIE = 0x0800,
    CAN_IER__BOFIE = 0x0400,
    CAN_IER__EPVIE = 0x0200,
    CAN_IER__EWGIE = 0x0100,
    CAN_IER__FOVIE1 = 0x0040,
    CAN_IER__FFIE1 = 0x0020,
    CAN_IER__FMPIE1 = 0x0010,
    CAN_IER__FOVIE0 = 0x0008,
    CAN_IER__FFIE0 = 0x0004,
    CAN_IER__FMPIE0 = 0x0002,
    CAN_IER__TMEIE = 0x0001,
};

static inline volatile uint32_t* can_get_ESR(const _can_t* _can)
{
    return mem_get_reg32(_can->base_address + CAN_ESR_OFFSET);
}

enum
{
    CAN_ESR__REC_MASK = 0xFF000000,
    CAN_ESR__TEC_MASK = 0x00FF0000,
    CAN_ESR__LEC_MASK = 0x00000070,
    CAN_ESR__BOFF = 0x00000004,
    CAN_ESR__EPVF = 0x00000002,
    CAN_ESR__EWGF = 0x00000001,
};

static inline volatile uint32_t* can_get_BTR(const _can_t* _can)
{
    return mem_get_reg32(_can->base_address + CAN_BTR_OFFSET);
}

enum
{
    CAN_BTR__SILM = 0x80000000,
    CAN_BTR__LBKM = 0x40000000,
    CAN_BTR__SJW_MASK = 0x03000000,
    CAN_BTR__TS2_MASK = 0x00700000,
    CAN_BTR__TS1_MASK = 0x000F0000,
    CAN_BTR__BRP_MASK = 0x000003FF,
};

/** mailbox is 0-2 */
static inline volatile uint32_t* can_get_TIxR(const _can_t* _can, uint32_t mailbox)
{
    return mem_get_reg32(
            _can->base_address + CAN_TI0R_OFFSET + mailbox * (CAN_TIxR_OFFSET));
}

enum
{
    CAN_TIxR__STDID_MASK = 0xFFE00000,
    CAN_TIxR__EXID_MASK = 0xFFFFFFF8,
    CAN_TIxR__IDE = 0x00000004,
    CAN_TIxR__RTR = 0x00000002,
    CAN_TIxR__TXRQ = 0x00000001,
};

/** mailbox is 0-2 */
static inline volatile uint32_t* can_get_TDTxR(const _can_t* _can, uint32_t mailbox)
{
    return mem_get_reg32(
            _can->base_address + CAN_TDT0R_OFFSET + mailbox * (CAN_TIxR_OFFSET));
}

enum
{
    CAN_TDTxR__TIME_MASK = 0xFFFF0000,
    CAN_TDTxR__TGT = 0x00000100,
    CAN_TDTxR__DLC_MASK = 0x0000000F,
};

/** mailbox is 0-2 */
static inline volatile uint32_t* can_get_TDLxR(const _can_t* _can, uint32_t mailbox)
{
    return mem_get_reg32(
            _can->base_address + CAN_TDL0R_OFFSET + mailbox * (CAN_TIxR_OFFSET));
}
/** mailbox is 0-2 */
static inline volatile uint32_t* can_get_TDHxR(const _can_t* _can, uint32_t mailbox)
{
    return mem_get_reg32(
            _can->base_address + CAN_TDH0R_OFFSET + mailbox * (CAN_TIxR_OFFSET));
}

/** mailbox is 0-1 */
static inline volatile uint32_t* can_get_RIxR(const _can_t* _can, uint32_t mailbox)
{
    return mem_get_reg32(
            _can->base_address + CAN_RI0R_OFFSET + mailbox * (CAN_RIxR_OFFSET));
}

enum
{
    CAN_RIxR__STDID_MASK = 0xFFE00000,
    CAN_RIxR__EXID_MASK = 0xFFFFFFF8,
    CAN_RIxR__IDE = 0x00000004,
    CAN_RIxR__RTR = 0x00000002,
};

/** mailbox is 0-1 */
static inline volatile uint32_t* can_get_RDTxR(const _can_t* _can, uint32_t mailbox)
{
    return mem_get_reg32(
            _can->base_address + CAN_RDT0R_OFFSET + mailbox * (CAN_RIxR_OFFSET));
}

enum
{
    CAN_RDTxR__TIME_MASK = 0xFFFF0000,
    CAN_RDTxR__FMI_MASK = 0x0000FF00,
    CAN_RDTxR__DLC_MASK = 0x0000000F,
};

/** mailbox is 0-1 */
static inline volatile uint32_t* can_get_RDLxR(const _can_t* _can, uint32_t mailbox)
{
    return mem_get_reg32(
            _can->base_address + CAN_RDL0R_OFFSET + mailbox * (CAN_RIxR_OFFSET));
}
/** mailbox is 0-1 */
static inline volatile uint32_t* can_get_RDHxR(const _can_t* _can, uint32_t mailbox)
{
    return mem_get_reg32(
            _can->base_address + CAN_RDH0R_OFFSET + mailbox * (CAN_RIxR_OFFSET));
}

static inline volatile uint32_t* can_get_FMR()
{
    return mem_get_reg32(CAN1_BASE_ADDRESS + CAN_FMR_OFFSET);
}

enum
{
    CAN_FMR__FINIT = 0x1,
};

static inline volatile uint32_t* can_get_FM1R()
{
    return mem_get_reg32(CAN1_BASE_ADDRESS + CAN_FM1R_OFFSET);
}
static inline volatile uint32_t* can_get_FS1R()
{
    return mem_get_reg32(CAN1_BASE_ADDRESS + CAN_FS1R_OFFSET);
}
static inline volatile uint32_t* can_get_FFA1R()
{
    return mem_get_reg32(CAN1_BASE_ADDRESS + CAN_FFA1R_OFFSET);
}
static inline volatile uint32_t* can_get_FA1R()
{
    return mem_get_reg32(CAN1_BASE_ADDRESS + CAN_FA1R_OFFSET);
}
/** bank is 0-27 */
static inline volatile uint32_t* can_get_FxR1(uint32_t bank)
{
    return mem_get_reg32(
            CAN1_BASE_ADDRESS + CAN_F0R1_OFFSET + bank * CAN_FORx_OFFSET);
}
/** bank is 0-27 */
static inline volatile uint32_t* can_get_FxR2(uint32_t bank)
{
    return mem_get_reg32(
            CAN1_BASE_ADDRESS + CAN_F0R2_OFFSET + bank * CAN_FORx_OFFSET);
}

#endif /* CAN_REGISTERS_H_ */

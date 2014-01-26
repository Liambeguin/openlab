/*
 * This file is part of DronolabSTM32-embedded 
 *
 * DronolabSTM32-embedded is a student project designed 
 * for flying drones.
 *
 * Go to < dronolab.etsmtl.ca > to find out more about 
 * the project
 *
 *
 */

/*
 * lis302dl_regs.h
 *
 *  Created on: 2013-10-13
 *      Author: liam <liambeguin.at.gmail.com>
 */

#ifndef LIS302DL_REGS_H_
#define LIS302DL_REGS_H_

enum{
	LIS302DL_COM_DUMMY	= 0x00,
	LIS302DL_COM_READ	= 0x80,
	LIS302DL_COM_WRITE	= 0x00,
	LIS302DL_COM_MULTIP	= 0x40,
	LIS302DL_COM_S		= 0x00
};

enum{
	/* 0x00 to 0x0E reserved reg */
	LIS302DL_REGADDR_WHOAMI 		= 0x0F,
	/* 0x10 to 0x1F reserved reg */
	LIS302DL_REGADDR_CTRL1			= 0x20,
	LIS302DL_REGADDR_CTRL2			= 0x21,
	LIS302DL_REGADDR_CTRL3			= 0x22,
	LIS302DL_REGADDR_HP_FILTER_RST 	= 0x23,
	/* 0x24 to 0x26 reserved reg */
	LIS302DL_REGADDR_STATUS_REG		= 0x27,
	/* 0x28 not used */
	LIS302DL_REGADDR_OUT_X			= 0x29,
	/* 0x2A not used */
	LIS302DL_REGADDR_OUT_Y			= 0x2B,
	/* 0x2C not used */
	LIS302DL_REGADDR_OUT_Z			= 0x2D,
	/* 0x2E to 0x2F reserved reg */
	LIS302DL_REGADDR_FF_WU_CFG_1	= 0x30,
	LIS302DL_REGADDR_FF_WU_SRC_1	= 0x31,
	LIS302DL_REGADDR_FF_WU_THS_1	= 0x32,
	LIS302DL_REGADDR_FF_WU_DUR_1 	= 0x33,
	LIS302DL_REGADDR_FF_WU_CFG_2	= 0x34,
	LIS302DL_REGADDR_FF_WU_SRC_2	= 0x35,
	LIS302DL_REGADDR_FF_WU_THS_2	= 0x36,
	LIS302DL_REGADDR_FF_WU_DUR_2	= 0x37,
	LIS302DL_REGADDR_CLICK_CFG		= 0x38,
	LIS302DL_REGADDR_CLICK_SRC		= 0x39,
	/* 0x3A not used */
	LIS302DL_REGADDR_CLICK_THSY_X	= 0x3B,
	LIS302DL_REGADDR_CLICK_THSZ		= 0x3C,
	LIS302DL_REGADDR_CLICK_TimLimit = 0x3D,
	LIS302DL_REGADDR_CLICK_Latency	= 0x3E,
	LIS302DL_REGADDR_CLICK_Window	= 0x3F
};

enum{
	LIS302DL_CTRL1_XEN	= 0x00,
	LIS302DL_CTRL1_YEN	= 0x02,
	LIS302DL_CTRL1_ZEN	= 0x04,
	LIS302DL_CTRL1_STM 	= 0x08,
	LIS302DL_CTRL1_STP	= 0x10,
	LIS302DL_CTRL1_FS	= 0x20,
	LIS302DL_CTRL1_PD	= 0x40,
	LIS302DL_CTRL1_DR	= 0x80,
};

#endif /* LIS302DL_REGS_H_ */

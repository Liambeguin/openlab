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
 * tlc59116_regs.h
 *
 *  Created on: Jan 25, 2014
 *      Author: liam <liambeguin.at.gmail.com>
 */


//colors for 0 to 5
//printf("[\x1b[48;5;%dm   \x1B[48;0m]  \n", (16 + (Red * 36) + (Green * 6) + Blue));


#ifndef TLC59116_REGS_H_
#define TLC59116_REGS_H_

enum{
	TLC59116_REG_MODE1		= 0x00,
	TLC59116_REG_MODE2		= 0x01,
	/* */
	TLC59116_REG_PWM0		= 0x02,
	TLC59116_REG_PWM1		= 0x03,
	TLC59116_REG_PWM2		= 0x04,
	TLC59116_REG_PWM3		= 0x05,
	TLC59116_REG_PWM4		= 0x06,
	TLC59116_REG_PWM5		= 0x07,
	TLC59116_REG_PWM6		= 0x08,
	TLC59116_REG_PWM7		= 0x09,
	TLC59116_REG_PWM8		= 0x0A,
	TLC59116_REG_PWM9		= 0x0B,
	TLC59116_REG_PWM10		= 0x0C,
	TLC59116_REG_PWM11		= 0x0D,
	TLC59116_REG_PWM12		= 0x0E,
	TLC59116_REG_PWM13		= 0x0F,
	TLC59116_REG_PWM14		= 0x10,
	TLC59116_REG_PWM15		= 0x11,
	/* */
	TLC59116_REG_GRPPWM		= 0x12,
	TLC59116_REG_GRPFREQ	= 0x13,
	/* */
	TLC59116_REG_LEDOUT0	= 0x14,
	TLC59116_REG_LEDOUT1	= 0x15,
	TLC59116_REG_LEDOUT2	= 0x16,
	TLC59116_REG_LEDOUT3	= 0x17,
	TLC59116_REG_SUBADR1	= 0x18,
	TLC59116_REG_SUBADR2	= 0x19,
	TLC59116_REG_SUBADR3	= 0x1A,
	/* */
	TLC59116_REG_ALLCALLADR	= 0x1B,
	TLC59116_REG_IREF		= 0x1C,
	TLC59116_REG_EFLAG1		= 0x1D,
	TLC59116_REG_EFLAG2		= 0x1E,
};

enum{
	TLC59116_CTRL_AUTO_INC = 0x80,
};

enum{
	TLC59116_LEDOUT_OFF			= 0x00,
	TLC59116_LEDOUT_FULL_ON		= 0x01,
	TLC59116_LEDOUT_PWM			= 0x02,
	TLC59116_LEDOUT_PWM_GRPPGM	= 0x03,
	TLC59116_LEDOUT_ALL_PWM		= 0xAA,
};


#endif /* TLC59116_REGS_H_ */

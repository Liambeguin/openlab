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
 * lis302dl.c
 *
 *  Created on: 2013-10-13
 *      Author: liam
 */


#include <stdint.h>

#include "spi.h"
#include "gpio.h"

#include "lis302dl.h"
#include "lis302dl_.h"
#include "lis302dl_regs.h"

#include "soft_timer_delay.h"

#define NO_DEBUG_HEADER
#include "debug.h"


/*
 *  SPI : 4 wire interface CS, SPC, SDI, SDO
 *
 *  bit 0 		: (RW) 0 -> write on device, 1 -> read
 *  bit 1 		: (MS)
 *  bit [2: 7] 	: (AD) register address
 *  bit [8:15]	: (DO,DI) data
 */

static struct{
	spi_t spi;
	gpio_t cs_port;
	gpio_pin_t cs_pin;

}lis302dl;


//set chip select
inline void cs_set(){gpio_pin_set(lis302dl.cs_port, lis302dl.cs_pin);}
//clear chip select
inline void cs_clear(){gpio_pin_clear(lis302dl.cs_port, lis302dl.cs_pin);}

void lis302dl_config(spi_t spi,
		gpio_t port, gpio_pin_t pin)
{
	lis302dl.cs_port = port;
	lis302dl.cs_pin = pin;
	lis302dl.spi = spi;
}

void lis302dl_init(){

	uint8_t rx_buf =0x00, ctrl;

	//Setting CS pin
	gpio_enable(lis302dl.cs_port);
	gpio_set_output(lis302dl.cs_port, lis302dl.cs_pin);
	cs_set();

	ctrl = (uint8_t) (LIS302DL_CTRL1_PD | LIS302DL_CTRL1_XEN | LIS302DL_CTRL1_YEN | LIS302DL_CTRL1_ZEN);
	/* Write value to MEMS CTRL_REG1 regsister */
	lis302dl_write(&ctrl, LIS302DL_REGADDR_CTRL1, 1);

	/* Configure Interrupt control register: enable Click interrupt1 */
	ctrl = 0x07;
	lis302dl_write(&ctrl, LIS302DL_REGADDR_CTRL3, 1);

	/* Enable Interrupt generation on click/double click on Z axis */
	ctrl = 0x70;
	lis302dl_write(&ctrl, LIS302DL_REGADDR_CLICK_CFG, 1);

	/* Configure Click Threshold on X/Y axis (10 x 0.5g) */
	ctrl = 0xAA;
	lis302dl_write(&ctrl, LIS302DL_REGADDR_CLICK_THSY_X, 1);

	/* Configure Click Threshold on Z axis (10 x 0.5g) */
	ctrl = 0x0A;
	lis302dl_write(&ctrl, LIS302DL_REGADDR_CLICK_THSZ, 1);

	/* Configure Time Limit */
	ctrl = 0x03;
	lis302dl_write(&ctrl, LIS302DL_REGADDR_CLICK_TimLimit, 1);

	/* Configure Latency */
	ctrl = 0x7F;
	lis302dl_write(&ctrl, LIS302DL_REGADDR_CLICK_Latency, 1);

	/* Configure Click Window */
	ctrl = 0x7F;
	lis302dl_write(&ctrl, LIS302DL_REGADDR_CLICK_Window, 1);


	lis302dl_read(&rx_buf, LIS302DL_REGADDR_WHOAMI, 1);
	if(rx_buf == 0x3B)
		log_info("\t[LIS302DL] Initialized !!");
	else
		log_error("\t[LIS302DL] EROR unable to init !");
}


void lis302dl_write(uint8_t* data, uint8_t reg_addr, uint16_t length){
	/* Configure the MS bit:
       - When 0, the address will remain unchanged in multiple read/write commands.
       - When 1, the address will be auto incremented in multiple read/write commands.
	 */
	if(length > 0x01)
	{
		reg_addr |= (uint8_t)LIS302DL_COM_MULTIP;
	}
	/* Set chip select Low at the start of the transmission */
	cs_clear();

	/* Send the Address of the indexed register */
	spi_transfer_single(lis302dl.spi, reg_addr);

	/* Send the data that will be written into the device (MSB First) */
	while(length >= 0x01)
	{
		spi_transfer_single(lis302dl.spi, *data);
		length--;
		data++;
	}

	/* Set chip select High at the end of the transmission */
	cs_set();
}

void lis302dl_read(uint8_t* data, uint8_t reg_addr, uint16_t length)
{
	if(length > 0x01)
	{
		reg_addr |= (uint8_t)(LIS302DL_COM_READ | LIS302DL_COM_MULTIP);
	}
	else
	{
		reg_addr |= (uint8_t)LIS302DL_COM_READ;
	}
	/* Set chip select Low at the start of the transmission */
	cs_clear();

	/* Send the Address of the indexed register */
	spi_transfer_single(lis302dl.spi, reg_addr);

	/* Receive the data that will be read from the device (MSB First) */
	while(length > 0x00)
	{
		/* Send dummy byte (0x00) to generate the SPI clock to LIS302DL (Slave device) */
		*data = spi_transfer_single(lis302dl.spi, LIS302DL_COM_DUMMY);
		length--;
		data++;
	}

	/* Set chip select High at the end of the transmission */
	cs_set();
}

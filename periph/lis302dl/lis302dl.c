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
void cs_set(){
	gpio_pin_set(lis302dl.cs_port, lis302dl.cs_pin);
}

//clear chip select
void cs_clear(){
	gpio_pin_clear(lis302dl.cs_port, lis302dl.cs_pin);
}

void lis302dl_config(spi_t spi,
		gpio_t port, gpio_pin_t pin)
{
	lis302dl.cs_port = port;
	lis302dl.cs_pin = pin;
	lis302dl.spi = spi;
}
void lis302dl_init(){

	//enable port
	gpio_enable(lis302dl.cs_port);

	//set input/output
	gpio_set_output(lis302dl.cs_port, lis302dl.cs_pin);
	gpio_config_output_type(lis302dl.cs_port, lis302dl.cs_pin, GPIO_TYPE_PUSH_PULL);
	gpio_config_pull_up_down(lis302dl.cs_port, lis302dl.cs_pin, GPIO_PULL_UP);
//	gpio_set_speed(lis302dl.cs_port, lis302dl.cs_pin, GPIO_SPEED_HIGH);

	//set CS
	cs_set();

}



void lis302dl_read_whoami(){

	uint8_t data = 0;

	cs_clear();
	spi_transfer_single(lis302dl.spi, (LIS302DL_COM_WRITE | LIS302DL_REGADDR_CTRL2));
	spi_transfer_single(lis302dl.spi, 0x40);
	cs_set();

	cs_clear();
	spi_transfer_single(lis302dl.spi, (LIS302DL_COM_WRITE | LIS302DL_REGADDR_CTRL1));
	spi_transfer_single(lis302dl.spi, (LIS302DL_COM_WRITE | LIS302DL_CTRL1_PD | LIS302DL_CTRL1_ZEN | LIS302DL_CTRL1_YEN | LIS302DL_CTRL1_XEN));
	cs_set();

	soft_timer_delay_us(5);

	cs_clear();
	spi_transfer_single(lis302dl.spi, (LIS302DL_COM_READ | LIS302DL_REGADDR_WHOAMI));
	data = spi_transfer_single(lis302dl.spi, LIS302DL_COM_DUMMY);
	cs_set();

	log_warning("data : 0x%02x", data);

}

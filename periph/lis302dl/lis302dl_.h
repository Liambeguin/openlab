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
 * lis302dl_.h
 *
 *  Created on: 2013-10-13
 *      Author: liam <liambeguin.at.gmail.com>
 */

#ifndef LIS302DL__H_
#define LIS302DL__H_

//todo add param
void lis302dl_init();
void lis302dl_config(spi_t spi, gpio_t port, gpio_pin_t pin);


#endif /* LIS302DL__H_ */

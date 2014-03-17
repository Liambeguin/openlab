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
 * tlc59116.c
 *
 *  Created on: Jan 21, 2014
 *      Author: liam <liambeguin.at.gmail.com>
 */

#include <stdint.h>

#include "i2c.h"

#define NO_DEBUG_HEADER()
#include "debug.h"

#include "tlc59116.h"
#include "tlc59116_regs.h"

tlc59116_t _led_driver;


void tlc59116_config(i2c_t i2c, uint8_t addr){

	_led_driver.i2c		= i2c;
	_led_driver.addr	= addr;
}


void tlc59116_init(){

	uint8_t tx_buf[10] 	= {0};
	uint8_t rx_buf[4] 	= {0};
	uint8_t read_addr	= 0x00;


	tx_buf[0] = TLC59116_REG_MODE1 | TLC59116_CTRL_AUTO_INC;
	tx_buf[1] = 0x01;	// MODE1 value
	tx_buf[2] = 0x00;	// MODE2 value
	i2c_tx(_led_driver.i2c, _led_driver.addr, tx_buf, 3);

	// Read MODE1 and MODE2
//	read_addr = TLC59116_REG_MODE1 | TLC59116_CTRL_AUTO_INC;
//	i2c_tx_rx(_led_driver.i2c, _led_driver.addr, &read_addr, 1, rx_buf, 2);
//	log_not_implemented("MODE1 : [0x%02x]\tMODE2 : [0x%02x]", rx_buf[0], rx_buf[1]);


	//setting LED output state
	tx_buf[0] = TLC59116_REG_LEDOUT0 | TLC59116_CTRL_AUTO_INC;
	tx_buf[1] = TLC59116_LEDOUT_ALL_PWM;	// LEDOUT0
	tx_buf[2] = TLC59116_LEDOUT_ALL_PWM;	// LEDOUT1
	tx_buf[3] = TLC59116_LEDOUT_ALL_PWM;	// LEDOUT2
	tx_buf[4] = TLC59116_LEDOUT_ALL_PWM;	// LEDOUT3
	i2c_tx(_led_driver.i2c, _led_driver.addr, tx_buf, 5);

	//Make LED0 blink for init
	tlc59116_set_led_color(TLC59116_LED0, RGB_BLUE);

	read_addr = TLC59116_REG_EFLAG1 | TLC59116_CTRL_AUTO_INC;
	i2c_tx_rx(_led_driver.i2c, _led_driver.addr, &read_addr, 1, rx_buf, 2);
	if(rx_buf[0] || rx_buf[1]){
		log_error("[LED] ERROR");
	}
	else
		log_info("[LED] TLC59116 OK !!");

	tlc59116_set_led_color(TLC59116_LED0, RGB_OFF);
	tlc59116_set_led_color(TLC59116_LED1, RGB_OFF);
	tlc59116_set_led_color(TLC59116_LED2, RGB_OFF);
	tlc59116_set_led_color(TLC59116_LED3, RGB_OFF);
	tlc59116_set_led_color(TLC59116_LED4, RGB_OFF);
}

void tlc59116_get_led_color(tlc59116_led_t led/*, tlc59116_color_t *color*/){

	uint8_t read_addr = 0;
	uint8_t rx_buf[3];
	char name[6] = {'L', 'E', 'D', '_', 'X', '\0'};

	switch(led){
	case TLC59116_LED0:
		read_addr = TLC59116_REG_PWM0 | 0x80;
		name[4] = '0';
		break;
	case TLC59116_LED1:
		read_addr = TLC59116_REG_PWM3 | 0x80;
		name[4] = '1';
		break;
	case TLC59116_LED2:
		read_addr = TLC59116_REG_PWM6 | 0x80;
		name[4] = '2';
		break;
	case TLC59116_LED3:
		read_addr = TLC59116_REG_PWM9 | 0x80;
		name[4] = '3';
		break;
	case TLC59116_LED4:
		read_addr = TLC59116_REG_PWM13 | 0x80;
		name[4] = '4';
		break;
	default:
		log_error("undefined LED");
		break;
	}

	i2c_tx_rx(_led_driver.i2c, _led_driver.addr, &read_addr, 1, rx_buf, 4);

	/*
	color->red 		= rx_buf[0];
	color->green 	= rx_buf[1];
	color->blue		= rx_buf[2];
	*/

	if((16 + (rx_buf[0]/51 * 36) + (rx_buf[1]/51 * 6) + rx_buf[2]/51)==16)
		printf("%s is  OFF  \n", name);
	else
	printf("%s is \x1b[48;5;%dm     \x1B[48;0m  \n", name,
			(16 + (rx_buf[0]/51 * 36) + (rx_buf[1]/51 * 6) + rx_buf[2]/51));


}

void tlc59116_set_led_color(tlc59116_led_t led, tlc59116_color_t color){

	uint8_t tmp[4] = {0x00, color.green, color.red, color.blue};
	char name[6] = {'L', 'E', 'D', '_', 'X', '\0'};

	switch(led){
	case TLC59116_LED0:
		tmp[0] = TLC59116_REG_PWM0 | 0x80;
		name[4] = '0';
		break;
	case TLC59116_LED1:
		tmp[0] = TLC59116_REG_PWM3 | 0x80;
		name[4] = '1';
		break;
	case TLC59116_LED2:
		tmp[0] = TLC59116_REG_PWM6 | 0x80;
		name[4] = '2';
		break;
	case TLC59116_LED3:
		tmp[0] = TLC59116_REG_PWM9 | 0x80;
		name[4] = '3';
		break;
	case TLC59116_LED4:
		tmp[0] = TLC59116_REG_PWM13 | 0x80;
		name[4] = '4';
		break;
	default:
		log_error("undefined LED");
		break;
	}

	i2c_tx(_led_driver.i2c, _led_driver.addr, &tmp, 4);

	//printing color of led
//	if((16 + (red/51 * 36) + (green/51 * 6) + blue/51)==16)
//			printf("%s is  OFF  \n", name);
//		else
//		printf("%s set to \x1b[48;5;%dm     \x1B[48;0m  \n", name,
//				(16 + (red/51 * 36) + (green/51 * 6) + blue/51));

}

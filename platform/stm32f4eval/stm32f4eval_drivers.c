/*

 * This file is part of HiKoB Openlab.
 *
 * HiKoB Openlab is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, version 3.
 *
 * HiKoB Openlab is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with HiKoB Openlab. If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Copyright (C) 2011,2012 HiKoB.
 */

/*
 * stm32f4eval_drivers.c
 *
 *  Created on: Sep 17, 2012
 *      Author: Clément Burin des Roziers <clement.burin-des-roziers.at.hikob.com>
 *      Author: liam <liambeguin.at.gmail.com>
 *
 */

#include "stm32f4eval.h"
#include "debug.h"

#include "rcc.h"
#include "nvic_.h"

#include "uart_.h"
#include "spi_.h"
#include "gpio_.h"
#include "timer_.h"
#include "ethmac_.h"
#include "exti_.h"
#include "exti.h"

#define I2C_ADDR (uint8_t)0x94



#define USART2_GPIO_PORT GPIO_A
#define USART2_GPIO_TX GPIO_PIN_2
#define USART2_GPIO_RX GPIO_PIN_3
/* USART pins : PC10(yellow), PC11(orange) */

static void gpio_drivers_setup();
static void uart_drivers_setup();
static void timer_drivers_setup();
static void ethmac_drivers_setup();
static void spi_drivers_setup();
static void i2c_drivers_setup();

void platform_drivers_setup()
{
    gpio_drivers_setup();
    uart_drivers_setup();
    timer_drivers_setup();

//    spi_drivers_setup();
    i2c_drivers_setup();

    exti_set_handler(EXTI_LINE_Px0, NULL, NULL);

//    ethmac_drivers_setup();
}

static void i2c_drivers_setup(){


	gpio_set_output(GPIO_D, GPIO_PIN_4);
	gpio_pin_set(GPIO_D, GPIO_PIN_4);

	//I2C 2 config for Audio driver
	gpio_set_i2c_sda(GPIO_B, GPIO_PIN_9);
	gpio_set_i2c_scl(GPIO_B, GPIO_PIN_6);

	i2c_enable(I2C_1, I2C_CLOCK_MODE_STANDARD);

	//read from addr 0x01 and auto increment
	uint8_t reg = 0x81;
//	uint8_t tmp[10] = {0};

//	i2c_tx(I2C_1, I2C_ADDR, &reg, 1);
//	i2c_rx(I2C_1, I2C_ADDR, tmp, 4);

//	i2c_tx_rx(I2C_1, I2C_ADDR, &reg, 1, &tmp, 1);

//	log_debug("I2C OK !! [0x%02x][0x%02x][0x%02x][0x%02x]", tmp[0], tmp[1], tmp[2], tmp[3]);
}

static void spi_drivers_setup()
{
	//LIS302DL accelerometer config
	gpio_set_spi_clk(GPIO_A, GPIO_PIN_5);
	gpio_set_spi_miso(GPIO_A, GPIO_PIN_6);
	gpio_set_spi_mosi(GPIO_A, GPIO_PIN_7);
	spi_enable(SPI_1, 4000000, SPI_CLOCK_MODE_IDLE_HIGH_FALLING);
}

static void gpio_drivers_setup()
{
    gpio_enable(GPIO_A);
    gpio_enable(GPIO_B);
    gpio_enable(GPIO_C);
    gpio_enable(GPIO_D);
    gpio_enable(GPIO_E);
    gpio_enable(GPIO_F);
    gpio_enable(GPIO_G);
    gpio_enable(GPIO_H);
    gpio_enable(GPIO_I);
}

/* UART declaration */
uart_t uart_print = UART_3;

static void uart_drivers_setup()
{
    // Enable the print uart
    gpio_set_uart_tx(GPIO_C, GPIO_PIN_10);
    gpio_set_uart_rx(GPIO_C, GPIO_PIN_11);
    uart_enable(uart_print, 500000);
}

void usart3_isr()
{
    uart_handle_interrupt(UART_3);
}

static void timer_drivers_setup()
{
    // Configure the General Purpose Timers
	timer_enable(TIM_1);

	//TIM_2 used for soft timer in <platform>_lib.c
    timer_enable(TIM_2);

    timer_enable(TIM_3);
    timer_enable(TIM_4);

    // Select the clocks for all timers
    timer_select_internal_clock(TIM_1, 0);
    timer_select_internal_clock(TIM_2, 0);
    timer_select_internal_clock(TIM_3, 0);
    timer_select_internal_clock(TIM_4, 0);

    // Start timer 2 for soft_timer
    timer_select_internal_clock(TIM_1,
    		(rcc_sysclk_get_clock_frequency(RCC_SYSCLK_CLOCK_PCLK1_TIM) / 262500)
    		- 1);

    // Start timer 2 for soft_timer
    timer_select_internal_clock(TIM_2,
            (rcc_sysclk_get_clock_frequency(RCC_SYSCLK_CLOCK_PCLK1_TIM) / 262500)
                    - 1);

    timer_select_internal_clock(TIM_3,
            (rcc_sysclk_get_clock_frequency(RCC_SYSCLK_CLOCK_PCLK1_TIM) / 32768)
                    - 1);

    //start timer 4 for motor control
    timer_select_internal_clock(TIM_4,
            (rcc_sysclk_get_clock_frequency(RCC_SYSCLK_CLOCK_PCLK1_TIM) / 262500)
                    - 1);

    // Start ALL PWM and other timers
    timer_start(TIM_1, 0xFFFF, NULL, NULL);
    timer_start(TIM_2, 0xFFFF, NULL, NULL);
    timer_start(TIM_3, 0xFFFF, NULL, NULL);
    timer_start(TIM_4, 0xFFFF, NULL, NULL);
}




void tim1_brk_tim9_isr()
{
    timer_handle_interrupt(TIM_1);
}
void tim1_up_tim10_isr()
{
    timer_handle_interrupt(TIM_1);
}
void tim1_trg_com_tim11_isr()
{
    timer_handle_interrupt(TIM_1);
}
void tim1_cc_isr()
{
    timer_handle_interrupt(TIM_1);
}

void tim2_isr()
{
    timer_handle_interrupt(TIM_2);
}
void tim3_isr()
{
    timer_handle_interrupt(TIM_3);
}
void tim4_isr()
{
    timer_handle_interrupt(TIM_4);
}

static void ethmac_drivers_setup()
{
    int i;

    // Set all GPIO AF11
    uint8_t A[] =
    { 1, 2, 7 };
    for (i = 0; i < sizeof(A); i++)
    {
        gpio_set_alternate_function(GPIO_A, A[i], GPIO_AF_11);
        gpio_config_output_type(GPIO_A, A[i], GPIO_TYPE_PUSH_PULL);
        gpio_set_speed(GPIO_A, A[i], GPIO_SPEED_HIGH);
    }

    uint8_t B[] =
    { 5, 8};
    for (i = 0; i < sizeof(B); i++)
    {
        gpio_set_alternate_function(GPIO_B, B[i], GPIO_AF_11);
        gpio_config_output_type(GPIO_B, B[i], GPIO_TYPE_PUSH_PULL);
        gpio_set_speed(GPIO_B, B[i], GPIO_SPEED_HIGH);
    }

    uint8_t C[] =
    { 1, 2, 3, 4, 5 };
    for (i = 0; i < sizeof(C); i++)
    {
        gpio_set_alternate_function(GPIO_C, C[i], GPIO_AF_11);
        gpio_config_output_type(GPIO_C, C[i], GPIO_TYPE_PUSH_PULL);
        gpio_set_speed(GPIO_C, C[i], GPIO_SPEED_HIGH);
    }

    gpio_set_alternate_function(GPIO_E, GPIO_PIN_2, GPIO_AF_11);

    uint8_t G[] =
    { 11, 13, 14 };
    for (i = 0; i < sizeof(G); i++)
    {
        gpio_set_alternate_function(GPIO_G, G[i], GPIO_AF_11);
        gpio_config_output_type(GPIO_G, G[i], GPIO_TYPE_PUSH_PULL);
        gpio_set_speed(GPIO_G, G[i], GPIO_SPEED_HIGH);
    }

    uint8_t H[] =
    { 2, 3, 6, 7 };
    for (i = 0; i < sizeof(H); i++)
    {
        gpio_set_alternate_function(GPIO_H, H[i], GPIO_AF_11);
        gpio_config_output_type(GPIO_H, H[i], GPIO_TYPE_PUSH_PULL);
        gpio_set_speed(GPIO_H, H[i], GPIO_SPEED_HIGH);
    }

    uint8_t I[] =
    { 10 };
    for (i = 0; i < sizeof(I); i++)
    {
        gpio_set_alternate_function(GPIO_I, I[i], GPIO_AF_11);
        gpio_config_output_type(GPIO_H, I[i], GPIO_TYPE_PUSH_PULL);
        gpio_set_speed(GPIO_I, I[i], GPIO_SPEED_HIGH);
    }

    ethmac_init(ETHMAC_MODE_MII, 0);
}
void exti0_isr()
{
    exti_handle_interrupt(EXTI_LINE_Px0);
}
void exti1_isr()
{
    exti_handle_interrupt(EXTI_LINE_Px1);
}
void exti2_isr()
{
    exti_handle_interrupt(EXTI_LINE_Px2);
}
void exti3_isr()
{
    exti_handle_interrupt(EXTI_LINE_Px3);
}
void exti4_isr()
{
    exti_handle_interrupt(EXTI_LINE_Px4);
}
void exti9_5_isr()
{
    exti_handle_9_5_interrupt();
}
void exti15_10_isr()
{
    exti_handle_15_10_interrupt();
}
void i2c1_ev_isr()
{
    i2c_handle_ev_interrupt(I2C_1);
}

void i2c1_er_isr()
{
    i2c_handle_er_interrupt(I2C_1);
}
void i2c2_ev_isr()
{
    i2c_handle_ev_interrupt(I2C_2);
}

void i2c2_er_isr()
{
    i2c_handle_er_interrupt(I2C_2);
}

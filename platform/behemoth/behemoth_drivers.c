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
 * behemoth_drivers.c
 *
 *  Created on: Jan 16, 2014
 *      Author: Clément Burin des Roziers <clement.burin-des-roziers.at.hikob.com>
 *      Author: liam <liambeguin.at.gmail.com>
 *
 */

#include "behemoth.h"
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
#include "i2c_.h"


#define USART2_GPIO_PORT GPIO_A
#define USART2_GPIO_TX GPIO_PIN_2
#define USART2_GPIO_RX GPIO_PIN_3
/* USART pins : PC10(yellow), PC11(orange) */

static void gpio_drivers_setup();
static void uart_drivers_setup();
static void timer_drivers_setup();
static void spi_drivers_setup();
static void i2c_drivers_setup();

void platform_drivers_setup()
{
    gpio_drivers_setup();
    uart_drivers_setup();
    timer_drivers_setup();

    i2c_drivers_setup();
    spi_drivers_setup();
}

static void i2c_drivers_setup(){

	//I2C 2 config for LED Driver
	gpio_set_i2c_sda(GPIO_B, GPIO_PIN_11);
	gpio_set_i2c_scl(GPIO_B, GPIO_PIN_10);
	i2c_enable(I2C_2, I2C_CLOCK_MODE_FAST);
}

static void spi_drivers_setup()
{
	//TODO
	//SPI3 config Memory
//	gpio_set_spi_clk(GPIO_B, GPIO_PIN_3);
//	gpio_set_spi_miso(GPIO_B, GPIO_PIN_4);
//	gpio_set_spi_mosi(GPIO_B, GPIO_PIN_5);
//	spi_enable(SPI_3, 4000000, SPI_CLOCK_MODE_IDLE_LOW_RISING);

}

static void gpio_drivers_setup()
{
    // Set base address and AHB bit for all GPIO ports
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
uart_t uart_print = UART_2;
static void uart_drivers_setup()
{
    // Enable the print uart
    gpio_set_uart_tx(GPIO_D, GPIO_PIN_5);
    gpio_set_uart_rx(GPIO_D, GPIO_PIN_6);
    uart_enable(uart_print, 500000);
}
void usart2_isr()
{
    uart_handle_interrupt(UART_2);
}

//GX3
void usart3_isr()
{
    uart_handle_interrupt(UART_3);
}

static void timer_drivers_setup()
{
    // Configure the General Purpose Timers
    timer_enable(TIM_1);
    timer_enable(TIM_2);
    timer_enable(TIM_3);

    // Select the clocks for all timers
    timer_select_internal_clock(TIM_1, 0);
    timer_select_internal_clock(TIM_2, 0);
    timer_select_internal_clock(TIM_3, 0);

    //start timer 4 for motor control
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


    // Start ALL PWM and other timers
    timer_start(TIM_1, 0xFFFF, NULL, NULL);
    timer_start(TIM_2, 0xFFFF, NULL, NULL);
    timer_start(TIM_3, 0xFFFF, NULL, NULL);
}

/* Timer 1 interrupt Service Registers */
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
/*   ***   */

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
void i2c2_ev_isr()
{
    i2c_handle_ev_interrupt(I2C_2);
}

void i2c2_er_isr()
{
    i2c_handle_er_interrupt(I2C_2);
}

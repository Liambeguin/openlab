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
 * boreas_drivers.c
 *
 *  Created on: Aug 01, 2014
 *      Author: Clément Burin des Roziers <clement.burin-des-roziers.at.hikob.com>
 *      Author: liam <liambeguin.at.gmail.com>
 *
 */

#include "boreas.h"
#include "debug.h"
#include "boot_log.h"

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
#include "can_.h"

/** GPIO Section **/
static void gpio_drivers_setup();

/** UART Section **/
#define UART1_PRINT_BAUDRATE	115200
#define VN200_BAUDRATE		460800
#define DATALINK_BAUDRATE	  9600
static void uart_drivers_setup();

/** TIMER Section **/
//#define TIM2_FREQUENCY	262144
static void timer_drivers_setup();

/** SPI Section **/
static void spi_drivers_setup();

/** I2C Section **/
#define I2C1_CLOCK_MODE	I2C_CLOCK_MODE_FAST
static void i2c_drivers_setup();

static void can_drivers_setup();
can_t can = CAN_2;


void platform_drivers_setup()
{
	gpio_drivers_setup();
	uart_drivers_setup();
	timer_drivers_setup();

	i2c_drivers_setup();
	spi_drivers_setup();
	can_drivers_setup();
}

static void i2c_drivers_setup(){

	/** I2C_2 configuration for RGB LED Driver **/
	gpio_set_i2c_scl(GPIO_B, GPIO_PIN_10);
	gpio_set_i2c_sda(GPIO_B, GPIO_PIN_11);
	i2c_enable(I2C_2, I2C_CLOCK_MODE_FAST);
	boot_success("I2C_2 enabled\n");
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



static void spi_drivers_setup()
{
	//SPI1 to extern
	gpio_set_spi_clk(GPIO_A,  GPIO_PIN_5);
	gpio_set_spi_miso(GPIO_A, GPIO_PIN_6);
	gpio_set_spi_mosi(GPIO_A, GPIO_PIN_7);
	spi_enable(SPI_1, 4000000, SPI_CLOCK_MODE_IDLE_LOW_RISING);
	boot_success("External SPI enabled at 4000000\n");

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
uart_t uart_print = UART_1;
uart_t uart_external = UART_2;
static void uart_drivers_setup()
{
	/** Enable print UART1 **/
	gpio_set_uart_tx(GPIO_B, GPIO_PIN_6);
	gpio_set_uart_rx(GPIO_B, GPIO_PIN_7);
	uart_enable(UART_1, UART1_PRINT_BAUDRATE);
	boot_success("Starting UART_1 at %d\n", UART1_PRINT_BAUDRATE);
/* testing */
	gpio_set_uart_tx(GPIO_A, GPIO_PIN_2);
	gpio_set_uart_rx(GPIO_A, GPIO_PIN_3);
	uart_enable(UART_2,VN200_BAUDRATE);
	boot_success("Starting UART_2 at %d\n", VN200_BAUDRATE);

	gpio_set_uart_tx(GPIO_C, GPIO_PIN_10);
	gpio_set_uart_rx(GPIO_C, GPIO_PIN_11);
	uart_enable(UART_3, DATALINK_BAUDRATE);
	boot_success("Starting UART_3 at %d\n", DATALINK_BAUDRATE);


	gpio_set_uart_tx(GPIO_A, GPIO_PIN_0);
	gpio_set_uart_rx(GPIO_A, GPIO_PIN_1);
	uart_enable(UART_4, VN200_BAUDRATE);
	boot_success("Starting UART_4 at %d\n", VN200_BAUDRATE);

	gpio_set_uart_tx(GPIO_C, GPIO_PIN_12);
	gpio_set_uart_rx(GPIO_D, GPIO_PIN_2);
	uart_enable(UART_5, VN200_BAUDRATE);
	boot_success("Starting UART_5 at %d\n",VN200_BAUDRATE);
}
//print
void usart1_isr()
{
    uart_handle_interrupt(UART_1);
}
//
void usart2_isr()
{
    uart_handle_interrupt(UART_2);
}
//
void usart3_isr()
{
    uart_handle_interrupt(UART_3);
}
//
void uart4_isr()
{
    uart_handle_interrupt(UART_4);
}
//
void uart5_isr()
{
    uart_handle_interrupt(UART_5);
}

static void timer_drivers_setup()
{
    // Enable Motor timer
    timer_enable(TIM_1);
    timer_enable(TIM_8);

    // Enable Soft timer
    timer_enable(TIM_2);


    // Setting Motor timer to about 2MHz (2^21) = 2097152
    timer_select_internal_clock(TIM_1,
            (rcc_sysclk_get_clock_frequency(RCC_SYSCLK_CLOCK_PCLK1_TIM) / 2097152)
                    - 1);

   boot_success("Starting TIM_1 at about 2MHz\n");

    // Setting servo timer to about 30Hz (2^5) = 32
    // Setting Motor timer to about 2MHz (2^21) = 2097152
    timer_select_internal_clock(TIM_8,
            (rcc_sysclk_get_clock_frequency(RCC_SYSCLK_CLOCK_PCLK1_TIM) / 2097152)
                    - 1);

   boot_success("Starting TIM_8 at about 30Hz\n");

    // Setting Soft timer to about 32kHz
    timer_select_internal_clock(TIM_2,
            (rcc_sysclk_get_clock_frequency(RCC_SYSCLK_CLOCK_PCLK1_TIM) / 2097152)//32768)
                    - 1);
   boot_success("Starting TIM_2 at about 32kHz\n");

    // Start ALL timers
    timer_start(TIM_1, 0xFFFF, NULL, NULL, TIMER_MODE_CLOCK);
    timer_start(TIM_2, 0xFFFF, NULL, NULL, TIMER_MODE_CLOCK);
    timer_start(TIM_8, 0xFFFF, NULL, NULL, TIMER_MODE_CLOCK);


    timer_set_channel_compare(TIM_1, TIMER_CHANNEL_1, 0x00FF, NULL, NULL);
    timer_set_channel_compare(TIM_8, TIMER_CHANNEL_1, 0x00FF, NULL, NULL);
}

void tim1_up_tim10_isr()
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
void tim8_up_tim13_isr()
{
    timer_handle_interrupt(TIM_8);
}
void tim8_cc_isr()
{
    timer_handle_interrupt(TIM_8);
}

static void can_drivers_setup()
{
    // Configure the CAN PB12/13
    gpio_set_can_rx(GPIO_B, GPIO_PIN_12);
    gpio_set_can_tx(GPIO_B, GPIO_PIN_13);

    // Start it
    can_enable(CAN_2, 1000000);
}

void can2_tx_isr()
{
    can_handle_tx_interrupt(CAN_2);
}
void can2_rx0_isr()
{
    can_handle_rx0_interrupt(CAN_2);
}
void can2_rx1_isr()
{
    can_handle_rx1_interrupt(CAN_2);
}
void can2_sce_isr()
{
    can_handle_sce_interrupt(CAN_2);
}

/** External interrupts for RC receiver**/
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


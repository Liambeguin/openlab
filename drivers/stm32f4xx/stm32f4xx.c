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
 * stm32f4xx.c
 *
 *  Created on: Nov 6, 2012
 *      Author: Clément Burin des Roziers <clement.burin-des-roziers.at.hikob.com>
 */

#include "stm32f4xx.h"

#include "nvic_.h"

/* All GPIOs */
GPIO_INIT(_gpioA, GPIOA_BASE_ADDRESS, RCC_AHB_BIT_GPIOA);
GPIO_INIT(_gpioB, GPIOB_BASE_ADDRESS, RCC_AHB_BIT_GPIOB);
GPIO_INIT(_gpioC, GPIOC_BASE_ADDRESS, RCC_AHB_BIT_GPIOC);
GPIO_INIT(_gpioD, GPIOD_BASE_ADDRESS, RCC_AHB_BIT_GPIOD);
GPIO_INIT(_gpioE, GPIOE_BASE_ADDRESS, RCC_AHB_BIT_GPIOE);
GPIO_INIT(_gpioF, GPIOF_BASE_ADDRESS, RCC_AHB_BIT_GPIOF);
GPIO_INIT(_gpioG, GPIOG_BASE_ADDRESS, RCC_AHB_BIT_GPIOG);
GPIO_INIT(_gpioH, GPIOH_BASE_ADDRESS, RCC_AHB_BIT_GPIOH);
GPIO_INIT(_gpioI, GPIOI_BASE_ADDRESS, RCC_AHB_BIT_GPIOI);

/* Real I2C */
I2C_INIT(_i2c1, I2C1_BASE_ADDRESS, RCC_APB_BUS_I2C1, RCC_APB_BIT_I2C1,
        NVIC_IRQ_LINE_I2C1_EV, NVIC_IRQ_LINE_I2C1_ER);
I2C_INIT(_i2c2, I2C2_BASE_ADDRESS, RCC_APB_BUS_I2C2, RCC_APB_BIT_I2C2,
        NVIC_IRQ_LINE_I2C2_EV, NVIC_IRQ_LINE_I2C2_ER);

/* Real SPIs */
SPI_INIT(_spi1, SPI1_BASE_ADDRESS, RCC_APB_BUS_SPI1, RCC_APB_BIT_SPI1, NVIC_IRQ_LINE_SPI1);
SPI_INIT(_spi2, SPI2_BASE_ADDRESS, RCC_APB_BUS_SPI2, RCC_APB_BIT_SPI2, NVIC_IRQ_LINE_SPI2);
SPI_INIT(_spi3, SPI3_BASE_ADDRESS, RCC_APB_BUS_SPI3, RCC_APB_BIT_SPI3, NVIC_IRQ_LINE_SPI3);

/* Real Timers */
TIMER_INIT(_tim1, TIM1_BASE_ADDRESS, RCC_APB_BUS_TIM1, RCC_APB_BIT_TIM1, NVIC_IRQ_LINE_TIM1_UP_TIM10, 4);
TIMER_INIT(_tim2, TIM2_BASE_ADDRESS, RCC_APB_BUS_TIM2, RCC_APB_BIT_TIM2, NVIC_IRQ_LINE_TIM2, 4);
TIMER_INIT(_tim3, TIM3_BASE_ADDRESS, RCC_APB_BUS_TIM3, RCC_APB_BIT_TIM3, NVIC_IRQ_LINE_TIM3, 4);
TIMER_INIT(_tim4, TIM4_BASE_ADDRESS, RCC_APB_BUS_TIM4, RCC_APB_BIT_TIM4, NVIC_IRQ_LINE_TIM4, 4);
TIMER_INIT(_tim5, TIM5_BASE_ADDRESS, RCC_APB_BUS_TIM5, RCC_APB_BIT_TIM5, NVIC_IRQ_LINE_TIM5, 4);

TIMER_INIT(_tim6, TIM6_BASE_ADDRESS, RCC_APB_BUS_TIM6, RCC_APB_BIT_TIM6, NVIC_IRQ_LINE_TIM6_DAC, 0);
TIMER_INIT(_tim7, TIM7_BASE_ADDRESS, RCC_APB_BUS_TIM7, RCC_APB_BIT_TIM7, NVIC_IRQ_LINE_TIM7, 0);

TIMER_INIT(_tim8, TIM8_BASE_ADDRESS, RCC_APB_BUS_TIM8, RCC_APB_BIT_TIM8, NVIC_IRQ_LINE_TIM8_CC, 4);

/* Real UARTs */
UART_INIT(_uart1, USART1_BASE_ADDRESS, RCC_APB_BUS_USART1,
        RCC_APB_BIT_USART1, NVIC_IRQ_LINE_USART1);
UART_INIT(_uart2, USART2_BASE_ADDRESS, RCC_APB_BUS_USART2,
        RCC_APB_BIT_USART2, NVIC_IRQ_LINE_USART2);
UART_INIT(_uart3, USART3_BASE_ADDRESS, RCC_APB_BUS_USART3,
        RCC_APB_BIT_USART3, NVIC_IRQ_LINE_USART3);

/* DMA STREAMS */
DMA_INIT(_dma1_str0, DMA1_BASE_ADDRESS, RCC_AHB_BIT_DMA1, 0, NVIC_IRQ_LINE_DMA1_STR0);
DMA_INIT(_dma1_str1, DMA1_BASE_ADDRESS, RCC_AHB_BIT_DMA1, 1, NVIC_IRQ_LINE_DMA1_STR1);
DMA_INIT(_dma1_str2, DMA1_BASE_ADDRESS, RCC_AHB_BIT_DMA1, 2, NVIC_IRQ_LINE_DMA1_STR2);
DMA_INIT(_dma1_str3, DMA1_BASE_ADDRESS, RCC_AHB_BIT_DMA1, 3, NVIC_IRQ_LINE_DMA1_STR3);
DMA_INIT(_dma1_str4, DMA1_BASE_ADDRESS, RCC_AHB_BIT_DMA1, 4, NVIC_IRQ_LINE_DMA1_STR4);
DMA_INIT(_dma1_str5, DMA1_BASE_ADDRESS, RCC_AHB_BIT_DMA1, 5, NVIC_IRQ_LINE_DMA1_STR5);
DMA_INIT(_dma1_str6, DMA1_BASE_ADDRESS, RCC_AHB_BIT_DMA1, 6, NVIC_IRQ_LINE_DMA1_STR6);
DMA_INIT(_dma1_str7, DMA1_BASE_ADDRESS, RCC_AHB_BIT_DMA1, 7, NVIC_IRQ_LINE_DMA1_STR7);
DMA_INIT(_dma2_str0, DMA2_BASE_ADDRESS, RCC_AHB_BIT_DMA2, 0, NVIC_IRQ_LINE_DMA2_STR0);
DMA_INIT(_dma2_str1, DMA2_BASE_ADDRESS, RCC_AHB_BIT_DMA2, 1, NVIC_IRQ_LINE_DMA2_STR1);
DMA_INIT(_dma2_str2, DMA2_BASE_ADDRESS, RCC_AHB_BIT_DMA2, 2, NVIC_IRQ_LINE_DMA2_STR2);
DMA_INIT(_dma2_str3, DMA2_BASE_ADDRESS, RCC_AHB_BIT_DMA2, 3, NVIC_IRQ_LINE_DMA2_STR3);
DMA_INIT(_dma2_str4, DMA2_BASE_ADDRESS, RCC_AHB_BIT_DMA2, 4, NVIC_IRQ_LINE_DMA2_STR4);
DMA_INIT(_dma2_str5, DMA2_BASE_ADDRESS, RCC_AHB_BIT_DMA2, 5, NVIC_IRQ_LINE_DMA2_STR5);
DMA_INIT(_dma2_str6, DMA2_BASE_ADDRESS, RCC_AHB_BIT_DMA2, 6, NVIC_IRQ_LINE_DMA2_STR6);
DMA_INIT(_dma2_str7, DMA2_BASE_ADDRESS, RCC_AHB_BIT_DMA2, 7, NVIC_IRQ_LINE_DMA2_STR7);

/* Definitions for user access */
const gpio_t gpioA = GPIO_A;
const gpio_t gpioB = GPIO_B;
const gpio_t gpioC = GPIO_C;
const gpio_t gpioD = GPIO_D;
const gpio_t gpioE = GPIO_E;
const gpio_t gpioF = GPIO_F;
const gpio_t gpioG = GPIO_G;
const gpio_t gpioH = GPIO_H;
const gpio_t gpioI = GPIO_I;

const i2c_t i2c1 = I2C_1;
const i2c_t i2c2 = I2C_2;

const spi_t spi1 = SPI_1;
const spi_t spi2 = SPI_2;

const uart_t uart1 = UART_1;
const uart_t uart2 = UART_2;
const uart_t uart3 = UART_3;

const openlab_timer_t tim1 = TIM_1;
const openlab_timer_t tim2 = TIM_2;
const openlab_timer_t tim3 = TIM_3;
const openlab_timer_t tim4 = TIM_4;
const openlab_timer_t tim5 = TIM_5;
const openlab_timer_t tim6 = TIM_6;
const openlab_timer_t tim7 = TIM_7;
const openlab_timer_t tim8 = TIM_8;

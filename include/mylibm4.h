/******************************************************************************
 * @file     mylibm4.h
 * @brief    STM32F407xx Device Peripheral Access Layer Header File
 * @author   Bugrahan OZTURK
 * @version  V1
 * @date     31. May 2020
 ******************************************************************************/
/*
 * Copyright (c) 2020 Bugrahan OZTURK. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef MYLIBM4_H
#define MYLIBM4_H

/*************************************************
* #INCLUDES
*************************************************/
#include<stdint.h>

// Define the base addresses for relevant peripherals.
// You can calculate the addresses relative to periph_base,
// or directly assign them
// Base addresses can be found on page 64 in RM0090
#define PERIPH_BASE     ((uint32_t) 0x40000000)
#define AHB1PERIPH_BASE (PERIPH_BASE + 0x00020000)
#define GPIOA_BASE      (PERIPH_BASE + 0x20000) // GPIOA base address is 0x40020000
#define GPIOB_BASE      (PERIPH_BASE + 0x20400) // GPIOB base address is 0x40020400
#define GPIOC_BASE      (PERIPH_BASE + 0x20800) // GPIOC base address is 0x40020800
#define GPIOD_BASE      (PERIPH_BASE + 0x20C00) // GPIOD base address is 0x40020C00
#define RCC_BASE        (PERIPH_BASE + 0x23800) // RCC base address is 0x40023800
#define USART2_BASE     (PERIPH_BASE + 0x4400)  // USART2 base address is 0x40004400
#define USART4_BASE 	(PERIPH_BASE + 0x4C00)  // USART4 base address is 0x40004C00
#define FLASH_BASE      (AHB1PERIPH_BASE + 0x3C00) // FLASH base address
#define PWR_BASE        (PERIPH_BASE + 0x7000U) // Power Control base address

/*
 * rm0090 is the document for STM32F405/407 MCUs and at the end of each section, include
 * the register list in order (e.g. RCC, GPIO).
 *
 * Each of these items should also be guarded with volatile for optimization misbehavior
 */

/*
 * Reset and Clock Control (RCC) module
 *   page 267 from rm0090
 */
typedef struct {
	volatile uint32_t CR;         /* Address offset: 0x00 */
	volatile uint32_t PLLCFGR;    /* Address offset: 0x04 */
	volatile uint32_t CFGR;       /* Address offset: 0x08 */
	volatile uint32_t CIR;        /* Address offset: 0x0C */
	volatile uint32_t AHB1RSTR;   /* Address offset: 0x10 */
	volatile uint32_t AHB2RSTR;   /* Address offset: 0x14 */
	volatile uint32_t AHB3RSTR;   /* Address offset: 0x18 */
	volatile uint32_t RESERVED1;  /* Address offset: 0x1C */
	volatile uint32_t APB1RSTR;   /* Address offset: 0x20 */
	volatile uint32_t APB2RSTR;   /* Address offset: 0x24 */
	volatile uint32_t RESERVED2;  /* Address offset: 0x28 */
	volatile uint32_t RESERVED3;  /* Address offset: 0x2C */
	volatile uint32_t AHB1ENR;    /* Address offset: 0x30 */
	volatile uint32_t AHB2ENR;    /* Address offset: 0x34 */
	volatile uint32_t AHB3ENR;    /* Address offset: 0x38 */
	volatile uint32_t RESERVED4;  /* Address offset: 0x3C */
	volatile uint32_t APB1ENR;    /* Address offset: 0x40 */
	volatile uint32_t APB2ENR;    /* Address offset: 0x44 */
	volatile uint32_t RESERVED5;  /* Address offset: 0x48 */
	volatile uint32_t RESERVED6;  /* Address offset: 0x4C */
	volatile uint32_t AHB1LPENR;  /* Address offset: 0x50 */
	volatile uint32_t AHB2LPENR;  /* Address offset: 0x54 */
	volatile uint32_t AHB3LPENR;  /* Address offset: 0x58 */
	volatile uint32_t RESERVED7;  /* Address offset: 0x5C */
	volatile uint32_t APB1LPENR;  /* Address offset: 0x60 */
	volatile uint32_t APB2LPENR;  /* Address offset: 0x64 */
	volatile uint32_t RESERVED8;  /* Address offset: 0x68 */
	volatile uint32_t RESERVED9;  /* Address offset: 0x6C */
	volatile uint32_t BDCR;       /* Address offset: 0x70 */
	volatile uint32_t CSR;        /* Address offset: 0x74 */
	volatile uint32_t RESERVED10; /* Address offset: 0x78 */
	volatile uint32_t RESERVED11; /* Address offset: 0x7C */
	volatile uint32_t SSCGR;      /* Address offset: 0x80 */
	volatile uint32_t PLLI2SCFGR; /* Address offset: 0x84 */
} RCC_type;

 /*
  * General-purpose I/Os (GPIO) module
  *   page 288 from rm0090
  */
typedef struct {
	volatile uint32_t MODER;      /* Address offset: 0x00 */
	volatile uint32_t OTYPER;     /* Address offset: 0x04 */
	volatile uint32_t OSPEEDR;    /* Address offset: 0x08 */
	volatile uint32_t PUPDR;      /* Address offset: 0x0C */
	volatile uint32_t IDR;        /* Address offset: 0x10 */
	volatile uint32_t ODR;        /* Address offset: 0x14 */
	volatile uint32_t BSRR;       /* Address offset: 0x18 */
	volatile uint32_t LCKR;       /* Address offset: 0x1C */
	volatile uint32_t AFRL;       /* Address offset: 0x20 */
	volatile uint32_t AFRH;       /* Address offset: 0x24 */
} GPIO_type;

 /*
  * Universal Synchronous/Asynchronous Receiver
  * Transmitter (U(S)ART) module
  *   page 1007 from rm0090
  */
 typedef struct {
    volatile uint32_t SR;         /* Address offset: 0x00 */
    volatile uint32_t DR;         /* Address offset: 0x04 */
    volatile uint32_t BRR;        /* Address offset: 0x08 */
    volatile uint32_t CR1;        /* Address offset: 0xDC */
    volatile uint32_t CR2;        /* Address offset: 0x10 */
    volatile uint32_t CR3;        /* Address offset: 0x14 */
    volatile uint32_t GTPR;       /* Address offset: 0x18 */
 } UART_type;

  /*
  * Flash module
  *   page 1007 from rm0090
  */
 typedef struct {
    volatile uint32_t ACR;       /*!< FLASH access control register,   Address offset: 0x00 */
    volatile uint32_t KEYR;      /*!< FLASH key register,              Address offset: 0x04 */
    volatile uint32_t OPTKEYR;   /*!< FLASH option key register,       Address offset: 0x08 */
    volatile uint32_t SR;        /*!< FLASH status register,           Address offset: 0x0C */
    volatile uint32_t CR;        /*!< FLASH control register,          Address offset: 0x10 */
    volatile uint32_t OPTCR;     /*!< FLASH option control register ,  Address offset: 0x14 */
    volatile uint32_t OPTCR1;    /*!< FLASH option control register 1, Address offset: 0x18 */
 } FLASH_type;

 /** 
  * @brief Power Control
  */

typedef struct
{
  volatile uint32_t CR;          /*!< PWR power control register,        Address offset: 0x00 */
  volatile uint32_t CSR;         /*!< PWR power control/status register, Address offset: 0x04 */
} PWR_type;

/*
 * Create pointers based at the given addresses
 * You can access to each register by using -> operator
 * e.g. GPIOD->MODER
 *
 * Now we can access to the memory locations of these peripherals
 */
#define GPIOA  ((GPIO_type  *) GPIOA_BASE)
#define GPIOB  ((GPIO_type  *) GPIOB_BASE)
#define GPIOC  ((GPIO_type  *) GPIOC_BASE)
#define GPIOD  ((GPIO_type  *) GPIOD_BASE)
#define RCC    ((RCC_type   *) RCC_BASE)
#define USART2 ((UART_type  *) USART2_BASE)
#define USART4 ((UART_type  *) USART4_BASE)
#define FLASH  ((FLASH_type *) FLASH_BASE)
#define PWR    ((PWR_type   *) PWR_BASE)

#endif /* MYLIBM4_H */
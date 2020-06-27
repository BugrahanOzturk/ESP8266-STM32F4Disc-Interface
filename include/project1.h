#ifndef PROJECT1_H
#define PROJECT1_H

/*************************************************
* Includes
*************************************************/
#include "mylibm4.h"
#include "system_stm32f4xx.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>

/*************************************************
* Function Declarations
*************************************************/
void Reset_Handler(void);
void _init_data(void);
void Default_Handler(void);
void USART2_IRQHandler(void);
void USART4_IRQHandler(void);
void delay(volatile uint32_t);
void uart_tx(UART_type* USARTx, const char* brand);
void init_uart(UART_type* USARTx);
void init_green_led(void);
void init_red_led(void);
extern int main(void);


#endif
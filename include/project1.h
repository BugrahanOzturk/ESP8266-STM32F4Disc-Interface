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
#include <stdlib.h>

// Project related macros
#define SIZE_OF_BUFFER (50*8)

/*************************************************
* Function Declarations
*************************************************/
void Reset_Handler(void);
void _init_data(void);
void Default_Handler(void);
void USART2_IRQHandler(void);
void USART4_IRQHandler(void);
char USART_Receive_DATA(UART_type* USARTx);
void delay(volatile uint32_t);
void uart_tx(UART_type* USARTx, const char* brand);
void init_uart(UART_type* USARTx);
void init_green_led(void);
void init_red_led(void);
int ArraySearcher(const char* source);
extern int main(void);

#endif
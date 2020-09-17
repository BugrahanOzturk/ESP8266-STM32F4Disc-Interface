/*
 * author: Bugrahan OZTURK
 * description: STM32F4 - Useful Function Definitions for STM32F4Disc: Completely written from scratch.
 *   reference document: RM0090
 */

/*************************************************
* #INCLUDES
*************************************************/
#include "project1.h"
#include "main.h"

/*************************************************
* Global Variable Section
*************************************************/
uint32_t volatile *const pNVIC_ISER1reg = (uint32_t*)(0xE000E100 + 0x004);
uint32_t volatile *const pNVIC_IPR8reg = (uint32_t*)(0xE000E100 + 0x320); // For USART2 Interrupt
uint32_t volatile *const pNVIC_IPR13reg = (uint32_t*)(0xE000E100 + 0x334); // For USART4 Interrupt
char rx_buffer_uart2[SIZE_OF_BUFFER] = {0};
char rx_buffer_uart4[SIZE_OF_BUFFER] = {0};
int CONFIG_MODE = 0;
int tx_done_flag = 1;

// A simple and not accurate delay function
void delay(volatile uint32_t s)
{
	for(; s>0; s--){
		// Do nothing
	}
}

/*************************************************
* UART2 Interrupt Handler (For RX)
*************************************************/
void USART2_IRQHandler(void)
{
	//USART_Receive_DATA(USART2);
	static volatile int RxIndex = 0;
// check if the source is receive interrupt
	if(USART2->SR & (1 << 5)){
		GPIOD->ODR |= (1U << 12);
		char data = USART_Receive_DATA(USART2);
		if((RxIndex < SIZE_OF_BUFFER) && (data != '\n')){
			rx_buffer_uart2[RxIndex++] = data;
		} else {
			rx_buffer_uart2[RxIndex++] = data;
			RxIndex = 0;
			/* Disable the Rx buffer not empty interrupt */
			USART2->SR &= ~(1U << 5);
			
			/*** CLI MENU IS CHOSEN HERE ***/
			CLI_MODE(rx_buffer_uart2);
			/*************************************/

			while(tx_done_flag == 0); //wait if any transmission is on hand

			/*** CLI COMMANDS ARE DEFINED HERE ***/
			CLI_FUNCTIONS(CONFIG_MODE, rx_buffer_uart2);
			/*************************************/

			memset(rx_buffer_uart2, '\0', sizeof(rx_buffer_uart2));
		}
	} 
	GPIOD->ODR &= ~(1U << 12);
}

/*************************************************
* UART4 Interrupt Handler (For RX)
*************************************************/
void USART4_IRQHandler(void){
	static volatile int RxIndex = 0;
// check if the source is receive interrupt
	if(USART4->SR & (1 << 5)){
		GPIOD->ODR |= (1U << 14);
		char data = USART_Receive_DATA(USART4);
		if((data != 'K') && (data != 'R') && (data != 'L') && (RxIndex < SIZE_OF_BUFFER - 1)){
			rx_buffer_uart4[RxIndex++] = data;
		} else{
			rx_buffer_uart4[RxIndex++] = data;
			if(ArraySearcher(rx_buffer_uart4) == 1){
				rx_buffer_uart4[RxIndex] = '\n';
				RxIndex = 0;
				while(tx_done_flag == 0);
				uart_tx(USART2, rx_buffer_uart4);
				memset(rx_buffer_uart4, '\0', sizeof(rx_buffer_uart4));
			} else{
				;
			}
		}
	}
	GPIOD->ODR &= ~(1U << 14);
}

/*************************************************
* Transmitter For UART
*************************************************/
void uart_tx(UART_type* USARTx, const char* brand)
{
	tx_done_flag = 0;
	for (uint32_t i=0; i<strlen((const char*)brand); i++){
		// send character
		USARTx->DR = brand[i];
		// wait for transmit complete
		while(!(USARTx->SR & (1 << 6)));  // Check Wheter The Transmittion Is Complete Or Not
		tx_done_flag = 1;
	}
}

void init_uart(UART_type* USARTx)
{
	if (USARTx == USART2){
	
	// enable USART2 clock, bit 17 on APB1ENR
	RCC->APB1ENR |= (1 << 17);

	// enable GPIOA clock, bit 0 on AHB1ENR
	RCC->AHB1ENR |= (1 << 0);

	// set pin modes as alternate mode 7 (pins 2 and 3)
	GPIOA->MODER &= 0xFFFFFF0F; // Reset bits 10-15 to clear old values
	GPIOA->MODER |= 0x000000A0; // Set pin 2/3 to alternate func. mode (0b10)

	// set pin modes as high speed
	GPIOA->OSPEEDR |= 0x000000A0; // Set pin 2/3 to high speed mode (0b10)

	// choose AF7 for USART2 in Alternate Function registers
	GPIOA->AFRL |= (0x7 << 8); // for pin 2
	GPIOA->AFRL |= (0x7 << 12); // for pin 3

    /* Not going to use CMSIS for Interrupts
    *NVIC_SetPriority(USART2_IRQn, 1); // Priority level 1
    *NVIC_EnableIRQ(USART2_IRQn);
    */
    //Set Priorty for USART2 Global Interrupt
    *pNVIC_IPR8reg &= ~(0xFFU << 8); //Clear Bit Positions
    *pNVIC_IPR8reg |= (1 << 8); //Set priority 1

    //Enable USART2 Global Interrupt
    *pNVIC_ISER1reg |= (1 << 6); //Set for 38th Interrupt

	} 
	else if (USARTx == USART4){

		// enable USART4 clock, bit 19 on APB1ENR
		RCC->APB1ENR |= (1 << 19);
		
		// enable GPIOA clock, bit 0 on AHB1ENR
		RCC->AHB1ENR |= (1 << 0);

		// set pin modes as alternate mode 7 (pins 0 and 1)
		GPIOA->MODER &= 0xFFFFFFF0; // Reset bits 0-3 to clear old values
		GPIOA->MODER |= 0x0000000A; // Set pin 0/1 to alternate func. mode

		// set pin modes as high speed
		GPIOA->OSPEEDR |= 0x0000000A; // Set pin 0/1 to high speed mode
	
		// choose AF7 for USART4 in Alternate Function registers
		GPIOA->AFRL |= (0x8 << 0); // for pin 0
		GPIOA->AFRL |= (0x8 << 4); // for pin 1		

		/* Not going to use CMSIS for Interrupts
    	*NVIC_SetPriority(USART2_IRQn, 1); // Priority level 1
    	*NVIC_EnableIRQ(USART2_IRQn);
    	*/

		//Set Priorty for USART4 Global Interrupt
    	*pNVIC_IPR13reg &= ~(0xFFU << 0); //Clear Bit Positions
    	*pNVIC_IPR13reg |= (1 << 0); //Set priority 1

		//Enable USART4 Global Interrupt
    	*pNVIC_ISER1reg |= (1 << 20); //Set for 52th Interrupt
	
	}
	else {
		; //Do nothing
	}

	// USART4 word length M, bit 12
		//USART4->CR1 |= (0 << 12); // 0 - 1,8,n

		// USART4 parity control, bit 9
		//USART4->CR1 |= (0 << 9); // 0 - no parity

		// USART4 tx enable, TE bit 3
		USARTx->CR1 |= (1 << 3);

		// USART4 rx enable, RE bit 2
		USARTx->CR1 |= (1 << 2);

		USARTx->CR1 &= ~(1U << 1);

		// USART4 rx interrupt enable, rxne bit 5
		USARTx->CR1 |= (1 << 5);

		// baud rate = fCK / (8 * (2 - OVER8) * USARTDIV)
		//   for fCK = 42 Mhz, baud = 115200, OVER8 = 0
		//   USARTDIV = 42Mhz / 115200 / 16
		//   = 22.7864 22.8125
		// we can also look at the table in RM0090
		//   for 42 Mhz PCLK, OVER8 = 0 and 115.2 KBps baud
		//   we need to program 22.8125
		// Fraction : 16*0.8125 = 13 (multiply fraction with 16)
		// Mantissa : 22
		// 12-bit mantissa and 4-bit fraction
		USARTx->BRR |= (22 << 4);
		USARTx->BRR |= 13;

		// enable USART4 - UE, bit 13
		USARTx->CR1 |= (1 << 13);
	
}

void init_green_led(void){
	/* Enable GPIOD clock (AHB1ENR: bit 3) */
    // AHB1ENR: XXXX XXXX XXXX XXXX XXXX XXXX XXXX 1XXX
    RCC->AHB1ENR |= 0x00000008;
	GPIOD->MODER &= 0xFCFFFFFF;   // Reset bits 25:24 to clear old values
    GPIOD->MODER |= 0x01000000;   // Set MODER bits 25:24 to 01
}

void init_red_led(void){
	/* Enable GPIOD clock (AHB1ENR: bit 3) */
    // AHB1ENR: XXXX XXXX XXXX XXXX XXXX XXXX XXXX 1XXX
    RCC->AHB1ENR |= 0x00000008;
	GPIOD->MODER &= 0xCFFFFFFF;   // Reset bits 25:24 to clear old values
    GPIOD->MODER |= 0x10000000;   // Set MODER bits 25:24 to 01
}

/*************************************************
* ArraySearch Function for Detecting "OK" or "ERROR" Messages from ESP8266
*************************************************/
int ArraySearcher(const char* source){
	for(uint32_t i = 0; i < strlen((const char*)source); i++){
		if((i > 1) && (source[i] == 'K') && (source[i-1] == 'O') && (source[i-2] == '\n')){
			return 1;
		} else if((i > 3) && (source[i] == 'R') && (source[i-1] == 'O') && (source[i-2] == 'R') && (source[i-3] == 'R') && (source[i-4] == 'E')){
			return 1;
		} else if((i >2) && (source[i] == 'L') && (source[i-1] == 'I') && (source[i-2] == 'A') && (source[i-3] == 'F')){
			return 1;
		}
	}
	return 0;
}

/*************************************************
* Receiver For UART
*************************************************/
char USART_Receive_DATA(UART_type* USARTx){
	return (char)(USARTx->DR & 0x000000FF);
}

/*
 *  Default handler function
 *  All the exceptions will hit here since
 *    they are all pointed to this function
 *
 */
/*
void Default_Handler(void)
{
	GPIOD->ODR |= (1U << 14); // red led
	GPIOD->ODR |= (1U << 12); // green led
	for (;;);  // Wait forever
}
*/


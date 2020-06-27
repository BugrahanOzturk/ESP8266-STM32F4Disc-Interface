/*
 * author: Bugrahan OZTURK
 * description: STM32F4 - Startup File & Useful Function Definitions for STM32F4Disc: Completely written from scratch.
 *   reference document: RM0090
 */

/*************************************************
* #INCLUDES
*************************************************/
#include "project1.h"

// Project related macros

/*************************************************
* Global Variable Section
*************************************************/
uint32_t volatile *const pNVIC_ISER1reg = (uint32_t*)(0xE000E100 + 0x004);
uint32_t volatile *const pNVIC_IPR8reg = (uint32_t*)(0xE000E100 + 0x320); // For USART2 Interrupt
uint32_t volatile *const pNVIC_IPR13reg = (uint32_t*)(0xE000E100 + 0x334); // For USART4 Interrupt
char rx_buffer_uart2[150] = {0};
char rx_buffer_uart4[150] = {0};
int rx_done_flag = 0;
volatile int bufpos = 0;

// A simple and not accurate delay function
void delay(volatile uint32_t s)
{
	for(; s>0; s--){
		// Do nothing
	}
}

/*************************************************
* UART2 Interrupt Handler (For TX&RX)
*************************************************/
void USART2_IRQHandler(void)
{
	rx_done_flag = 0;
	static volatile int i = 0;
// check if the source is receive interrupt
	if(USART2->SR & (1 << 5)){
		GPIOD->ODR &= ~(1U << 12);
		char data = (uint8_t) (USART2->DR & 0x000000FF);
		if((data != '\n') && (i < 148)){
			GPIOD->ODR |= (1U << 12);
			rx_buffer_uart2[i++] = data;
		} else {
			rx_buffer_uart2[i] = '\n'; // Send New Line
			rx_buffer_uart2[i+1] = '\0'; // Send Line Break
			USART2->SR &= ~(1U << 5); // Clear RXNE bit
			rx_done_flag = 1;
			i = 0;
			USART4->CR1 |= (1 << 7); // Tx interrupt enable for USART4
			GPIOD->ODR &= ~(1U << 12);
		}
	} else if (USART2->SR & (1 << 7)){ // check if the source is transmit interrupt
		for(uint32_t i=0; i<strlen(rx_buffer_uart4); i++){
			// send character
			USART2->DR = rx_buffer_uart4[i];
			while(!(USART2->SR & (1 << 6))); // Check Wheter The Transmittion Is Complete Or Not
		}
	USART2->CR1 &= ~(1U << 7); // Tx interrupt disable for USART2
	memset(rx_buffer_uart4, 0, sizeof(rx_buffer_uart4));
	}
}

/*************************************************
* UART4 Interrupt Handler (For TX&RX)
*************************************************/
void USART4_IRQHandler(void){
	rx_done_flag = 0;
	static volatile int i = 0;
// check if the source is receive interrupt
	if(USART4->SR & (1 << 5)){
		GPIOD->ODR &= ~(1U << 14);
		char data = (uint8_t) (USART4->DR & 0x000000FF);
		if((data != '\n') && (i < 148)){
			GPIOD->ODR |= (1U << 14);
			rx_buffer_uart4[i++] = data;
		} else {
			rx_buffer_uart4[i] = '\n'; // Send New Line
			rx_buffer_uart4[i+1] = '\0'; // Send Line Break
			USART4->SR &= ~(1U << 5); // Clear RXNE bit
			rx_done_flag = 1;
			i = 0;
			USART2->CR1 |= (1 << 7); // Tx interrupt enable for USART2
			GPIOD->ODR &= ~(1U << 14);
		}
	} else if(USART4->SR & (1 << 7)){ // check if the source is transmit interrupt
		for(uint32_t i=0; i<strlen(rx_buffer_uart2); i++){
			// send character
			USART4->DR = rx_buffer_uart2[i];
			while(!(USART4->SR & (1 << 6))); // Check Wheter The Transmittion Is Complete Or Not
		}
		USART4->CR1 &= ~(1U << 7); // Tx interrupt disable for USART4
		memset(rx_buffer_uart2, 0, sizeof(rx_buffer_uart2));
	} 
}

/*************************************************
* Transmitter For UART
*************************************************/
void uart_tx(UART_type* USARTx, const char* brand)
{
	for (uint32_t i=0; i<strlen((const char*)brand); i++){
		// send character
		USARTx->DR = brand[i];
		// wait for transmit complete
		while(!(USARTx->SR & (1 << 6)));  // Check Wheter The Transmittion Is Complete Or Not
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
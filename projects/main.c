/*
 * author: Bugrahan OZTURK
 * description: STM32F4 - Smart Medicine Box Project: Completely written from scratch.
 *   reference document: RM0090
 */

/*************************************************
* #INCLUDES
*************************************************/
#include "project1.h"

/*************************************************
* Global Variable Section
*************************************************/
extern int rx_done_flag;
const char EntryMsg[] = "Please connect your Wifi module!\r\n ";

/*************************************************
* main code starts from here
*************************************************/
int main(void)
{
    /* set system clock to 168 Mhz */
    set_sysclk_to_168();
	init_uart(USART2);
	init_uart(USART4);
	init_green_led();
	init_red_led();
	USART2->CR1 |= (1 << 5);
	USART4->CR1 |= (1 << 5);
    while(1)
    {
		uart_tx(USART2, EntryMsg);
		while(rx_done_flag == 0);

		rx_done_flag = 0;
    }
    return 0;
}

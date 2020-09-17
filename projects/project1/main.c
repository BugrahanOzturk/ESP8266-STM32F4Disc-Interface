/*
 * author: Bugrahan OZTURK
 * description: STM32F4 - Smart Medicine Box Project: Completely written from scratch.
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
const char *EntryMsg = "################################## \n\n\
IoT CAMERA CONFIGURATION INTERFACE\n\n***USEFUL COMMANDS:*** \n-help\t\t\t\t-> Shows the available commands you can use.\n-config\t\t\t\t-> Enters you into a mode where you can use the AT instruction set to configure ESP8266.\n-normalmode & end\t\t-> Returns you back to the normal mode from configmode.\n-clear\t\t\t\t-> Clears the terminal.\n-show_available_ap\t\t-> Shows the available access points.\n-show_wifi_status\t\t-> Shows the status of WI-FI (Up or Down).\n-connect_wifi(<SSID>,<PW>)\t-> Allows you to connect to an AP.\n\n\n\n##################################\n\n\r\n";
extern int CONFIG_MODE;
extern int tx_done_flag;

/*************************************************
* main code starts from here
*************************************************/
int main(void)
{
    /*************************************************
    * System Initializations
    *************************************************/
    /* set system clock to 168 Mhz */
    set_sysclk_to_168();
	init_uart(USART2);
	init_uart(USART4);
	init_green_led();
	init_red_led();
    uart_tx(USART2, EntryMsg);
    
    while(1)
    {

    }
    return 0;
}

/*************************************************
* Function that computes which mode CLI is on (config or normal mode)
*************************************************/
void CLI_MODE(const char* brand){
	if(strcmp(brand, "config\r\n") == 0){
	    CONFIG_MODE = 1;
	} else if((strcmp(brand, "normalmode\r\n") == 0) || (strcmp(brand, "end\r\n") == 0)){
	    CONFIG_MODE = 0;
	} else{
        ;
    }
}

/*************************************************
* Function to transmit the mode information to UART (CLI)
*************************************************/
void CLI_FORMAT_TRANSMITTER(int MODE, const char* brand){
    if(MODE == 1){
	    uart_tx(USART2, "CONFIGMODE# ");
		uart_tx(USART2,brand);
        if(strcmp(brand, "config\r\n") !=0){
            uart_tx(USART4, brand);
        } else{
            ;
        }
	} else if(MODE == 0){
		uart_tx(USART2, "NORMALMODE# ");
		uart_tx(USART2, brand);
	}
}

/*************************************************
* Function that contains the CLI Commands
*************************************************/
void CLI_FUNCTIONS(int MODE, const char* brand){
    char temp[13] = {0}; //Used for connect_wifi command.

    if((strcmp(brand, "help\r\n") == 0) && (MODE == 0)){
        CLI_FORMAT_TRANSMITTER(MODE, brand);
        uart_tx(USART2, EntryMsg);

    } else if((strcmp(brand, "clear\r\n") == 0) && (MODE == 0)){
        for(uint8_t i = 0; i < 100; i++){
            uart_tx(USART2, "\r\n");
        }

    } else if((strcmp(brand, "show_available_ap\r\n") == 0) && (MODE == 0)){
        CLI_FORMAT_TRANSMITTER(MODE, brand);
        SHOW_AVAILABLE_AP();

    } else if((strcmp(memcpy(temp,brand,12), "connect_wifi") == 0) && (MODE == 0)){
        char SSID[20] = {0};
        char PW[20] = {0};
        CLI_FORMAT_TRANSMITTER(MODE, brand);
        //USE MEMCCPY -> FOR SSID AND PW
        memccpy(SSID,brand+13,',',20);
        memccpy(PW,brand+13+strlen(SSID),')',20);
        SSID[strlen(SSID) - 1] = '\0';
        PW[strlen(PW) - 1] = '\0';
        CONNECT_WIFI(SSID, PW);
        //memset(PW, '\0', sizeof(PW));
        //memset(SSID, '\0', sizeof(SSID));

    } else if((strcmp(brand, "show_wifi_status\r\n") == 0) && (MODE == 0)){
        CLI_FORMAT_TRANSMITTER(MODE, brand);
        uart_tx(USART4, " AT+CWJAP_DEF?\r\n");

    } else{
        if((MODE != 1) && (strcmp(brand, "end\r\n") != 0) && (strcmp(brand, "normalmode\r\n") != 0)){
            CLI_FORMAT_TRANSMITTER(MODE, brand);
            uart_tx(USART2, "COMMAND NOT FOUND! Please refer to <help> command.\n");
        } else{
            CLI_FORMAT_TRANSMITTER(MODE, brand);
        }
    }
}

void CONNECT_WIFI(const char* SSID, const char* PW){
    char buffer[100] = {0};
    snprintf(buffer, sizeof(buffer), " AT+CWJAP_DEF=\"%s\",\"%s\"\r\n", SSID, PW);
    uart_tx(USART4, buffer);
}

void SHOW_AVAILABLE_AP(void){
    uart_tx(USART4, " AT+CWLAP\r\n");
}
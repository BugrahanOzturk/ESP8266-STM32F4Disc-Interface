#ifndef MAIN_H
#define MAIN_H

/*************************************************
* Includes
*************************************************/
#include "project1.h"

/*************************************************
* Function Declarations
*************************************************/
void CLI_MODE(const char* brand);
void CLI_FORMAT_TRANSMITTER(int MODE, const char* brand);
void CLI_FUNCTIONS(int MODE, const char* brand);
void CONNECT_WIFI(const char* SSID, const char* PW);
void SHOW_AVAILABLE_AP(void);

#endif /*__MAIN_H */
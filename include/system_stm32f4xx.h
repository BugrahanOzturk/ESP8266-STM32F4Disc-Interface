#ifndef SYSTEM_STM32F4XX_H
#define SYSTEM_STM32F4XX_H

/* PROJECT SPECIFIS MACROS */
#ifndef STM32F407xx
    #define STM32F407xx
#endif

/* Clock PLLs for 407 chip */
#if defined (STM32F407xx) || defined (STM32F429xx)
// Main PLL = N * (source_clock / M) / P
// HSE = 8 Mhz
// fCLK =   N * (8Mhz / M) / P
#define PLL_M   8
#define PLL_Q   7
#define PLL_P   2
#endif

/* stm32f407 runs at 168Mhz max */
#if defined (STM32F407xx)
#define PLL_N   336
#endif

void set_sysclk_to_168(void);

#endif /*__SYSTEM_STM32F4XX_H */
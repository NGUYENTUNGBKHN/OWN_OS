#ifndef STM32F746_H
#define STM32F746_H

#include <stdint.h>

#define REG32(addr) (*(volatile uint32_t *)(addr))

#define SCB_CPACR REG32(0xE000ED88UL)

#define ITM_BASE 0xE0000000UL
#define ITM_STIM_PORT0 REG32(ITM_BASE + 0x000UL)
#define ITM_TER REG32(ITM_BASE + 0xE00UL)
#define ITM_TCR REG32(ITM_BASE + 0xE80UL)

#define DEMCR REG32(0xE000EDFCUL)
#define DEMCR_TRCENA (1UL << 24)

void SystemInit(void);
int ITM_SendChar(int ch);

#endif

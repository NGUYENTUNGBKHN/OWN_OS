#include "stm32f746.h"

int ITM_SendChar(int ch)
{
    if ((DEMCR & DEMCR_TRCENA) == 0U) {
        return ch;
    }

    if ((ITM_TCR & 1U) == 0U || (ITM_TER & 1U) == 0U) {
        return ch;
    }

    while (ITM_STIM_PORT0 == 0U) {
    }

    *((volatile uint8_t *)(ITM_BASE + 0x000UL)) = (uint8_t)ch;
    return ch;
}

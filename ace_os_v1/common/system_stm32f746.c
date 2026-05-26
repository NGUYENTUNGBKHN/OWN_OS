#include "stm32f746.h"

void SystemInit(void)
{
    SCB_CPACR |= (0xFUL << 20);
}

#include "stm32f746.h"

#if defined(LAB4_USE) || defined(LAB5_USE)
uint32_t SystemCoreClock = 216000000UL;

void SystemClock_Config(void)
{
    RCC_CR |= RCC_CR_HSION;
    while ((RCC_CR & RCC_CR_HSIRDY) == 0U) {
    }

    RCC_CFGR &= ~(0x3UL << RCC_CFGR_SW_Pos);
    while (((RCC_CFGR >> RCC_CFGR_SWS_Pos) & 0x3UL) != 0U) {
    }

    RCC_CR &= ~RCC_CR_PLLON;
    while ((RCC_CR & RCC_CR_PLLRDY) != 0U) {
    }

    RCC_APB1ENR |= RCC_APB1ENR_PWREN;
    PWR_CR1 &= ~(0x3UL << PWR_CR1_VOS_Pos);
    PWR_CR1 |= (0x3UL << PWR_CR1_VOS_Pos);
    PWR_CR1 |= PWR_CR1_ODEN;
    while ((PWR_CSR1 & PWR_CSR1_ODRDY) == 0U) {
    }
    PWR_CR1 |= PWR_CR1_ODSWEN;
    while ((PWR_CSR1 & PWR_CSR1_ODSWRDY) == 0U) {
    }

    FLASH_ACR = (7UL << FLASH_ACR_LATENCY_Pos) |
                FLASH_ACR_PRFTEN |
                FLASH_ACR_ARTEN;

    RCC_PLLCFGR = (16UL << RCC_PLLCFGR_PLLM_Pos) |
                  (432UL << RCC_PLLCFGR_PLLN_Pos) |
                  (0UL << RCC_PLLCFGR_PLLP_Pos) |
                  (9UL << RCC_PLLCFGR_PLLQ_Pos);

    RCC_CFGR &= ~((0xFUL << RCC_CFGR_HPRE_Pos) |
                  (0x7UL << RCC_CFGR_PPRE1_Pos) |
                  (0x7UL << RCC_CFGR_PPRE2_Pos));
    RCC_CFGR |= (0x5UL << RCC_CFGR_PPRE1_Pos) |
                (0x4UL << RCC_CFGR_PPRE2_Pos);

    RCC_CR |= RCC_CR_PLLON;
    while ((RCC_CR & RCC_CR_PLLRDY) == 0U) {
    }

    RCC_CFGR &= ~(0x3UL << RCC_CFGR_SW_Pos);
    RCC_CFGR |= (RCC_CFGR_SW_PLL << RCC_CFGR_SW_Pos);
    while (((RCC_CFGR >> RCC_CFGR_SWS_Pos) & 0x3UL) != RCC_CFGR_SWS_PLL) {
    }
}
#endif /* LAB4_USE || LAB5_USE */

void SysTick_Enable(uint32_t tick_hz)
{
    uint32_t reload;

    if (tick_hz == 0U) {
        return;
    }

#if defined(LAB4_USE) || defined(LAB5_USE)
    reload = (SystemCoreClock / tick_hz) - 1U;
#else
    reload = (16000000UL / tick_hz) - 1U;
#endif /* LAB4_USE || LAB5_USE */

    if (reload > 0x00FFFFFFUL) {
        return;
    }

    SYSTICK_CTRL = 0U;
    SYSTICK_LOAD = reload;
    SYSTICK_VAL = 0U;
    SYSTICK_CTRL = SYSTICK_CTRL_CLKSOURCE |
                   SYSTICK_CTRL_TICKINT |
                   SYSTICK_CTRL_ENABLE;
}

void __disable_irq(void)
{
    __asm volatile ("cpsid i" ::: "memory");
    __asm volatile ("isb");
}

void __enable_irq(void)
{
    __asm volatile ("cpsie i" ::: "memory");
    __asm volatile ("isb");
}

void SystemInit(void)
{
    SCB_CPACR |= (0xFUL << 20);
#if defined(LAB4_USE) || defined(LAB5_USE)
    SystemClock_Config();
#endif /* LAB4_USE || LAB5_USE */
}

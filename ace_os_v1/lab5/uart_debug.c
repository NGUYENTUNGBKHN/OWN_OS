#include "stm32f746.h"
#include "uart_debug.h"

#define UART1_BAUDRATE 115200UL
#define UART1_PCLK_HZ 108000000UL
#define UART1_TX_PIN 9UL
#define GPIO_AF7_USART 7UL

void UART1_Debug_Init(void)
{
    RCC_AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC_APB2ENR |= RCC_APB2ENR_USART1EN;

    GPIOA_MODER &= ~(0x3UL << (UART1_TX_PIN * 2UL));
    GPIOA_MODER |= (0x2UL << (UART1_TX_PIN * 2UL));

    GPIOA_OTYPER &= ~(1UL << UART1_TX_PIN);

    GPIOA_OSPEEDR &= ~(0x3UL << (UART1_TX_PIN * 2UL));
    GPIOA_OSPEEDR |= (0x3UL << (UART1_TX_PIN * 2UL));

    GPIOA_PUPDR &= ~(0x3UL << (UART1_TX_PIN * 2UL));
    GPIOA_PUPDR |= (0x1UL << (UART1_TX_PIN * 2UL));

    GPIOA_AFRH &= ~(0xFUL << ((UART1_TX_PIN - 8UL) * 4UL));
    GPIOA_AFRH |= (GPIO_AF7_USART << ((UART1_TX_PIN - 8UL) * 4UL));

    USART1_CR1 = 0U;
    USART1_BRR = (UART1_PCLK_HZ + (UART1_BAUDRATE / 2UL)) / UART1_BAUDRATE;
    USART1_CR1 = USART_CR1_TE | USART_CR1_UE;
}

int UART1_Debug_WriteChar(int ch)
{
    while ((USART1_ISR & USART_ISR_TXE) == 0U) {
    }

    USART1_TDR = (uint32_t)(ch & 0xFF);
    return ch;
}

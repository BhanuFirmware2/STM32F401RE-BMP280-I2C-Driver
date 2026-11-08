#include <stdint.h>
#include "usart.h"

#define RCC_AHB1ENR  (*(volatile unsigned int *)0x40023830U)
#define RCC_APB1ENR  (*(volatile unsigned int *)0x40023840U)
#define GPIOA_MODER  (*(volatile unsigned int *)0x40020000U)
#define GPIOA_AFRL   (*(volatile unsigned int *)0x40020020U)
#define USART2_SR    (*(volatile unsigned int *)0x40004400U)
#define USART2_DR    (*(volatile unsigned int *)0x40004404U)
#define USART2_BRR   (*(volatile unsigned int *)0x40004408U)
#define USART2_CR1   (*(volatile unsigned int *)0x4000440CU)

void USART2_Init(void) {
    RCC_AHB1ENR |= (1U << 0);   // Enable GPIOA Clock
    RCC_APB1ENR |= (1U << 17);  // Enable USART2 Clock

    // PA2 (TX), PA3 (RX) to Alternate Function Mode
    GPIOA_MODER &= ~((3U << 4) | (3U << 6));
    GPIOA_MODER |=  ((2U << 4) | (2U << 6));

    // Map to AF7 (USART2)
    GPIOA_AFRL &= ~((15U << 8) | (15U << 12));
    GPIOA_AFRL |=  ((7U << 8) | (7U << 12));

    USART2_CR1 = 0U;
    USART2_BRR = 0x0683U;       // 9600 Baudrate at 16MHz

    USART2_CR1 |= (1U << 3);    // TE (Transmit Enable)
    USART2_CR1 |= (1U << 13);   // UE (USART Enable)
}

void USART2_SendChar(char data) {
    while ((USART2_SR & (1U << 7)) == 0U); // Wait for TXE
    USART2_DR = (uint8_t)data;
}

void USART2_SendString(const char *str) {
    while (*str != '\0') {
        USART2_SendChar(*str);
        str++;
    }

}

#include "i2c.h"
#include <stdint.h>

#define RCC_AHB1ENR         (*(volatile uint32_t *)0x40023830U)
#define RCC_APB1ENR         (*(volatile uint32_t *)0x40023840U)
#define GPIOB_MODER         (*(volatile uint32_t *)0x40020400U)
#define GPIOB_OTYPER        (*(volatile uint32_t *)0x40020404U)
#define GPIOB_PUPDR         (*(volatile uint32_t *)0x4002040CU)
#define GPIOB_AFRH          (*(volatile uint32_t *)0x40020424U)
#define I2C1_CR1            (*(volatile uint32_t *)0x40005400U)
#define I2C1_CR2            (*(volatile uint32_t *)0x40005404U)
#define I2C1_DR             (*(volatile uint32_t *)0x40005410U)
#define I2C1_SR1            (*(volatile uint32_t *)0x40005414U)
#define I2C1_SR2            (*(volatile uint32_t *)0x40005418U)
#define I2C1_CCR            (*(volatile uint32_t *)0x4000541CU)
#define I2C1_TRISE          (*(volatile uint32_t *)0x40005420U)

void I2C1_GPIO_Init(void) {
    RCC_AHB1ENR |= (1U << 1); // Enable GPIOB Clock

    GPIOB_MODER &= ~((3U << 16) | (3U << 18));
    GPIOB_MODER |=  ((2U << 16) | (2U << 18)); // Alternate Function

    GPIOB_AFRH &= ~((0xFU << 0) | (0xFU << 4));
    GPIOB_AFRH |=  ((4U << 0) | (4U << 4));  // AF4 for I2C1

    GPIOB_OTYPER |= ((1U << 8) | (1U << 9));   // Open-Drain

    GPIOB_PUPDR &= ~((3U << 16) | (3U << 18));
    GPIOB_PUPDR |=  ((1U << 16) | (1U << 18));  // Pull-up
}

void I2C1_Init(void) {
    I2C1_GPIO_Init();
    RCC_APB1ENR |= (1U << 21); // Enable I2C1 Clock

    // SWRST (Software Reset)
    I2C1_CR1 |= (1U << 15);
    for(volatile int i = 0; i < 1000; i++);
    I2C1_CR1 &= ~(1U << 15);

    I2C1_CR1 &= ~(1U << 0); // Disable PE
    I2C1_CR2 = 16U;         // 16 MHz APB1 Clock
    I2C1_CCR = 80U;         // 100kHz SM
    I2C1_TRISE = 17U;
    I2C1_CR1 |= (1U << 0);  // Enable PE
}

void I2C1_Start(void) {
    I2C1_CR1 |= (1U << 8);
    while (!(I2C1_SR1 & (1U << 0))); // Wait for SB
}

void I2C1_Stop(void) {
    I2C1_CR1 |= (1U << 9);
}

// టైమ్‌అవుట్ మరియు AF ఎర్రర్ హ్యాండ్లింగ్‌తో కూడిన Writeaddress
void I2C1_Writeaddress(uint8_t address) {
    I2C1_DR = address;
    uint32_t timeout = 50000; // హ్యాంగ్ అవ్వకుండా ఉండటానికి టైమ్‌అవుట్

    while (!(I2C1_SR1 & (1U << 1))) {
        // AF (Acknowledge Failure) బిట్ చెక్ చేయడం (Bit 9)
        if (I2C1_SR1 & (1U << 9)) {
            volatile uint32_t temp = I2C1_SR1;
            (void)temp;
            I2C1_Stop();
            I2C1_SR1 &= ~(1U << 9); // AF క్లియర్ చేయడం
            return;
        }

        timeout--;
        if (timeout == 0)
        {
            I2C1_Stop();
            return;
        }
    }

    volatile uint32_t temp = I2C1_SR1;
    temp = I2C1_SR2;
    (void)temp; // Clear ADDR flag
}

void I2C1_Writeadata(uint8_t data)
{
    uint32_t timeout = 50000;
    while (!(I2C1_SR1 & (1U << 7)))
    { // TXE Check
        if (I2C1_SR1 & (1U << 9))
        {
            volatile uint32_t temp = I2C1_SR1;
            (void)temp;
            I2C1_Stop();
            I2C1_SR1 &= ~(1U << 9);
            return;
        }
        if (--timeout == 0)
        {
        	I2C1_Stop();
        	return;
        }
    }

    I2C1_DR = data;

    timeout = 50000;
    while (!(I2C1_SR1 & (1U << 2)))
    { // BTF Check
        if (I2C1_SR1 & (1U << 9))
        {
            volatile uint32_t temp = I2C1_SR1;
            (void)temp;
            I2C1_Stop();
            I2C1_SR1 &= ~(1U << 9);
            return;
        }
        if (--timeout == 0)
        {
        	I2C1_Stop();
        	return;
        }
    }
}

void I2C1_Readaddress(uint8_t address)
{
    I2C1_DR = address;
    uint32_t timeout = 50000;

    while (!(I2C1_SR1 & (1U << 1)))
    {
        if (I2C1_SR1 & (1U << 9))
        {
            volatile uint32_t temp = I2C1_SR1;
            (void)temp;
            I2C1_Stop();
            I2C1_SR1 &= ~(1U << 9);
            return;
        }
        if (--timeout == 0)
        {
        	I2C1_Stop();
        	return;
        }
    }

    volatile uint32_t temp = I2C1_SR1;
    temp = I2C1_SR2;
    (void)temp;
}

uint8_t I2C1_Readdata_NACK(void)
{
    I2C1_CR1 &= ~(1U << 10); // Clear ACK

    uint32_t timeout = 50000;
    while (!(I2C1_SR1 & (1U << 6)))
    { // RXNE Check
        if (--timeout == 0)
        {
        	I2C1_Stop();
        	return 0;
        }
    }

    I2C1_CR1 |= (1U << 9);   // Generate STOP
    uint8_t data = (uint8_t)I2C1_DR;
    I2C1_CR1 |= (1U << 10);  // Restore ACK

    return data;
}

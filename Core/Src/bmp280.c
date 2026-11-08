#include <stdint.h>
#include "i2c.h"
#include "bmp280.h"
#include "usart.h"

#define Writeaddress 0XECU
#define Readaddress  0XEDU
#define ChipId_Reg   0XD0U

uint8_t BMP280_ReadChip(void) {
    uint8_t chipid;
    USART2_SendString("1. START\r\n");
    I2C1_Start();

    USART2_SendString("2. Write address\r\n");
    I2C1_Writeaddress(Writeaddress);

    USART2_SendString("3. chip id register address\r\n");
    I2C1_Writeadata(ChipId_Reg);

    USART2_SendString("4. Repeated start\r\n");
    I2C1_Start();

    USART2_SendString("5. Read address\r\n");
    I2C1_Readaddress(Readaddress);

    USART2_SendString("6. Read data\r\n");
    chipid = I2C1_Readdata_NACK();

    USART2_SendString("7. completed\r\n");
    return chipid;
}

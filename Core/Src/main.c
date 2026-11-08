#include "usart.h"
#include "i2c.h"
#include "bmp280.h"
#include <stdint.h>


volatile uint8_t chip_id;

int main(void)
{
	I2C1_Init();
    USART2_Init();

    USART2_SendString("BMP280 CHIP ID Test\r\n");

    chip_id = BMP280_ReadChip();

    if (chip_id == 0x58U)
    {
        USART2_SendString("BMP280 Detected Successfully\r\n");
        USART2_SendString("CHIP ID = 0x58\r\n");
    }
    else
    {
        USART2_SendString("BMP280 Communication Failed\r\n");
    }

    while (1)
    {
    }
}

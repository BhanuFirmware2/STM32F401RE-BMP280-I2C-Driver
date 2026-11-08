#include <stdint.h>
#ifndef INC_I2C_H_
#define INC_I2C_H_

void I2C1_GPIO_Init(void);
void I2C1_Init(void);
void I2C1_Start(void);
void I2C1_Stop(void);
void I2C1_Writeaddress(uint8_t address);
void I2C1_Readaddress(uint8_t address);
void I2C1_Writeadata(uint8_t data);
uint8_t I2C1_Readdata_ACK(void);
uint8_t I2C1_Readdata_NACK(void);
#endif

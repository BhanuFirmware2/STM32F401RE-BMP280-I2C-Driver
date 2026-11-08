# STM32F401RE – BMP280 I2C Driver

## Project Overview

This project demonstrates I2C communication between the STM32 NUCLEO-F401RE microcontroller and the BMP280 sensor using Bare-Metal Embedded C programming.

The main objective is to initialize the I2C1 peripheral and read the BMP280 Chip ID register (`0xD0`) to verify successful communication between the microcontroller and the sensor.

The project uses direct register-level programming without STM32 HAL libraries.

## Hardware Requirements

- STM32 NUCLEO-F401RE Development Board
- BMP280 Sensor Module
- Breadboard
- Jumper Wires
- USB Cable

## Software Requirements

- Windows 10/11
- STM32CubeIDE
- Embedded C
- GNU Arm Embedded Toolchain
- PuTTY (Serial Terminal)

## Hardware Connections

### STM32F401RE to BMP280

| STM32F401RE | BMP280 |
|---|---|
| PB6 | SCL |
| PB7 | SDA |
| 3.3V | VCC |
| GND | GND |
| 3.3V | CSB |
| GND | SDO |

### I2C Configuration

- I2C Peripheral: I2C1
- SCL: PB6
- SDA: PB7
- Alternate Function: AF4
- GPIO Configuration: Alternate Function Open-Drain
- Communication Mode: Master
- I2C Speed: 100 kHz
- APB1 Clock: 16 MHz
- 7-bit Slave Address: 0x76

## Registers Used

### RCC Registers

| Register | Address | Purpose |
|---|---|---|
| RCC_AHB1ENR | 0x40023830 | Enable GPIOB clock |
| RCC_APB1ENR | 0x40023840 | Enable I2C1 clock |

### GPIOB Registers

| Register | Address | Purpose |
|---|---|---|
| GPIOB_MODER | 0x40020400 | GPIO mode configuration |
| GPIOB_OTYPER | 0x40020404 | Output type configuration |
| GPIOB_PUPDR | 0x4002040C | Pull-up/Pull-down configuration |
| GPIOB_AFRL | 0x40020420 | Alternate function configuration |

### I2C1 Registers

| Register | Address | Purpose |
|---|---|---|
| I2C1_CR1 | 0x40005400 | Control register 1 |
| I2C1_CR2 | 0x40005404 | Control register 2 |
| I2C1_DR | 0x40005410 | Data register |
| I2C1_SR1 | 0x40005414 | Status register 1 |
| I2C1_SR2 | 0x40005418 | Status register 2 |
| I2C1_CCR | 0x4000541C | Clock control register |
| I2C1_TRISE | 0x40005420 | Rise time register |

## I2C Clock Configuration

For an APB1 clock frequency of 16 MHz and standard-mode I2C communication at 100 kHz:

| Parameter | Value |
|---|---:|
| PCLK1 | 16 MHz |
| CR2 | 16 |
| CCR | 80 |
| TRISE | 17 |
| I2C Frequency | 100 kHz |

## BMP280 Chip ID Register

| Parameter | Value |
|---|---|
| Register Name | CHIP_ID |
| Register Address | 0xD0 |
| Expected Chip ID | 0x58 |
| Decimal Value | 88 |

The BMP280 Chip ID register is used to verify whether the sensor is responding correctly to I2C communication.

## Working Principle

1. Enable GPIOB and I2C1 peripheral clocks.
2. Configure PB6 and PB7 for I2C1 alternate function AF4.
3. Configure GPIO pins in open-drain mode.
4. Initialize I2C1 for 100 kHz communication.
5. Initialize USART2 for serial communication.
6. Generate the I2C START condition.
7. Send the BMP280 slave address with the Write bit.
8. Send the CHIP_ID register address (`0xD0`).
9. Generate a Repeated START condition.
10. Send the BMP280 slave address with the Read bit.
11. Read one byte from the sensor.
12. Generate NACK and STOP conditions.
13. Display the received Chip ID through USART2 on PuTTY.

## I2C Communication Sequence

```text
          START
            |
            v
    Slave Address + Write
            |
            v
       ACK Received
            |
            v
    Register Address (0xD0)
            |
            v
       ACK Received
            |
            v
      Repeated START
            |
            v
     Slave Address + Read
            |
            v
       ACK Received
            |
            v
      Read CHIP_ID Byte
            |
            v
           NACK
            |
            v
           STOP
            |
            v
    Verify CHIP_ID = 0x58
            |
            v
    Display Result on PuTTY
```

## Project Structure

```text
STM32F401RE-BMP280-I2C-Driver/
│
├── Core/
│   ├── Inc/
│   │   ├── I2C.h
│   │   ├── BMP280.h
│   │   └── USART.h
│   │
│   └── Src/
│       ├── I2C.c
│       ├── BMP280.c
│       ├── USART.c
│       └── main.c
│
└── README.md
```

### Driver Description

| File | Description |
|---|---|
| I2C.h / I2C.c | I2C1 initialization and communication functions |
| BMP280.h / BMP280.c | BMP280 Chip ID register reading |
| USART.h / USART.c | USART2 serial communication |
| main.c | Main application execution |

## Key Learning Outcomes

- Understanding the I2C communication protocol.
- STM32F401RE peripheral register programming.
- GPIO alternate function configuration.
- I2C START, STOP and Repeated START conditions.
- Understanding ACK and NACK operations.
- I2C master transmitter and receiver operations.
- BMP280 sensor register reading.
- USART2 serial communication using PuTTY.
- Embedded firmware debugging using STM32CubeIDE.

## Development Approach

- Bare-Metal Embedded C programming.
- Direct memory-mapped register access.
- No STM32 HAL functions for peripheral operations.
- Driver-based modular programming.

## Project Scope

**Current Implementation: BMP280 Chip ID Reading Only.**

This project focuses on establishing I2C communication and verifying the BMP280 sensor by reading its Chip ID register.

Temperature measurement, pressure measurement, calibration coefficients and compensation algorithms are not implemented in this project.

## Clone Repository

Clone this repository using Git:

```bash
git clone https://github.com/BhanuFirmware2/STM32F401RE-BMP280-I2C-Driver.git
```

Navigate to the project directory:

```bash
cd STM32F401RE-BMP280-I2C-Driver
```

Open the project in STM32CubeIDE and build the firmware.

## Output
https://github.com/BhanuFirmware2/STM32F401RE-BMP280-I2C-Driver/blob/main/Output%20Read%20Chipid.png

## Author

 Ravinuthala Bhanu Prakash

- Microcontroller: STM32 NUCLEO-F401RE
- Sensor: BMP280
- Communication Protocol: I2C
- Serial Interface: USART2
- Programming Language: Embedded C
- IDE: STM32CubeIDE

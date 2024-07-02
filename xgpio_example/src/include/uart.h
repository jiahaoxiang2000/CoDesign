#ifndef __UART_H
#define __UART_H

#include "PmodGPIO.h"
extern PmodGPIO JaDevice; // JA pmod

#define UART_TX_DOWN() GPIO_setPin(&JaDevice, 1, 0)
#define UART_TX_UP() GPIO_setPin(&JaDevice, 1, 1) // SCL

#define UART_RX() GPIO_getPin(&JaDevice, 2) // DIN



#define CLOCK_SPEED 1000000 // 100MHz
#define BAUD_RATE 115200
#define BIT_TIME (CLOCK_SPEED / BAUD_RATE)

void uartSendByte(uint8_t byte);
uint8_t uartReceiveByte();
#endif

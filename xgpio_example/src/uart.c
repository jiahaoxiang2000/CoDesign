#include "uart.h"
#include "stdlib.h"

#include "xiltimer.h" // Ensure you include the header for XilTimer

void delayBitTime(uint32_t bitTime) {
    // Convert bitTime from clock cycles to microseconds assuming a clock speed
    // This conversion will depend on your system's clock speed
    // For example, assuming a 100 MHz clock, 1 clock cycle = 0.01 microseconds
    // Adjust the conversion factor based on your actual clock speed
    // unsigned long useconds = (bitTime / 10) * 0.01;
    usleep(bitTime -1);
}

void uartSendByte(uint8_t byte) {
    uint8_t bit;

    // Start bit
    UART_TX_DOWN();
    delayBitTime(BIT_TIME);

    // Send 8 data bits
    for (bit = 0; bit < 8; bit++) {
        if (byte & (1 << bit)) {
            UART_TX_UP();
        } else {
            UART_TX_DOWN();
        }
        delayBitTime(BIT_TIME);
    }

    // Stop bit
    UART_TX_UP();
    delayBitTime(BIT_TIME);
}

uint8_t uartReceiveByte() {
    uint8_t byte = 0;
    uint8_t bit;

    // Wait for start bit
    while(UART_RX() != 0); // Assuming 0 is the active low start bit

    // Delay to the middle of the start bit
    delayBitTime(BIT_TIME / 2 + BIT_TIME);

    // Read 8 data bits
    for (bit = 0; bit < 8; bit++) {
        byte >>= 1; // Shift byte to right
        if (UART_RX()) {
            byte |= 0x80; // Set the MSB if UART_RX is high
        }
        // Wait for the next bit
        delayBitTime(BIT_TIME);
    }

    // Wait for stop bit (optional, depending on your timing and protocol requirements)
    delayBitTime(BIT_TIME);

    return byte;
}
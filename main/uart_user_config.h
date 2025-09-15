#ifndef UART_USER_CONFIG_H
#define UART_USER_CONFIG_H

#include "HardwareSerial.h"

// Define default baud rate
#define UART_BAUD_RATE 9600
static const int RX_BUF_SIZE = 128;

// UART ports and pin definitions
// You can use any available GPIO pins
#define SO2_UART_PORT 1
#define SO2_UART_RX_PIN 16
#define SO2_UART_TX_PIN 17

#define H2S_UART_PORT 2
#define H2S_UART_RX_PIN 26
#define H2S_UART_TX_PIN 27

// Instantiate HardwareSerial objects
extern HardwareSerial SO2_Serial;
extern HardwareSerial H2S_Serial;

#endif // UART_USER_CONFIG_H
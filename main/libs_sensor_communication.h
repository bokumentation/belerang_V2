#ifndef LIBS_SENSOR_COMMUNICATION_H
#define LIBS_SENSOR_COMMUNICATION_H

// #include "libs_communication_sensor.h"
#include "Arduino.h"
#include "tb600b_so2.h"

// User void
void sensor_so2_get_combined_data();
void sensor_h2s_get_combined_data();

// Instantiate the HardwareSerial objects
HardwareSerial SO2_Serial(SO2_UART_PORT);
HardwareSerial H2S_Serial(H2S_UART_PORT);

// Arduino Setup and Loop
void tb600bSetup();
void tb600bLoop();

#endif // LIBS_SENSOR_COMMUNICATION_H
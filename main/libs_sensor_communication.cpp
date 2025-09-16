// Copyrighted: Ibe hiahaha
#include "Arduino.h"

// User include
#include "tb600b_so2.h"
#include "libs_sensor_communication.h"

// User void
void sensor_so2_get_combined_data();
void sensor_h2s_get_combined_data();

void tb600bSetup() {
  // Initialize UART1 for SO2 sensor
  SO2_Serial.begin(UART_BAUD_RATE, SERIAL_8N1, SO2_UART_RX_PIN, SO2_UART_TX_PIN);
  delay(100);
  Serial.println("SO2 UART initialized.");

  // Initialize UART2 for H2S sensor
  H2S_Serial.begin(UART_BAUD_RATE, SERIAL_8N1, H2S_UART_RX_PIN, H2S_UART_TX_PIN);
  delay(100);
  Serial.println("H2S UART initialized.");
}

void tb600bLoop() {
  sensor_h2s_get_combined_data();
  sensor_so2_get_combined_data();
}

void sensor_so2_get_combined_data() {
  // Need to work for get global varible and not to print here.
  ESP_LOGI("SO2", "GET DATA");
  tb600b::get_combined_data(SO2_Serial, CMD_GET_COMBINED_DATA, sizeof(CMD_GET_COMBINED_DATA));
}

void sensor_h2s_get_combined_data() {
  // Need to work for get global varible and not to print here.
  ESP_LOGI("H2S", "GET DATA");
  tb600b::get_combined_data(H2S_Serial, CMD_GET_COMBINED_DATA, sizeof(CMD_GET_COMBINED_DATA));
}
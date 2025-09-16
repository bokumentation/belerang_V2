#include "Arduino.h"
#include <stdio.h>

// Wifi Library
#include "WiFi.h"     // Unused
#include "esp_now.h"  // Unused
#include "esp_wifi.h" // Unused

// User include library
#include "anemometer.h"
#include "libs_sensor_communication.h"
#include "tb600b_so2.h"
#include "uart_user_config.h"


// User variable

const static char *pTAG_MAIN = "MAIN";

// // Instantiate the HardwareSerial objects
// HardwareSerial SO2_Serial(SO2_UART_PORT);
// HardwareSerial H2S_Serial(H2S_UART_PORT);

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect
  }

  // WiFi.mode(WIFI_STA);
  // String macAddress = WiFi.macAddress();
  // Serial.print("MAC Address: ");
  // Serial.println(macAddress);

  // // Initialize UART1 for SO2 sensor
  // SO2_Serial.begin(UART_BAUD_RATE, SERIAL_8N1, SO2_UART_RX_PIN, SO2_UART_TX_PIN);
  // delay(100);
  // Serial.println("SO2 UART initialized.");

  // // Initialize UART2 for H2S sensor
  // H2S_Serial.begin(UART_BAUD_RATE, SERIAL_8N1, H2S_UART_RX_PIN, H2S_UART_TX_PIN);
  // delay(100);
  // Serial.println("H2S UART initialized.");

  // Initialize Anemometer
  anemometer_setup();
  tb600bSetup();
  delay(100);
}

void loop() {
  // Sensor Anemometer
  anemometer_loop();
  float wind_speed = getAnemometerSpeed_kmph();
  ESP_LOGI("MAIN LOOP", "--------------------------------------");
  ESP_LOGI("ANEMOMETER", "GET WIND SPEED");
  Serial.println("Wind Speed Response: ");
  Serial.print(wind_speed);
  Serial.print(" km/h");
  Serial.println(" ");
  tb600bLoop();
  
  // To-do
  
  // Sensor H2S
  // Need new implementation!! - ibe
  
  // Sensor SO2
  // Need new implementation!! - ibe
  delay(5000);
}
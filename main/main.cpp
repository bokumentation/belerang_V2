#include <stdio.h>
#include "Arduino.h"

// Wifi Library
#include "esp_wifi.h"
#include "esp_now.h"
#include "WiFi.h"

// User include library
#include "anemometer.h"
#include "tb600b_so2.h"
#include "uart_user_config.h"

// User variable

const static char *pTAG_MAIN = "MAIN";

// Instantiate the HardwareSerial objects
HardwareSerial SO2_Serial(SO2_UART_PORT);
HardwareSerial H2S_Serial(H2S_UART_PORT);

void setup()
{
  Serial.begin(115200);
  while (!Serial)
  {
    ; // wait for serial port to connect
  }
  WiFi.mode(WIFI_STA);
  String macAddress = WiFi.macAddress();
  Serial.print("MAC Address: ");
  Serial.println(macAddress);

  // Initialize UART1 for SO2 sensor
  SO2_Serial.begin(UART_BAUD_RATE, SERIAL_8N1, SO2_UART_RX_PIN, SO2_UART_TX_PIN);
  delay(100);
  Serial.println("SO2 UART initialized.");

  // Initialize UART2 for H2S sensor
  H2S_Serial.begin(UART_BAUD_RATE, SERIAL_8N1, H2S_UART_RX_PIN, H2S_UART_TX_PIN);
  delay(100);
  Serial.println("H2S UART initialized.");

  // Initialize Anemometer
  anemometer_setup();
  delay(100);
}

void loop()
{
  anemometer_loop();
  float wind_speed = getAnemometerSpeed_kmph();
  // ESP_LOGI(pTAG_MAIN, "Current wind speed: %.2f km/h", wind_speed);
  
  ESP_LOGI("MAIN LOOP", "--------------------------------------");
  ESP_LOGI("ANEMOMETER", "GET WIND SPEED");
  Serial.println("Wind Speed Response: ");
  Serial.print(wind_speed); Serial.print(" km/h");
  Serial.println(" ");
  
  // Get combined data from the UART sensor
  ESP_LOGI("SO2", "GET DATA");
  tb600b::get_combined_data(SO2_Serial, CMD_GET_COMBINED_DATA, sizeof(CMD_GET_COMBINED_DATA));

  ESP_LOGI("H2S", "GET DATA");
  tb600b::get_combined_data(H2S_Serial, CMD_GET_COMBINED_DATA, sizeof(CMD_GET_COMBINED_DATA));

  delay(5000);
}
#include "Arduino.h"

// User include library
#include "anemometer.h"
#include "tb600b_so2.h"
#include "uart_user_config.h"

// Instantiate the HardwareSerial objects
HardwareSerial SO2_Serial(SO2_UART_PORT);
HardwareSerial H2S_Serial(H2S_UART_PORT);

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect
  }

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

void loop() {
  anemometer_loop();
  float wind_speed = getAnemometerSpeed_kmph();
  ESP_LOGI("MAIN LOOP", "--------------------------------------");
  ESP_LOGI("ANEMOMETER", "GET WIND SPEED");
  Serial.println("Wind Speed Response: ");
  Serial.print(wind_speed);
  Serial.print(" km/h");
  Serial.println(" ");

  // Get combined data from the SO2 sensor
  ESP_LOGI("SO2", "GET DATA");
  tb600b::get_so2_data(SO2_Serial, CMD_GET_COMBINED_DATA, sizeof(CMD_GET_COMBINED_DATA));

  if (tb600b::so2_isDataAvailable) {
    Serial.println("SO2 Sensor Data:");
    Serial.printf("  Temperature: %.2f °C\n", tb600b::so2_currentTemperature);
    Serial.printf("  Humidity: %.2f %%\n", tb600b::so2_currentHumidity);
    Serial.printf("  Gas Concentration: %.2f ug/m^3\n", tb600b::so2_currentGasUg);
  } else {
    Serial.println("SO2 sensor data not available.");
  }

  // Get combined data from the H2S sensor
  ESP_LOGI("H2S", "GET DATA");
  tb600b::get_h2s_data(H2S_Serial, CMD_GET_COMBINED_DATA, sizeof(CMD_GET_COMBINED_DATA));

  if (tb600b::h2s_isDataAvailable) {
    Serial.println("H2S Sensor Data:");
    Serial.printf("  Temperature: %.2f °C\n", tb600b::h2s_currentTemperature);
    Serial.printf("  Humidity: %.2f %%\n", tb600b::h2s_currentHumidity);
    Serial.printf("  Gas Concentration: %.2f ug/m^3\n", tb600b::h2s_currentGasUg);
  } else {
    Serial.println("H2S sensor data not available.");
  }

  delay(5000);
}
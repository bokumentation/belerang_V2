#include "tb600b_so2.h"
#include "Arduino.h"

namespace tb600b {
void get_combined_data(HardwareSerial& uart_port, const uint8_t *command, size_t commandSize) {
  const int responseLength = 13;
  uint8_t responseData[responseLength];

  Serial.println("SEND_CMD: get combined data ---");
  uart_port.write(command, commandSize);

  int bytesRead = uart_port.readBytes(responseData, responseLength);

  if (bytesRead == responseLength) {
    Serial.println("Received Combined Data Response:");
    // Arduino doesn't have a direct equivalent of ESP_LOG_BUFFER_HEXDUMP, so we'll print it manually
    for (int i = 0; i < bytesRead; i++) {
      if (responseData[i] < 0x10) Serial.print("0");
      Serial.print(responseData[i], HEX);
      Serial.print(" ");
    }
    Serial.println();

    if (responseData[0] == 0xFF && responseData[1] == 0x87) {
      int16_t rawTemperature = (int16_t)((responseData[8] << 8) | responseData[9]);
      float temperature = (float)rawTemperature / 100.0;
      uint16_t rawHumidity = (uint16_t)((responseData[10] << 8) | responseData[11]);
      float humidity = (float)rawHumidity / 100.0;
      uint16_t rawGasUg = (uint16_t)((responseData[2] << 8) | responseData[3]);
      float gasUg = (float)rawGasUg;

      Serial.printf("Temperature: %.2f °C\n", temperature);
      Serial.printf("Humidity: %.2f %%\n", humidity);
      Serial.printf("Gas Concentration: %.2f ug/m^3\n", gasUg);
    } else {
      Serial.println("Received malformed response header.");
    }
  } else {
    Serial.println("Failed to receive complete combined data response within timeout.");
  }

  delay(100);
}

namespace led {
void read_confirmation(HardwareSerial& uart_port) {
  const int responseLength = 2;
  uint8_t responseData[responseLength];
  int bytesRead = uart_port.readBytes(responseData, responseLength);

  if (bytesRead == responseLength) {
    if (responseData[0] == 0x4F && responseData[1] == 0x4B) {
      Serial.println("Received: 'OK' confirmation.");
    } else {
      Serial.println("Received unexpected response for confirmation.");
      for (int i = 0; i < bytesRead; i++) {
        if (responseData[i] < 0x10) Serial.print("0");
        Serial.print(responseData[i], HEX);
        Serial.print(" ");
      }
      Serial.println();
    }
  } else {
    Serial.println("Failed to receive 'OK' confirmation within timeout.");
  }
}

void read_status_response(HardwareSerial& uart_port) {
  const int responseLength = 9;
  uint8_t responseData[responseLength];
  int bytesRead = uart_port.readBytes(responseData, responseLength);

  if (bytesRead == responseLength) {
    Serial.println("Received Status Response:");
    for (int i = 0; i < bytesRead; i++) {
      if (responseData[i] < 0x10) Serial.print("0");
      Serial.print(responseData[i], HEX);
      Serial.print(" ");
    }
    Serial.println();

    if (responseData[2] == 0x01) {
      Serial.println("Light Status: ON (0x01)");
    } else if (responseData[2] == 0x00) {
      Serial.println("Light Status: OFF (0x00)");
    } else {
      Serial.println("Light Status: Unknown");
    }
  } else {
    Serial.println("Failed to receive status response within timeout.");
  }
}

void get_led_status(HardwareSerial& uart_port) {
  Serial.println("SEND_CMD: CHECK LED STATUS.");
  uart_port.write(CMD_GET_LED_STATUS, sizeof(CMD_GET_LED_STATUS));
  read_status_response(uart_port);
  delay(100);
}

void turn_off_led(HardwareSerial& uart_port) {
  Serial.println("SEND_CMD: TURN OFF LED.");
  uart_port.write(CMD_TURN_OFF_LED, sizeof(CMD_TURN_OFF_LED));
  read_confirmation(uart_port);
  delay(100);
}

void turn_on_led(HardwareSerial& uart_port) {
  Serial.println("SEND_CMD: TURN ON LED.");
  uart_port.write(CMD_TURN_ON_LED, sizeof(CMD_TURN_ON_LED));
  read_confirmation(uart_port);
  delay(100);
}
} // namespace led

void set_passive_mode(HardwareSerial& uart_port) {
  Serial.println("SEND_CMD: Switching Passive Mode or QnA.");
  uart_port.write(CMDSET_MODE_PASSIVE_UPLOAD, sizeof(CMDSET_MODE_PASSIVE_UPLOAD));
  delay(100);
}
} // namespace tb600b
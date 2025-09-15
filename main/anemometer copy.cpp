#include "Arduino.h"
#include "anemometer.h"
#include "esp_log.h"
#include "driver/gpio.h"

// ANEMOMETER PARAMETERS
int GPIO_pulse = 32; // PIN ESP32 = D32
const static char* pTAG_ANEMOMETER = "ANEMOMETER";

volatile byte rpmcount;
volatile unsigned long last_micros;
unsigned long timeold;
unsigned long timemeasure_ms = 10.00; // detik

float rotasi_per_detik;
float kecepatan_kilometer_per_jam;
float kecepatan_meter_per_detik;
volatile boolean flag = false;

void ICACHE_RAM_ATTR rpm_anemometer()
{
  flag = true;
  rpmcount++;
}

void anemometer_setup()
{
  gpio_install_isr_service(0);
  pinMode(GPIO_pulse, INPUT_PULLUP);
  digitalWrite(GPIO_pulse, LOW);
  detachInterrupt(digitalPinToInterrupt(GPIO_pulse));
  attachInterrupt(digitalPinToInterrupt(GPIO_pulse), rpm_anemometer, RISING);
  rpmcount = 0;
  timeold = 0;
  last_micros = micros();
}

void anemometer_loop()
{
  if (flag)
  {
    if (long(micros() - last_micros) >= 5000)
    {
      rpmcount++;
      last_micros = micros();
    }
    flag = false;
  }

  if ((millis() - timeold) >= timemeasure_ms * 1000)
  {
    detachInterrupt(digitalPinToInterrupt(GPIO_pulse));
    rotasi_per_detik = float(rpmcount) / float(timemeasure_ms);
    kecepatan_meter_per_detik = ((-0.0181 * (rotasi_per_detik * rotasi_per_detik)) + (1.3859 * rotasi_per_detik) + 1.4055);
    if (kecepatan_meter_per_detik <= 1.5)
    {
      kecepatan_meter_per_detik = 0.0;
    }
    kecepatan_kilometer_per_jam = kecepatan_meter_per_detik * 3.6;

    // Serial.println("rotasi_per_detik=");
    // Serial.print(rotasi_per_detik);
    ESP_LOGI(pTAG_ANEMOMETER, "Rotasi per detik: %f", rotasi_per_detik);
    
    // Serial.println("kecepatan_meter_per_detik=");
    // Serial.print(kecepatan_meter_per_detik);
    ESP_LOGI(pTAG_ANEMOMETER, "Kecepatan Meter per detik: %f", kecepatan_meter_per_detik);

    // Serial.println("kecepatan_kilometer_per_jam=");
    // Serial.print(kecepatan_kilometer_per_jam);
    ESP_LOGI(pTAG_ANEMOMETER, "Kecepatan KM/J: %f", kecepatan_kilometer_per_jam);
    
    // Serial.println("Waiting...");
    ESP_LOGI(pTAG_ANEMOMETER, "Waiting...");

    timeold = millis();
    rpmcount = 0;
    attachInterrupt(digitalPinToInterrupt(GPIO_pulse), rpm_anemometer, RISING);
  }

  delay(1);
}
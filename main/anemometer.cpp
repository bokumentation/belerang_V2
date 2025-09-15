#include "anemometer.h"
#include "esp_log.h"
#include "driver/gpio.h"

// ANEMOMETER PARAMETERS
const int GPIO_pulse = 32;
// const static char *pTAG_ANEMOMETER = "ANEMOMETER";

// Declare as volatile for safe access from ISR
volatile unsigned int rpmcount = 0;
unsigned long timeold = 0;
const unsigned long timemeasure_ms = 10000; // 10 seconds in milliseconds

// Latest calculated wind speeds
float current_speed_kmph = 0.0;
float current_speed_mps = 0.0;

// The ISR should be as simple as possible.
// It only increments the count.
void ICACHE_RAM_ATTR rpm_anemometer()
{
  rpmcount++;
}

void anemometer_setup()
{
  // Correct way to set up the GPIO for an input with a pull-up resistor.
  // The internal pull-up provides a default high state.
  pinMode(GPIO_pulse, INPUT_PULLUP);

  // Attach the interrupt once in the setup.
  // The interrupt will handle counting in the background.
  attachInterrupt(digitalPinToInterrupt(GPIO_pulse), rpm_anemometer, RISING);

  // Initialize timing variable.
  timeold = millis();
}

void anemometer_loop()
{
  // Check if the measurement interval has passed without blocking.
  if ((millis() - timeold) >= timemeasure_ms)
  {
    // Detaching the interrupt is not strictly necessary but
    // can prevent race conditions on multi-core systems when reading rpmcount.
    // It's a good practice to protect the variable during calculation.
    detachInterrupt(digitalPinToInterrupt(GPIO_pulse));

    // Use a temporary variable to hold the current count
    // to prevent it from changing mid-calculation.
    unsigned int current_rpmcount = rpmcount;

    // Reset the counter and timer for the next interval
    rpmcount = 0;
    timeold = millis();

    // Re-attach the interrupt
    attachInterrupt(digitalPinToInterrupt(GPIO_pulse), rpm_anemometer, RISING);

    // Perform calculations outside of the ISR and protected section.
    // The cast to float is important for accurate floating-point math.
    float rotations_per_second = (float)current_rpmcount / (timemeasure_ms / 1000.0);

    // Your conversion formula
    float speed_m_per_s = (-0.0181 * (rotations_per_second * rotations_per_second)) + (1.3859 * rotations_per_second) + 1.4055;

    // A simple conditional to handle low wind speeds.
    if (speed_m_per_s <= 1.5)
    {
      speed_m_per_s = 0.0;
    }

    float speed_km_per_h = speed_m_per_s * 3.6;

    // Store the calculated values in the global variables
    current_speed_kmph = speed_km_per_h;
    current_speed_mps = speed_m_per_s;

    // Log the results.
    // ESP_LOGI(pTAG_ANEMOMETER, "Rotations per second: %.2f", rotations_per_second);
    // ESP_LOGI(pTAG_ANEMOMETER, "Speed (m/s): %.2f", speed_m_per_s);
    // ESP_LOGI(pTAG_ANEMOMETER, "Speed (km/h): %.2f", speed_km_per_h);
    // ESP_LOGI(pTAG_ANEMOMETER, "Waiting for next measurement...");
  }
}

float getAnemometerSpeed_kmph() {
  return current_speed_kmph;
}

float getAnemometerSpeed_mps() {
  return current_speed_mps;
}
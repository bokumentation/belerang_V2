#ifndef ANEMOMETER_H
#define ANEMOMETER_H

#include "Arduino.h"

// Variables for anemometer
extern volatile unsigned int rpmcount; 
extern volatile unsigned long last_micros;

// Function prototypes
void anemometer_setup();
void anemometer_loop();

// Implementation 
float getAnemometerSpeed_kmph();
float getAnemometerSpeed_mps();


#endif // ANEMOMETER_H
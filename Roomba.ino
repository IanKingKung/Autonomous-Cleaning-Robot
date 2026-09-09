#include <Arduino.h>
#include <Wire.h>
#include <TFLI2C.h>
#include <Stepper.h>

//main file for Roomba code

TFLI2C luna;

int16_t distance;
int16_t flux;
int16_t temp;
int16_t luna_address = TFL_DEF_ADR;

// Stepper setup - moved to safe ESP32 GPIO pins (avoiding 6-11 flash pins)
const int STEPS_PER_REV = 2048;
Stepper stepper(STEPS_PER_REV, 18, 19, 25, 26);  
int current_step = 0;

void setup() {
  Serial.begin(115200);
  Wire.begin();          // Defaults to GPIO 21 (SDA) and GPIO 22 (SCL) on ESP32
  stepper.setSpeed(10);  // RPM
}

void loop() {
  // Step the motor 1 position forward
  stepper.step(1);
  current_step = (current_step + 1) % STEPS_PER_REV;

  // Read TF-Luna distance
  if (luna.getData(distance, flux, temp, luna_address)) {
    uint8_t header = 0xAA;
    uint16_t step_val = static_cast<uint16_t>(current_step);
    uint16_t dist_val = static_cast<uint16_t>(distance);  // distance in cm

    Serial.write(header);
    Serial.write((uint8_t*)&step_val, sizeof(step_val));
    Serial.write((uint8_t*)&dist_val, sizeof(dist_val));
  }
}
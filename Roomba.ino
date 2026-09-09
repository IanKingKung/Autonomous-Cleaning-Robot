//main script for Roomba logic

#include <Arduino.h>
#include <Wire.h>
#include <TFLI2C.h>
#include <Stepper.h>
#include "coordinate.h"
#include "grid.h"

TFLI2C luna;  //declare luna object

int16_t distance;
int16_t flux;
int16_t temp;
int16_t luna_address = TFL_DEF_ADR;

GRID local_map(100, 100, 0.05);  //Initialize Grid (100x100 cells, 0.05m / 5cm per cell = 5m x 5m coverage)

//Stepper setup
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
    float distance_meters = static_cast<float>(distance) / 100.0; //convert distance to meters
    POINT2D hit_point = steps_to_cartesian(distance_meters, current_step, STEPS_PER_REV);
    local_map.raytrace_and_mark(hit_point);
  }
}
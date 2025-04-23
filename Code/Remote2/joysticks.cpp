#include "globals.h"
#include "joysticks.h"

//Functions
void init_joysticks(){ //Init joysticks as input
  
  pinMode(LJX_PIN, INPUT); //Left X
  pinMode(LJY_PIN, INPUT); //Left Y

  Serial.println(F("Joysticks initialized."));
}

void check_joysticks() {
  byte ljxcen = 127;
  byte ljycen = 127;

  int ljx = map(analogRead(LJX_PIN), lj_min, lj_max, j_min, j_max);
  if (ljx != tm_data[10]) {
    if (ljx < jstick_center[1] && ljx > -jstick_center[1]) {
      ljx = 0;
    }
    tm_data[10] = ljx;
  }

  int ljy = map(analogRead(LJY_PIN), lj_min, lj_max, j_min, j_max);
  if (ljy != tm_data[11]) {
    if (ljy < jstick_center[1] && ljy > -jstick_center[1]) {
      ljy = 0;
    }
    tm_data[11] = ljy;
  }

  controlsUpdate = millis();
}
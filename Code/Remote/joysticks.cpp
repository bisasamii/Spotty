#include "globals.h"
#include "joysticks.h"

//Functions
void init_joysticks(){ //Init joysticks as input
  
  pinMode(LJX_PIN, INPUT); //Left X
  pinMode(LJY_PIN, INPUT); //Left Y

  pinMode(RJX_PIN, INPUT); //Right X
  pinMode(RJY_PIN, INPUT); //Right Y

  Serial.println(F("Joysticks initialized."));
}

void check_joysticks() {
  byte rjxcen = 127;
  byte rjycen = 127;
  byte ljxcen = 127;
  byte ljycen = 127;

  int rjx = map(analogRead(RJX_PIN), rj_min, rj_max, j_min, j_max);
  if (rjx != tm_data[12]) {
    if (rjx < jstick_center[1] && rjx > -jstick_center[1]) {
      rjx = 0;
    }
    tm_data[12] = rjx;

  }

  int ljx = map(analogRead(LJX_PIN), lj_min, lj_max, j_min, j_max);
  if (ljx != tm_data[10]) {
    if (ljx < jstick_center[1] && ljx > -jstick_center[1]) {
      ljx = 0;
    }
    tm_data[10] = ljx;
  }

  int rjy = map(analogRead(RJY_PIN), rj_min, rj_max, j_max, j_min);
  if (rjy != tm_data[13]) {
    if (rjy < jstick_center[1] && rjy > -jstick_center[1]) {
      rjy = 0;
    }
    tm_data[13] = rjy;
  }

  int ljy = map(analogRead(LJY_PIN), lj_min, lj_max, j_max, j_min);
  if (ljy != tm_data[11]) {
    if (ljy < jstick_center[1] && ljy > -jstick_center[1]) {
      ljy = 0;
    }
    tm_data[11] = ljy;
  }

  controlsUpdate = millis();
}
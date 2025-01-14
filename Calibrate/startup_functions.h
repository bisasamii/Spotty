#ifndef STARTUP_FUNCTIONS_H
#define STARTUP_FUNCTIONS_H

#include "globals.h"
#include <EEPROM.h>  // Ensure EEPROM library is available
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <Arduino.h>  // For Arduino-specific functions like Serial and delay


void loadNeutralPositions_EEPROM();
void saveToEEPROM(int motorIndex, int position);
void init_NRF24();
void setMotorsToNeutralPositions();
bool askUserForCalibration();
void startCalibration();
void takeStandbyPosition();

#endif

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "globals.h"
#include <EEPROM.h>  // Ensure EEPROM library is available
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <Arduino.h>  // For Arduino-specific functions like Serial and delay

//EEROM to save zero state (180°) for servos
void loadNeutralPositions_EEPROM();
void saveToEEPROM(int motorIndex, int position);
void calculateCorrectionValues();

//Initializations
void init_serial();
void init_servos();
void init_MP3();
void init_NRF24();
void init_MagnetSensor();

//Calibration
bool askUserForCalibration();
void startCalibration();

//Other functions
void get_remote_data();
void get_joystick_commands();
void check_magnetic_field();
void check_Remote();



#endif
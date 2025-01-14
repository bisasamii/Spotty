#ifndef LOOP_FUNCTIONS_H
#define LOOP_FUNCTIONS_H

#include "globals.h"
#include "startup_functions.h"
#include <EEPROM.h>  // Ensure EEPROM library is available
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <Arduino.h>  // For Arduino-specific functions like Serial and delay


void listen_serial_commands();
void get_remote_data();
void walk_forward();
void pushups(int durationInSeconds = -1);
void sit();
void down();
void paw(int durationInSeconds2 = -1);
void showoff();
#endif

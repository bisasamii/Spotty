#ifndef GLOBALS_H
#define GLOBALS_H

#include <Adafruit_PWMServoDriver.h>
#include <SPI.h>
#include <RF24.h>

//ROBOT DEFINITIONS
#define USMIN  500  // Minimum microsecond length for pulse
#define USMAX  2600 // Maximum microsecond length for pulse
#define SERVO_FREQ 60 // Analog servos run at ~50 Hz updates
#define SERVO_OSC 25000000
#define ANZAHL_MOT 12
#define NEUTRAL_INCREMENT 10  // Use a constant for calibration adjustments

// REMOTE DEFINITIONS
#define CE_PIN 9
#define CSN_PIN 10

//ROBOT DECLARATIONS
extern Adafruit_PWMServoDriver pwm;  // Declare pwm as extern
extern int neutral_pos; //Calculates the neutral position of the servo through MIN and MAX values
extern int neutralPositions[ANZAHL_MOT]; //stores neutral positions for all motors
extern int legPorts[4][3]; //stores what ports are connected to what leg
extern String currentPosition;
extern uint8_t ackData[15];

//REMOTE DECLARATIONS
extern RF24 radio; // Declare RF24 objectextern 
extern const uint8_t address[6]; // Declare remote addresses
//extern bool radioNumber; // Declare radio configuration
extern uint8_t receivedData[15]; // Declare data storage

#endif

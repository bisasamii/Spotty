#ifndef GLOBALS_H
#define GLOBALS_H

#include <Adafruit_PWMServoDriver.h>
#include <SPI.h>
#include <RF24.h>
#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>
#include <QMC5883LCompass.h>

//ROBOT DEFINITIONS
#define USMIN  500  // Minimum microsecond length for pulse
#define USMAX  2600 // Maximum microsecond length for pulse
#define SERVO_FREQ 60 // Analog servos run at ~50 Hz updates
#define SERVO_OSC 25000000
#define SERVO_DEGREERANGE 360
#define ANZAHL_MOT 12
#define NEUTRAL_INCREMENT 5  // Use a constant for calibration adjustments

//WALKING DEFINITIONS
#define COAX_SIZE 60.0
#define FEMUR_SIZE 107.5
#define TIBIA_SIZE 140.0

#define BODY_WIDTH 80.0
#define BODY_LENGTH 290.0

#define STEP_SIZE 40
#define STEP_DURATION 400
#define STEP_SWING_HEIGHT_FRONT 30
#define STEP_SWING_HEIGHT_REAR 30
#define STEP_STANCE_HEIGHT_FRONT 3
#define STEP_STANCE_HEIGHT_REAR 3
#define STEP_STANCE_TIME_PERCENTAGE 0.7

//NRF DEFINITIONS 
#define CE_PIN 9          //Defines which pins are used for the NRF Antenna
#define CSN_PIN 10        //
#define X_CENTER_LEFT 56
#define Y_CENTER_LEFT 205

//MAGNET-SENSOR DEFINITIONS
#define MAGNET_DIFF_THRESHOLD 500  //Diff Value for the Magnet-Sensor to detect an external magnetic field

//ROBOT DECLARATIONS
extern Adafruit_PWMServoDriver pwm;  // Declare pwm as extern
extern int neutral_pos; //Calculates the neutral position of the servo through MIN and MAX values
extern int neutralPositions[ANZAHL_MOT]; //stores neutral positions for all motors
extern int correctionValues[ANZAHL_MOT];
extern int legPorts[4][3]; //stores what ports are connected to what leg
extern String currentPosition;
extern uint8_t ackData[15];
extern int phase;

  //Robot Movement

//MP3 DECLARATIONS
//RX VON DF-PLAYER -> PIN 0 VON TEENSY (RX1), TX VON DF-PLAYER -> PIN 1 VON TEENSY (TX1)
extern SoftwareSerial mySoftwareSerial;
extern DFRobotDFPlayerMini myDFPlayer;

//NRF DECLARATIONS
extern RF24 radio; // Declare RF24 objectextern 
extern const uint8_t address[6]; // Declare remote addresses
//extern bool radioNumber; // Declare radio configuration
extern uint8_t receivedData[15]; // Declare data storage
extern bool remote_connected;

//MAGNET-SENSOR DEFINITIONS
extern QMC5883LCompass compass;
extern const int BASELINE_SAMPLES;  //How many samples are gathered before a baslineMagnitude has been established
extern float baselineMagnitude;     //The measured baseline Magintude of the Magnetic Sensor
extern bool magneticDetection;
extern unsigned long lastPlayTime;
extern const unsigned long cooldownTime;

//GLOBAL FUNCTIONS
int pulse_to_angle(int pulse);
int angle_to_pulse(int angle, bool reverse);
int deltaPulse_to_deltaAngle(int delta_pulse);


#endif

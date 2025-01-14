#include "globals.h"

//REMOTE
RF24 radio(CE_PIN, CSN_PIN); // Create RF24 object
const uint8_t address[6] = "1Node"; // Define the same address as on your remote
uint8_t receivedData[15]; // Array to store received data. Match size with remote's `tm_data`

//ROBOT
uint8_t ackData[15]= { //send data to remote
  0, 0, 0,        //battery, spd, volume
  0, 0, 0,        //move_steps, step_weight_factor_front, step_weight_factor_rear
  0, 0, 0, 0,     //remote_select, start_mode
  0, 0, 0, 0, 0,  //

};
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(); //Initzialize PWM Driver
int neutral_pos = USMIN + ((USMAX - USMIN) / 2);
int neutralPositions[ANZAHL_MOT];
String currentPosition = "neutral";

// Define motor ports for each leg
    int legPorts[4][3] = {  //LEG x MOTOR_POSITION (TIBIA, FEMUR, SHOULDER)
        {0, 1, 2},  // Front right leg (0)
        {3, 4, 5},  // Front left leg (1)
        {6, 7, 8},  // Behind right leg (2)
        {9, 10, 11} // Behind left leg (3)
    };
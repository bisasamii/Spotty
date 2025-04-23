#include "globals.h"

//MP3
SoftwareSerial mySoftwareSerial(0, 1);
DFRobotDFPlayerMini myDFPlayer;

//NRF
RF24 radio(CE_PIN, CSN_PIN); // Create RF24 object
const uint8_t address[6] = "1Node"; // Define the same address as on your remote
uint8_t receivedData[15]; // Array to store received data. Match size with remote's `tm_data`

//Magnet-SENSOR
QMC5883LCompass compass;
const int BASELINE_SAMPLES = 50;
float baselineMagnitude = 0.0;
bool magneticDetection = false;
unsigned long lastPlayTime = 0;
const unsigned long cooldownTime = 10000;

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

int pulse_to_angle(int pulse){
  float angle;
  //Check if given pulse is in bounds. If yes continue, if no get MIN/MAX values for allowed pulses
  if (pulse > USMAX){
    angle = SERVO_DEGREERANGE;
    return (int)angle;
  }
  else if (pulse < USMIN){
    angle = 0.0;
    return (int)angle;
  }
  else{
  //Calculate angle from pulse
  int pulse_bandwidth = USMAX - USMIN;
  float angle_per_pulse = (float)SERVO_DEGREERANGE/(float)pulse_bandwidth;
  int pulse_count = pulse - USMIN;

  angle = pulse_count * angle_per_pulse;
  

  return (int)angle;
  }

}

int angle_to_pulse(int angle){
  float pulse;
  //Check if given pulse is in bounds. If yes continue, if no get MIN/MAX values for allowed pulses
  if (angle > SERVO_DEGREERANGE){
    pulse = USMAX;
    return (int)pulse;
  }
  else if (angle < 0){
    pulse = USMIN;
    return (int)pulse;
  }
  else{
  //Calculate angle from pulse
  int pulse_bandwidth = USMAX - USMIN;
  float pulsewidth_per_degree = (float)pulse_bandwidth/(float)SERVO_DEGREERANGE;
  pulse = USMIN + (angle*pulsewidth_per_degree);

  return (int)pulse;
  }

}
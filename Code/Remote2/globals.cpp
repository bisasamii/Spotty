#include "globals.h"

//NRF
RF24 radio(9, 10);                 //DEFINE CE, CSN PIN HERE
uint8_t tm_data[data_num] = {     //transmit data array to robot
  0, 0, 0, 0,                     //btn1, btn2, btn3, btn4
  0, 0,                           //sel1, sel2
  0, 0, 0, 0,                     //p1, p2, p3, p4
  0, 0, 0, 0, 1       //lx, ly, rx, ry, MODE
};
uint8_t rc_data[data_num] = {     //receive data array from robot
  0, 0, 0,                        //battery, spd, volume
  0, 0, 0,                        //move_steps, step_weight_factor_front, step_weight_factor_rear
  0, 0, 0, 0,                     //remote_select, start_mode
  0, 0, 0, 0, 0                   //
};
uint8_t address[6] = "1Node";
bool tm_resp = false;
unsigned long lastNRFUpdate = 0;
unsigned int nrfInterval = 60;

//JOYSTICKS
int ljx = 0, ljy = 0;
int jstick_center[2] = {0};
int j_min = 0;
int j_max = 255;
int lj_min = 1023;
int lj_max = 0;

//MISC VARIABLES
unsigned int controlsInterval = 10;
unsigned long controlsUpdate = 0;
#ifndef GLOBALS_H
#define GLOBALS_H

#include <SPI.h>
#include <RF24.h>
#include <Wire.h>


// NRF24 Variables
extern RF24 radio;
constexpr int data_num = 15; //Number of Elements in Transmit/Receive Array
extern uint8_t tm_data[data_num]; //Transmit Data-Array
extern uint8_t rc_data[data_num]; //Recieve Data-Array
extern uint8_t address[6];        //Stores the NRF address
extern bool tm_resp;              //looks if the last transmission got a sucessful response
extern unsigned long lastNRFUpdate; //time since the last NRF Transmission
extern unsigned int nrfInterval;

// Joysticks
#define LJX_PIN A1
#define LJY_PIN A0
extern int ljx, ljy;
extern int jstick_center[2];
extern int j_min;
extern int j_max;
extern int lj_min;
extern int lj_max;


//Misc Variables
extern unsigned int controlsInterval; //Intervall in which controls state is being checked (buttons, joysticks...)
extern unsigned long controlsUpdate; //Time since the last update

#endif
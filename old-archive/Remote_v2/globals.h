#ifndef GLOBALS_H
#define GLOBALS_H

#include <SPI.h>
#include <RF24.h>
#include <Wire.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Version Information
extern float VERSION;

// NRF24 Variables
extern RF24 radio;
constexpr int data_num = 15; //Number of Elements in Transmit/Receive Array
extern uint8_t tm_data[data_num]; //Transmit Data-Array
extern uint8_t rc_data[data_num]; //Recieve Data-Array
extern uint8_t address[6];        //Stores the NRF address
extern bool tm_resp;              //looks if the last transmission got a sucessful response
extern unsigned long lastNRFUpdate; //time since the last NRF Transmission
extern unsigned int nrfInterval;

// OLED Configuration
#define OLED_ADDR 0x3D            //Oled I2C address for small screen
#define OLED_ADDR2 0x3C           ///Oled I2C address for big screen
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define SCREEN_WIDTH2 128
#define SCREEN_HEIGHT2 32
#define OLED_RESET -1
extern Adafruit_SSD1306 display; //object name for small screen
extern Adafruit_SSD1306 display2;//object name for big screen
extern const unsigned char smbmp [] PROGMEM; //Bitmap for startup animation
extern unsigned long lastRGBUpdate;
extern unsigned int displayInterval;
extern unsigned long lastDisplayUpdate;
extern unsigned int oledInterval;
extern unsigned long lastOLEDUpdate;
extern unsigned int oled2Interval;
extern unsigned long lastOLED2Update;

// LED Configuration
#define LED_STRIP_PIN 2          //Pin for LED signal into Mega
#define LED_STRIP_NUM 5          //Number of LEDs
extern Adafruit_NeoPixel led_strip;
extern int rgb_num;
extern int rgb_bright;
extern unsigned long lastRGBUpdate;
extern unsigned int rgbInterval;
extern int cur_rgb_val1[3];
extern int cur_rgb_val2[3];

// Slide Pots
#define POT1_PIN A8
#define POT2_PIN A9
#define POT3_PIN A10
#define POT4_PIN A11

extern int p1; //current value
extern int p1p;//previous value
extern int p2;
extern int p2p;
extern int p3;
extern int p3p;
extern int p4;
extern int p4p;
extern int pot_slack;     //leaves some play for the pot values to vary. Only value changes above the slack will refresh the values
extern int pot_min;        //if pots are orientated opposing to this configuration, 
extern int pot_max;      //swap these two values where min would be max value, and vice-versa

extern int per1;                         //pot percentage values
extern int per2;
extern int per3;
extern int per4;
extern int pper1;
extern int pper2;
extern int pper3;
extern int pper4;

// Joysticks
#define RJX_PIN A4
#define RJY_PIN A5
#define LJX_PIN A6
#define LJY_PIN A7
extern int rjx, rjy, ljx, ljy;
extern int jstick_center[2];

extern int j_min;
extern int j_max;
extern int rj_min;
extern int rj_max;
extern int lj_min;
extern int lj_max;

//Buttons           
                   // Fire Buttons
#define BTN1_PIN 3 //left front
#define BTN2_PIN 5 //right front
#define BTN3_PIN 4 //left back
#define BTN4_PIN 6 //right back
extern int b1;
extern int b1p;
extern int b2;
extern int b2p;
extern int b3;
extern int b3p;
extern int b4;
extern int b4p;

                    //Select Buttons
#define SEL1_PIN 9  //left green
#define SEL2_PIN 10 //right red
extern int selected_mode; //is the currently selected Mode
extern int mode_index; // the "highlighted" mode to be selected
extern int s1;
extern int s1p;
extern int s2;
extern int s2p;
extern int s3;
extern int s3p;


// Battery Monitor
#define BATT_MONITOR A3
extern float batt_voltage;
extern float batt_voltage2;

//Misc Variables
extern unsigned int controlsInterval; //Intervall in which controls state is being checked (buttons, joysticks...)
extern unsigned long controlsUpdate; //Time since the last update

// Function Prototypes
void check_mode();

#endif

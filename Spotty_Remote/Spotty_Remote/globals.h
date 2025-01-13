#ifndef GLOBALS_H
#define GLOBALS_H

#include <SPI.h>
#include <RF24.h>
#include <Wire.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

extern float VERSION;

//Settings
extern int debug;                    //general debugging
extern int debug2;                   //nrf24 serial comm
extern int debug3;                   //controls to serial
extern int debug4;                  //controls interactive test mode 
extern int debug5;                  //battery monitor 
extern int plotter;                  //control to plotter (also set debug3 = 1)

extern byte nrf_active;
extern byte batt_active;             //activate battery level monitoring
extern byte activity_active;         //activate inactivity monitor / sleep mode


//NRF24L01 transceiver
extern RF24 radio;
extern unsigned int nrfInterval;
extern unsigned long lastNRFUpdate;
extern uint8_t address[6];
constexpr int data_num = 14;  //number of elements in data arrays
extern bool tm_resp;
extern uint8_t tm_data[data_num];

//DEV: for currently unused two-way communication
extern uint8_t rc_data[data_num];

//slide pots (4)
#define POT1_PIN A8
#define POT2_PIN A9
#define POT3_PIN A10
#define POT4_PIN A11
extern int per1;                         //pot percentage values
extern int per2;
extern int per3;
extern int per4;
extern int pper1;
extern int pper2;
extern int pper3;
extern int pper4;

extern int p1;
extern int p1p;
extern int p2;
extern int p2p;
extern int p3;
extern int p3p;
extern int p4;
extern int p4p;
extern int pot_slack;
extern int pot_min;        //if pots are orientated opposing to this configuration, 
extern int pot_max;      //swap these two values where min would be max value, and vice-versa


//joysticks (2)
extern int jstick_center[2];
extern int j_min;          //if sticks are orientated opposing to this configuration, 
extern int j_max;        //swap these two values where min would be max value, and vice-versa

#define RJX_PIN A4
#define RJY_PIN A5
extern int rj_min;         //if sticks are orientated opposing to this configuration, 
extern int rj_max;      //swap these two values where min would be max value, and vice-versa

#define LJX_PIN A6
#define LJY_PIN A7   
extern int lj_min;      //if sticks are orientated opposing to this configuration, 
extern int lj_max;         //swap these two values where min would be max value, and vice-versa

//rgb leds (5)
#define LED_STRIP_PIN 2
#define LED_STRIP_NUM 5
extern Adafruit_NeoPixel led_strip;
extern unsigned int rgbInterval;
extern unsigned long lastRGBUpdate;
extern int rgb_del;
extern int current_led;
extern int fadeStep;
extern int fade_steps;
extern int fade_back;
extern int pattern;                  //set which light pattern is used
extern int pattern_int;            //set pattern delay between
extern int pattern_cnt;              //set number of loops of pattern
extern int pattern_step;
extern int cur_rgb_val1[3];
extern int cur_rgb_val2[3];
extern int rgb_num;
extern int rgb_bright;
extern byte set_led[5];

//"fire" buttons (4)
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

//select / stop buttons (2)
#define SEL1_PIN 9  //left green
#define SEL2_PIN 10 //right red
extern int s1;
extern int s1p;
extern int s2;
extern int s2p;
extern int s3;
extern int s3p;

//OLED screens (2)
//D20 = SDA
//D21 = SCL
#define OLED_ADDR 0x3D
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
extern Adafruit_SSD1306 display;
extern unsigned int oledInterval;
extern unsigned long lastOLEDUpdate;
extern unsigned int displayInterval;
extern unsigned long lastDisplayUpdate;
  
#define OLED_ADDR2 0x3C
#define SCREEN_WIDTH2 128
#define SCREEN_HEIGHT2 32
extern Adafruit_SSD1306 display2;
extern unsigned int oled2Interval;
extern unsigned long lastOLED2Update;

//battery monitor
#define BATT_MONITOR A3
extern unsigned long batteryInterval;             //60 seconds
extern unsigned long lastBatteryUpdate;
extern int batt_cnt;
extern float batt_voltage;                          //approx fully charged battery minimum nominal voltage
extern float batt_voltage_prev;                     //comparison voltage to prevent false positives
extern float batt_voltage2;                        //approx fully charged Robot battery minimum nominal voltage
extern float batt_voltage_prev2;                   //comparison voltage to prevent false positives
extern float avg_volts;
extern float batt_levels[10];

//misc vars
extern unsigned int controlsInterval;
extern unsigned long controlsUpdate;

extern byte inactivity_set;
extern int inactivity_loop;
extern int inactivity_cnt;
extern unsigned int inactivityInterval;
extern unsigned long inactivityUpdate;


//spotmicro bmp for oled
extern const unsigned char smbmp [] PROGMEM;


#endif
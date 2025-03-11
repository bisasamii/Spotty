//========================================
//  NRF24 Pin Wiring for Arduino Mega/Nano:
//  pin 7/7 -> CE pin
//  pin 8/8 -> CSN pin
//  pin 52/13 -> SCK pin
//  pin 51/11 -> MOSI pin
//  pin 50/12 -> MISO pin
//  pin xx -> IRQ pin (unused)
//========================================

#include "globals.h"
#include "nrf.h"
#include "pots.h"
#include "joysticks.h"
#include "led.h"
#include "buttons.h"
#include "oled.h"


void setup(){
  //Serial.begin(19200);
  //while (!Serial) {
  //}

  delay(2000);

  //INIT HARDWARE
  if (!radio.begin()){  //Init NRF
    Serial.println(F("NRF is not responding!"));
  }
  else {
    init_nrf(); 
  }
  
  init_pots();  //Init slide potentiometers
  init_joysticks(); //Init joysticks
  init_led(); //Init led strip
  init_buttons(); //Init fire and select buttons
  init_oled(); //Init oled screens

  //STARTUP SEQUENCE
  startup_led();
  startup_oled();

  //CONNECT REMOTE TO ROBOT
  tm_resp = send_remote_data();
}

void loop() {
  
  if (millis() - controlsUpdate > controlsInterval) { //check the state of the remote and its components after enough time has passed since the last transmission
    check_pots();
    check_buttons(); //checks buttons and displays respective menu on oled screen
    check_joysticks();
    check_mode();
  }

  if (millis() - lastNRFUpdate > nrfInterval) tm_resp = send_remote_data();

  if (millis() - lastRGBUpdate > rgbInterval) led_pat(0);
  if (millis() - lastOLEDUpdate > oledInterval) oled_refresh();
  if (millis() - lastOLED2Update > oled2Interval) oled2_refresh();
  if (millis() - lastDisplayUpdate > displayInterval) update_display(2);


}
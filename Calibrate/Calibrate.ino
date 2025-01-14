#include <Wire.h>
#include "globals.h"
#include "startup_functions.h"
#include "loop_functions.h"



void setup() {
  //Start serial communication and wait for connection
  Serial.begin(9600);
  while (!Serial) {
    ;
  }
  // Load neutral positions from EEPROM
  loadNeutralPositions_EEPROM();

  //Write saved neutral positions to motors
  setMotorsToNeutralPositions();

  //Ask User to enter Calibration-Setup
  bool calibrationCheck = askUserForCalibration();
  if (calibrationCheck) {
    startCalibration();
  }

  //set robot to standby Position
  takeStandbyPosition();
  Serial.println("Robot is now in standby position.");


  // NRF24 Initialization
  init_NRF24();
  Serial.println("Initialization process for NRF24 ended.");

}

void loop() {
  
  listen_serial_commands();

  // Check if data is available
    if (radio.available()) {
       get_remote_data();  //Gets Data from Remote and also sends Robot Data back to the Remote
    }

  switch (ackData[6]){    //Look what mode the Robot is in and run the respective code
    case 1: //Walking

    break;
    case 2: //Freestyle
    //In this Mode the Robot will not walk but move while staying in the same spot.
    //Fire buttons each start a different move. Sel2 will act as Stop button.

    //Check BTN1
    if (receivedData[0] == 1){
      Serial.println("BTN1 Pressed");
      pushups();
    }
    
    //Check BTN2
    if (receivedData[1] == 1){
      Serial.println("BTN2 Pressed");
      sit();
    }
    //Check BTN3
    if (receivedData[2] == 1){
      Serial.println("BTN3 Pressed");
      down();
    }
    //Check BTN4
    if (receivedData[3] == 1){
      Serial.println("BTN4 Pressed");
      takeStandbyPosition();
    }
    break;

    case 3: //Trot
    
    break;

    case 4: //Options
    
    break;

  }
}











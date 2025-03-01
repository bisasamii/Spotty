#include <Wire.h>
#include "globals.h"
#include "startup_functions.h"
#include "loop_functions.h"
#include "MP3_files.h"
#include "MyBody.h"
#include "MyLeg.h"
#include "ServoMotor.h"
MyBody robot;

void setup() {
  //Start serial communication and wait for connection
  Serial.begin(9600);
  while (!Serial) {
    ;
  }
  // Load neutral positions from EEPROM
  loadNeutralPositions_EEPROM();

  //Servo Initialization
  init_servos();
  robot.updateAllLegNeutralPositions();
  Serial.println("Initialization process for Servos ended.");
  
  //MP3 Initialization
  init_MP3();
  Serial.println("Initialization process for MP3-Module ended. ");

  //NRF24 Initialization
  init_NRF24();
  Serial.println("Initialization process for NRF24 ended.");

  //Magnet-Sensor Initialization
  init_MagnetSensor();
  Serial.println("Initialization process for Magnet Sensor ended.");

  //Write saved neutral positions to motors
  setMotorsToNeutralPositions();

  //Ask User to enter Calibration-Setup  #### Uncomment this for PC Serial Use
  bool calibrationCheck = askUserForCalibration();
  if (calibrationCheck) {
    startCalibration();
  }

  //set robot to standby Position
  takeStandbyPosition();
  Serial.println("Robot is now in standby position.");

  delay(1000);
  myDFPlayer.play(5);

}

void loop() {
  
  listen_serial_commands();

  // Check if data is available
    if (radio.available()) {
       get_remote_data();  //Gets Data from Remote and also sends Robot Data back to the Remote
    }

  /*switch (ackData[6]){    //Look what mode the Robot is in and run the respective code
    case 1: //Walking
    //Controls the robot with joysticks and checks for magnetic fields

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
    //In this mode you can reset the magnetic magnitude, calibrate the legs and set other global variables
    
    break;

  }*/
  

  //check_magnetic_field();
  
  //TEST
  Serial.println("TESTING");
  delay(2000);
  Serial.println("Neutral");
  robot.goNeutral();
  delay(2000);
  Serial.println("Moved");
  // Then move the same leg to new angles
  robot.moveLeg(1, 170, 170, 170);
  delay(2000);
  Serial.println("END");
  delay(1000);
}











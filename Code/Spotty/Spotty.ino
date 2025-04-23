#include <Wire.h>
#include "globals.h"
#include "functions.h"
#include "MyBody.h"
#include "MyLeg.h"
#include "ServoMotor.h"
MyBody robot;


void setup() {
  //Initialize all components
  delay(3000);
  //init_serial(); //TO USE WITHOUT PC THIS HAS TO BE COMMENTED OUT
  init_servos();
  init_MP3(); 

  init_NRF24();
  Serial.println("Waiting for remote connection...");
  // Wait until the remote is connected
  remote_connected = false; // Ensure the flag is false at startup
  while (!remote_connected) {
    check_Remote();  // This function sets remote_connected = true if radio.available() is true
    delay(100);      // Short delay to avoid busy-waiting
  }
  // Remote is connected at this point
  Serial.println("Remote connected. Continuing setup...");

  init_MagnetSensor();
  // Load neutral positions from EEPROM and write them to the robot and then take neutral Position
  loadNeutralPositions_EEPROM();
  //Calc Correction Values
  calculateCorrectionValues();

  //Attach servos and set Values
  robot.mRightFront.mTibia.Attach(0, deltaPulse_to_deltaAngle(correctionValues[0]));    //FRONT-RIGHT
  robot.mRightFront.mTibia.setCenterPulse(neutralPositions[0]);
  robot.mRightFront.mTibia.SetLimits(120.0, 320.0);
  robot.mRightFront.mFemur.Attach(1, deltaPulse_to_deltaAngle(correctionValues[1]));
  robot.mRightFront.mFemur.setCenterPulse(neutralPositions[1]);
  robot.mRightFront.mFemur.SetLimits(110.0, 260.0);
  robot.mRightFront.mCoax.Attach(2, deltaPulse_to_deltaAngle(correctionValues[2]));
  robot.mRightFront.mCoax.setCenterPulse(neutralPositions[2]);
  robot.mRightFront.mCoax.SetLimits(150.0, 210.0);

  robot.mLeftFront.mTibia.Attach(3, deltaPulse_to_deltaAngle(correctionValues[3]));     //FRONT-LEFT
  robot.mLeftFront.mTibia.setCenterPulse(neutralPositions[3]);
  robot.mLeftFront.mTibia.SetLimits(120.0, 320.0);
  robot.mLeftFront.mFemur.Attach(4, deltaPulse_to_deltaAngle(correctionValues[4]));
  robot.mLeftFront.mFemur.setCenterPulse(neutralPositions[4]);
  robot.mLeftFront.mFemur.SetLimits(110.0, 260.0);
  robot.mLeftFront.mCoax.Attach(5, deltaPulse_to_deltaAngle(correctionValues[5]));
  robot.mLeftFront.mCoax.setCenterPulse(neutralPositions[5]);
  robot.mLeftFront.mCoax.SetLimits(150.0, 210.0);

  robot.mRightRear.mTibia.Attach(6, deltaPulse_to_deltaAngle(correctionValues[6]));     //REAR-RIGHT
  robot.mRightRear.mTibia.setCenterPulse(neutralPositions[6]);
  robot.mRightRear.mTibia.SetLimits(120.0, 320.0);
  robot.mRightRear.mFemur.Attach(7, deltaPulse_to_deltaAngle(correctionValues[7]));
  robot.mRightRear.mFemur.setCenterPulse(neutralPositions[7]);
  robot.mRightRear.mFemur.SetLimits(110.0, 260.0);
  robot.mRightRear.mCoax.Attach(8, deltaPulse_to_deltaAngle(correctionValues[8]));
  robot.mRightRear.mCoax.setCenterPulse(neutralPositions[8]);
  robot.mRightRear.mCoax.SetLimits(150.0, 210.0);

  robot.mLeftRear.mTibia.Attach(9, deltaPulse_to_deltaAngle(correctionValues[9]));     //REAR_LEFT
  robot.mLeftRear.mTibia.setCenterPulse(neutralPositions[9]);
  robot.mLeftRear.mTibia.SetLimits(120.0, 320.0);
  robot.mLeftRear.mFemur.Attach(10, deltaPulse_to_deltaAngle(correctionValues[10]));
  robot.mLeftRear.mFemur.setCenterPulse(neutralPositions[10]);
  robot.mLeftRear.mFemur.SetLimits(110.0, 260.0);
  robot.mLeftRear.mCoax.Attach(11, deltaPulse_to_deltaAngle(correctionValues[11]));
  robot.mLeftRear.mCoax.setCenterPulse(neutralPositions[11]);
  robot.mLeftRear.mCoax.SetLimits(150.0, 210.0);

  //Get the robot in the zero state to check if calibration is needed
  robot.goNeutral();
  delay(1000);

  //Log the init state for every Motor
  robot.logState();

  //Ask User to enter Calibration-Setup  #### Uncomment this for PC Serial Use
  //bool calibrationCheck = askUserForCalibration();
  //if (calibrationCheck) {
    //startCalibration();
  //}

  //Play Sniffing sound to end setup
  myDFPlayer.play(5);

  //wait before you enter the loop
  delay(2000);
}


void loop() {
  static unsigned int last_mills = 0;
  unsigned long now_mills = millis();
  robot.setGait(GAIT_TROT); // select WALK mode

  if (last_mills != now_mills)
  {
    last_mills = now_mills;

    if(phase == 0){
      robot.GoWalkPosition(now_mills, 900, 0.3);
      phase++;
    }

  if(phase == 1){
    check_Remote();
    if (remote_connected == true) {
      get_remote_data();  //Gets Data from Remote and also sends Robot Data back to the Remote
      get_joystick_commands();
    }
    Serial.print("CurrentPos: ");
    Serial.println(currentPosition);

    if(currentPosition == "forward"){
      robot.StartWalk(600);
    }

    else if(currentPosition == "backwards"){
      robot.StartWalk(600);
    }

    else if(currentPosition == "right"){
      robot.StartWalk(600);
      robot.WalkTurn(TURN_RIGHT);
    }

    else if(currentPosition == "left"){
      robot.StartWalk(600);
      robot.WalkTurn(TURN_LEFT);
    }

    else if(currentPosition == "middle"){
      robot.StopWalk(now_mills);
    }
  }
    // Execute asynch servo movements
    robot.update(now_mills);
    check_magnetic_field();
  }

    
}



#include <Wire.h>
#include "globals.h"
#include "functions.h"
#include "MyBody.h"
#include "MyLeg.h"
#include "ServoMotor.h"
MyBody robot;


void setup() {
  //Initialize all components
  init_serial();
  init_servos();
  //init_MP3();
  //init_NRF24();
  //init_MagnetSensor();

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

  //Log the init state for every Motor
  robot.logState();

  //Ask User to enter Calibration-Setup  #### Uncomment this for PC Serial Use
  bool calibrationCheck = askUserForCalibration();
  if (calibrationCheck) {
    startCalibration();
  }

  //Play Sniffing sound to end setup
  delay(1000);
  //myDFPlayer.play(5);
}


void loop() {
  //if (loopCount >= 600000) {
        //Serial.print("Loop stopped after X runs.");
        //while (1) { } // Halt execution indefinitely
    //}
  //Serial.println("-------------LOOPSTART---------------");
  static unsigned int last_mills = 0;
  unsigned long now_mills = millis();
  robot.setGait(GAIT_TROT); // select WALK mode

  if (last_mills != now_mills)
  {
    last_mills = now_mills;

    //Write data to the servo
    Serial.println("__STARTING TEST2__");
    test2(now_mills);

    // Execute asynch servo movements
    Serial.println("__STARTING UPDATE__");
    robot.update(now_mills);
  }
  //loopCount++;
}

// test walking
void test2(unsigned long now_mills)
{
  static int test_phase = 0;
  static unsigned int wait_until = 1000;

  if (now_mills < wait_until)
    return;

  if (test_phase == 0)
  {
    // Go to walk initial position
    // 900 = the going to walk position will be completed in 900ms
    // 0.3 = servo speed ramp up for 30% then 60% constant then ramp dowm for 30%)
    robot.GoWalkPosition(now_mills, 900, 0.3); // start stand-up at time delta=0ms then 1000ms pause (the stand-up duration is set to 900ms then 100ms do nothing)

    test_phase++;
    wait_until = now_mills + 3000; // wait 2s (where the movement home->walk position elapse 900ms)
    return;
  }
  else if (test_phase == 1)
  {
    robot.StartWalk(STEP_DURATION); // start walk at 600ms per step

    test_phase++;
    wait_until = now_mills + 10000; // walk for 10s
    return;
  }
}


#include "MyBody.h"
#include "globals.h"


#define MYDEG_TO_RAD(x) x*M_PI/180

MyBody::MyBody():
  mLeftFront(true,true), //left, front
  mLeftRear(true,false),
  mRightFront(false,true),
  mRightRear(false,false)
{
  mRightFront.setReverse(true);
  mRightRear.setReverse(true);

  mRightRear.setName("R rear");
  mRightFront.setName("R front");
  mLeftRear.setName("L rear");
  mLeftFront.setName("L front");

  //setGait(GAIT_WALK);

  mWalking = false;
  mTurning = TURN_NONE;

  mDegree_x = 0.0;
  mDegree_y = 0.0;
  mDegree_z = 0.0;
  mPosition_x = 0.0;
  mPosition_y = 0.0;
  mPosition_z = 0.0;
}

//----------------------------------FUNCTIONS-----------------------------------------------------------



void MyBody::goNeutral(){
  mLeftFront.neutral();
  mRightFront.neutral();
  mLeftRear.neutral();
  mRightRear.neutral();
}

void MyBody::update(unsigned long mills)
{
  //if (mWalking)
    //WalkUpdate(mills); // update 
  
  mLeftFront.update(mills);
  mLeftRear.update(mills);
  mRightFront.update(mills);
  mRightRear.update(mills);
}

void MyBody::logState(){
  mRightFront.logState();
  mLeftFront.logState();
  mRightRear.logState();
  mLeftRear.logState();
}

void MyBody::GoWalkPosition(unsigned long mills, unsigned long duration, double ramp)
{
  double leg_len = TIBIA_SIZE+FEMUR_SIZE;

  double front_x_walk_position = STEP_SIZE / 4;
  double rear_x_walk_position = -STEP_SIZE / 8;
  
  double front_y_walk_position = leg_len * 0.75;
  double rear_y_walk_position = leg_len * 0.76;
  
  double z_walk_position = 0;
    
  mLeftFront.GoPosition(mills, front_x_walk_position, front_y_walk_position ,z_walk_position, duration, ramp);
  mRightFront.GoPosition(mills, front_x_walk_position, front_y_walk_position, z_walk_position, duration, ramp);
  mLeftRear.GoPosition(mills, rear_x_walk_position, rear_y_walk_position, z_walk_position, duration, ramp);
  mRightRear.GoPosition(mills, rear_x_walk_position, rear_y_walk_position, z_walk_position, duration, ramp);
}
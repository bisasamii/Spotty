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

void MyBody::setGait(int mode)
{
  if (mode == GAIT_WALK)
  {
    mFootDelays.frontLeft = 0.00;
    mFootDelays.frontRight = 0.50;
    mFootDelays.rearLeft = 0.25;
    mFootDelays.rearRight = 0.75;
  }
  else if (mode == GAIT_TROT)
  {
    mFootDelays.frontLeft = 0.00;
    mFootDelays.frontRight = 0.50;
    mFootDelays.rearLeft = 0.50;
    mFootDelays.rearRight = 0.00;
  }
  else if (mode == GAIT_PACE)
  {
    mFootDelays.frontLeft = 0.00;
    mFootDelays.frontRight = 0.50;
    mFootDelays.rearLeft = 0.00;
    mFootDelays.rearRight = 0.50;

  }
  else if (mode == GAIT_CANTER)
  {
    mFootDelays.frontLeft = 0.00;
    mFootDelays.frontRight = 0.30;
    mFootDelays.rearLeft = 0.70;
    mFootDelays.rearRight = 0.00; 
  }
  else
  {
    mFootDelays.frontLeft = 0.00;
    mFootDelays.frontRight = 0.00;
    mFootDelays.rearLeft = 0.50;
    mFootDelays.rearRight = 0.50;
  }
}

void MyBody::goNeutral(){
  mLeftFront.neutral();
  mRightFront.neutral();
  mLeftRear.neutral();
  mRightRear.neutral();
}

void MyBody::update(unsigned long mills)
{
  if (mWalking)
    WalkUpdate(mills); // update 
  
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

void MyBody::WalkUpdate(unsigned long mills)
{
  unsigned long total_step_points = 50; // the foot trajectory is divided in 50 points
  unsigned long time_delta = mWalkDuration / total_step_points;

  if (mills - mLastUpdate < time_delta)
    return;


  mLastUpdate = mills;

  double perc_FL = (double)((mWalkStep + (int)(mFootDelays.frontLeft * total_step_points)) % total_step_points) / (double)total_step_points;
  double perc_FR = (double)((mWalkStep + (int)(mFootDelays.frontRight * total_step_points)) % total_step_points) / (double)total_step_points;
  double perc_RL = (double)((mWalkStep + (int)(mFootDelays.rearLeft * total_step_points)) % total_step_points) / (double)total_step_points;
  double perc_RR = (double)((mWalkStep + (int)(mFootDelays.rearRight * total_step_points)) % total_step_points) / (double)total_step_points;

  mRightFront.DoStep(mills, perc_FR, time_delta, mTurning);
  mLeftFront.DoStep(mills, perc_FL, time_delta, mTurning);
  mRightRear.DoStep(mills, perc_RR, time_delta, mTurning);
  mLeftRear.DoStep(mills, perc_RL, time_delta, mTurning);

  mWalkStep++;

  if (mWalkStep > total_step_points)
    mWalkStep = 0;
}

void MyBody::GoWalkPosition(unsigned long mills, unsigned long duration, double ramp)
{
  double leg_len = TIBIA_SIZE+FEMUR_SIZE;

  double front_x_walk_position = STEP_SIZE / 4;
  double rear_x_walk_position = STEP_SIZE / 4;
  
  double front_y_walk_position = leg_len * 0.8;
  double rear_y_walk_position = leg_len * 0.8;
  
  double z_walk_position = 0;
    
  mLeftFront.GoPosition(mills, front_x_walk_position, front_y_walk_position ,z_walk_position, duration, ramp);
  mRightFront.GoPosition(mills, front_x_walk_position, front_y_walk_position, z_walk_position, duration, ramp);
  mLeftRear.GoPosition(mills, rear_x_walk_position, rear_y_walk_position, z_walk_position, duration, ramp);
  mRightRear.GoPosition(mills, rear_x_walk_position, rear_y_walk_position, z_walk_position, duration, ramp);
}

void MyBody::StartWalk(unsigned long single_step_duration)
{
  if (mWalking)
    return;

  mWalking = true;
  mTurning = TURN_NONE;
  mWalkStep = 0;
  mWalkDuration = single_step_duration;

}
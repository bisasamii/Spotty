#ifndef MYBODY_H
#define MYBODY_H

#include "MyLeg.h"

#define GAIT_WALK 0
#define GAIT_TROT 1
#define GAIT_PACE 2
#define GAIT_CANTER 3

struct GaitFootDelays
{
  double frontLeft;
  double frontRight;
  double rearLeft;
  double rearRight;
};

class MyBody {
  public:
  MyBody();

public:
  MyLeg mLeftFront;
  MyLeg mLeftRear;
  MyLeg mRightFront;
  MyLeg mRightRear;

private:
  GaitFootDelays mFootDelays;

  bool          mWalking;
  unsigned long mWalkStep;
  unsigned long mLastUpdate;
  unsigned long mWalkDuration;
  unsigned long mTurning;

  double mDegree_x;
  double mDegree_y;
  double mDegree_z;
  double mPosition_x;
  double mPosition_y;
  double mPosition_z;

public:
void goNeutral();
void update(unsigned long mills);
void GoWalkPosition(unsigned long mills, unsigned long duration, double ramp = 0.0);
void StartWalk(unsigned long single_step_duration);
void WalkTurn(unsigned long value);
void StopWalk(unsigned long mills);
void WalkUpdate(unsigned long mills);
void logState();
void setGait(int mode);
  
};

#endif
#ifndef MYBODY_H
#define MYBODY_H

#include "MyLeg.h"

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
void logState();
  
};

#endif
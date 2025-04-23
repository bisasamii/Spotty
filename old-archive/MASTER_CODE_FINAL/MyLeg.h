#ifndef _MyLeg_h
#define _MyLeg_h

#include "ServoMotor.h"

#define TURN_NONE 0
#define TURN_LEFT 1
#define TURN_RIGHT 2

class MyLeg
{
public:
  MyLeg(bool left, bool front);

public:
  ServoMotor mTibia;
  ServoMotor mFemur;
  ServoMotor mCoax;

  char * mName;
  coords mPosition;
  bool   mFront;
  bool   mLeft;

  void update(unsigned long mills);
  void neutral();
  void GoPosition(unsigned long mills, double x, double y, double z, unsigned long duration, double ramp = 0.0);
  void GoPosition(unsigned long mills, coords deltax, unsigned long duration, double ramp = 0.0);
  void logState();
  void DoStep(unsigned long mills, double percentage, unsigned long duration, unsigned long turning);

  //----GET-----
  coords getPosition() {return mPosition;}

  //-----SET----
  void setReverse(bool value);
  void setName(const char * name);

private:
  angle IKSolver(double x, double y, double z);
  coords StepCurve(double percentage, unsigned long turning);

};
#endif
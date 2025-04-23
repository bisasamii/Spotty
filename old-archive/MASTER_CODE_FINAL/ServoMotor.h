#ifndef SERVOMOTOR_H
#define SERVOMOTOR_H

#include <Adafruit_PWMServoDriver.h>

struct coords
{
  double x;
  double y;
  double z;
};

struct angle
{
    double coax_degree;
    double femur_degree;
    double tibia_degree;
};

struct Movement
{
	double         degree;
	unsigned long duration;
	double         ramp;
};

class ServoMotor {
public:
	ServoMotor();
	~ServoMotor();

private:
	int    mId;
  bool   mReverse;
  char * mName;
  double mCenterPulse;
  double mCurrentPulse;
  double mCorrectionPulse; //PULSE

	double mDegreeRange;

  double mCorrection; //ANGULAR
	double mHome;
	double mLimitLow;
	double mLimitHigh;

  double mTargetDegree;
	double mCurrentDegree;
	double mStartDegree;
	double mRamp;
	unsigned long mStartTime;
	unsigned long mEndTime;

	bool mIsMoving;

	double CutOffDegrees(double degree);
  double CutOffRamp(double ramp);

  public:
  void update(unsigned long mills);
  void Attach(int id, double correction);
  void moveToAngle(double angle);
  void printAllServoValues();
  void write_degree(double degree);
  void StartMovement(unsigned long mills, double degree, unsigned long duration, double ramp = 0.0);

//--------GET-------------------
  int           getId() {return mId;}
  bool          getReverse() {return mReverse;}
  char *        getName() {return mName;}
  double        getCenterPulse() {return mCenterPulse;}
  double        getCurrentPulse() {return mCurrentPulse;}
  double        getDegreeRange() {return mDegreeRange;}
  double        getCorrection() {return mCorrection;}
  double        getCorrectionPulse() {return mCorrectionPulse;}
  double        getHome() {return mHome;}
  double        getTargetDegree() {return mTargetDegree;}
  double        getStartDegree() {return mStartDegree;}
  double        getRamp() {return mRamp;}
  unsigned long getStartTime() {return mStartTime;}
  bool          getIsMoving() {return mIsMoving;}
  unsigned long getEndTime() {return mEndTime;}
  double        getMin() {return mLimitLow;}
  double        getMax() {return mLimitHigh;}
  double        getCurrentDegree() {return mCurrentDegree;}

//---------SET--------------
  void setReverse(bool value) {mReverse = value;}
  void setName(const char * owner, const char * name);
  void setCenterPulse(double centerPulse);
  void SetLimits(double low, double high);

};
#endif
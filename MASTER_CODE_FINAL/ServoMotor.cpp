#include <string.h>
#include "ServoMotor.h"
#include "globals.h"

ServoMotor::ServoMotor()
{
	mId = -1;
	mTargetDegree = -1;
	mCurrentDegree = -1;
	mIsMoving = false;
  mReverse = false;
  mName = NULL;
}

ServoMotor::~ServoMotor()
{
  if (mName)
    delete []mName;
}

void ServoMotor::update(unsigned long mills)
{
  if (mId < 0)
    return;

	if (mIsMoving)
	{
		if (mills >= mEndTime) //End of movement is reached: Write the target angle to the motor and set moving variable to false
		{

			write_degree(mTargetDegree);
			mIsMoving = false;
		}
		else //Time is smaller than end time which means that the movement is still performing
		{
      if (mTargetDegree == mCurrentDegree) //If Target is reached return
        return;
    
      double perc = (double)(mills - mStartTime) / (double)(mEndTime - mStartTime);

		  double pos_degree;

      if (mRamp > 0.0)
      {
				double velocitymax = (mTargetDegree - mStartDegree) / (1.0 - mRamp);

				double velocity;

        if (perc < mRamp) // ramp UP
				{
					velocity = perc / mRamp *  velocitymax;

					pos_degree = mStartDegree + perc * velocity / 2.0;

				}
				else if (perc < (1.0 - mRamp)) // constant velocity
				{
					velocity = velocitymax;

					pos_degree = mStartDegree + (mRamp / 2.0 + perc - mRamp) * velocitymax;

				}
				else // ramp DOWN
				{
					velocity = (1.0 - perc) / mRamp * velocitymax;

					pos_degree = mStartDegree + (1.0 - mRamp) * velocitymax - velocity * (1.0 - perc) / 2;

				}
      }
      else // No ramp
      {
		    pos_degree = mStartDegree + (mTargetDegree - mStartDegree) * perc;
        
      }
			write_degree(pos_degree);
		}
	}
}

void ServoMotor::write_degree(double degree)
{
	  if (mId < 0)
			return;

      // Instead of adding mCorrection (in degrees), use the pulse correction as in moveToAngle.
  // First, ensure the degree is within bounds:
  degree = CutOffDegrees(degree);
  
  // Calculate the corrected pulse using the same method as in moveToAngle:
  int value_in_pulse = angle_to_pulse(degree, mReverse) - mCorrectionPulse;
  
  // Update mCurrentDegree (using the non-corrected angle):
  mCurrentDegree = degree;
  if (value_in_pulse != mCurrentPulse) // Only write if there's a change
  {
    pwm.writeMicroseconds(mId, value_in_pulse);
    mCurrentPulse = value_in_pulse;
  }
}

void ServoMotor::Attach(int id, double correction)
{
	mId = id;
	//mHome = ; //Calculate this
  mCorrection = correction;
  mDegreeRange = SERVO_DEGREERANGE;
  mCorrectionPulse = correctionValues[id];

}

void ServoMotor::setName(const char * owner, const char * name)
{
  mName = new char[strlen(owner) + strlen(name) + 2];
  
  sprintf(mName, "%s-%s", owner, name);
}

void ServoMotor::moveToAngle(double angle)
{
  //Check reverse

  angle = CutOffDegrees(angle); //Checks if angle is inside bounds
  double microseconds = angle_to_pulse(angle, mReverse) - mCorrectionPulse;
  mCurrentDegree = angle;
  mCurrentPulse = microseconds;
	pwm.writeMicroseconds(mId, microseconds);

}

void ServoMotor::printAllServoValues() {
    Serial.println("########################################");
    Serial.print("VALUES FOR: ");
    Serial.println(getName());
    Serial.print("Id: ");
    Serial.println(getId());
    Serial.print("IsMoving: ");
    Serial.println(getIsMoving());
    Serial.print("ReverseValue: ");
    Serial.println(getReverse());
    Serial.print("CenterPulse: ");
    Serial.println(getCenterPulse());
    Serial.print("CurrentPulse: ");
    Serial.println(getCurrentPulse());
    Serial.print("DegreeRange: ");
    Serial.println(getDegreeRange());
    Serial.print("Correction: ");
    Serial.println(getCorrection());
    Serial.print("CorrectionPulse: ");
    Serial.println(getCorrectionPulse());
    Serial.print("Home: ");
    Serial.println(getHome());
    Serial.print("CurrentDegree: ");
    Serial.println(getCurrentDegree());
    Serial.print("TargetDegree: ");
    Serial.println(getTargetDegree());
    Serial.print("StartDegree: ");
    Serial.println(getStartDegree());
    Serial.print("Ramp: ");
    Serial.println(getRamp());
    Serial.print("StartTime: ");
    Serial.println(getStartTime());
    Serial.print("EndTime: ");
    Serial.println(getEndTime());
    Serial.print("Min: ");
    Serial.println(getMin());
    Serial.print("Max: ");
    Serial.println(getMax());
    Serial.println("########################################");
}

void ServoMotor::setCenterPulse(double centerPulse){
  mCenterPulse = centerPulse;
}

void ServoMotor::StartMovement(unsigned long mills, double degree, unsigned long duration, double ramp)
{
	degree = CutOffDegrees(degree);
  ramp = CutOffRamp(ramp);

  mTargetDegree = degree;
  mStartDegree = mCurrentDegree;
  mStartTime = mills;
  mEndTime = mStartTime + duration;
  mRamp = ramp;

  mIsMoving = true;
}

void ServoMotor::SetLimits(double low, double high)
{
	  mLimitLow = low;
		if (mLimitLow < 0.0)
			mLimitLow = 0.0;

		mLimitHigh = high;
		if (mLimitHigh > mDegreeRange)
			mLimitHigh = mDegreeRange;
}

double ServoMotor::CutOffDegrees(double degree)
{
	if (degree < mLimitLow)
		return mLimitLow;
	else if (degree > mLimitHigh)
		return mLimitHigh;
  return degree;
}

double ServoMotor::CutOffRamp(double ramp)
{
  if (ramp < 0)
    return 0;
  if (ramp > 0.4)
    return 0.4;
  return ramp;
}
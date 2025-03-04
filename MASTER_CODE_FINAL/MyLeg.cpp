#include "MyLeg.h"
#include "globals.h"

#define square(x) x*x

MyLeg::MyLeg(bool left, bool front)
{
  mFront = front;
  mLeft = left;
}

void MyLeg::setName(const char * name) 
{
  mName = new char[strlen(name) + 1];
  strcpy(mName, name);

  mTibia.setName(mName,"Tibia");
  mFemur.setName(mName,"Femur");
  mCoax.setName(mName,"Coax");
}

void MyLeg::setReverse(bool value)
{
  mTibia.setReverse(value);
  mFemur.setReverse(value);
  mCoax.setReverse(value);
}

//------------PUBLIC FUNCTIONS-------------------

void MyLeg::update(unsigned long mills)
{
  mTibia.update(mills);
  mFemur.update(mills);
  mCoax.update(mills);
}

void MyLeg::neutral() {
  mTibia.moveToAngle(SERVO_DEGREERANGE/2);
  mFemur.moveToAngle(SERVO_DEGREERANGE/2);
  mCoax.moveToAngle(SERVO_DEGREERANGE/2);
}

void MyLeg::logState(){
  mTibia.printAllServoValues();
  mFemur.printAllServoValues();
  mCoax.printAllServoValues();
}

void MyLeg::GoPosition(unsigned long mills, double x, double y, double z, unsigned long duration, double ramp)
{
  angle a = IKSolver(x, y, z);

  mPosition.x = x;
  mPosition.y = y;
  mPosition.z = z;

  // Log the input coordinates
  Serial.print("GoPosition called with: X=");
  Serial.print(x);
  Serial.print(", Y=");
  Serial.print(y);
  Serial.print(", Z=");
  Serial.println(z);

  // Log the calculated angles
  Serial.print("Calculated Angles -> Coax: ");
  Serial.print(a.coax_degree);
  Serial.print("°, Femur: ");
  Serial.print(a.femur_degree);
  Serial.print("°, Tibia: ");
  Serial.print(a.tibia_degree);
  Serial.println("°");


  mTibia.StartMovement(mills, a.tibia_degree, duration, ramp);
  mFemur.StartMovement(mills, a.femur_degree, duration, ramp);
  mCoax.StartMovement(mills, a.coax_degree, duration, ramp); 
}

//------------PRIVATE FUNCTIONS-------------------
angle MyLeg::IKSolver(double x, double y, double z)
{
  z = z + COAX_SIZE;

  double d1 = sqrt(square(z) + square(y) - square(COAX_SIZE));
  double d2 = sqrt(square(d1) + square(x));
  double d3 = (square(d2) - square(FEMUR_SIZE) - square(TIBIA_SIZE)) / (2 * FEMUR_SIZE * TIBIA_SIZE);

  double theta1 = -atan2(y, z) - atan2(d1, -COAX_SIZE);
  double theta3 = acos(d3);
  double theta2 = atan2(x,d1) - atan2(TIBIA_SIZE * sin(theta3), FEMUR_SIZE + TIBIA_SIZE * cos(theta3));

  angle a;

  theta1 = theta1 + (2*M_PI);
  theta2 = theta2 + M_PI;
  theta3 = theta3 + M_PI;

  if (theta1 >= 0)
    a.coax_degree  = 180.0 * theta1 / M_PI;
  else
    a.coax_degree  = 180 + 180.0 * theta1 / M_PI;

  if (theta2 >= 0)
    a.femur_degree  =  180.0 * theta2 / M_PI;
  else
    a.femur_degree  = 180 + 180.0 * theta2 / M_PI;

  if (theta3 >= 0)
    a.tibia_degree  = 180.0 * theta3 / M_PI;
  else
    a.tibia_degree  = 180 + 180.0 * theta3 / M_PI;

  return a;
}
#ifndef MYLEG_H
#define MYLEG_H

#include "ServoMotor.h"

class MyLeg {
  public:
    // Constructor: each leg is defined by three fixed servo ports.
    MyLeg(int portTibia, int portFemur, int portShoulder);
    
    void updateNeutralPositions();  // New method to update servos
    // Moves the leg by setting angles for Tibia, Femur, and Shoulder.
    void moveLeg(int tibiaAngle, int femurAngle, int shoulderAngle);
    
  private:
    ServoMotor tibia;
    ServoMotor femur;
    ServoMotor shoulder;

    // Store the port numbers so we can access the updated EEPROM value later
    int _portTibia, _portFemur, _portShoulder;
};

#endif

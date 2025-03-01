#ifndef SERVOMOTOR_H
#define SERVOMOTOR_H

#include <Adafruit_PWMServoDriver.h>

class ServoMotor {
  public:
    // Constructor: pass the servo port number (on the PCA board)
    ServoMotor(int port, int EEROM_neutral_pos);
    // Moves the servo to the specified angle
    void moveTo(int angle);
    void updateNeutralPosition (int newNeutral);
    
  private:
    int _port;
    int _EEROM_neutral_pos;
    int _pulseCorrectionValue;
};

#endif

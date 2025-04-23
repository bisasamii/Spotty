#include "ServoMotor.h"
#include "globals.h"

ServoMotor::ServoMotor(int port, int EEROM_neutral_pos) {
  _port = port;
  _EEROM_neutral_pos = EEROM_neutral_pos;
  _pulseCorrectionValue = neutral_pos - _EEROM_neutral_pos;
}

void ServoMotor::updateNeutralPosition(int newNeutral) {
  _EEROM_neutral_pos = newNeutral;
  _pulseCorrectionValue = neutral_pos - _EEROM_neutral_pos;
}

void ServoMotor::moveTo(int angle) {
  int microseconds = angle_to_pulse(angle) - _pulseCorrectionValue;
  pwm.writeMicroseconds(_port, microseconds);
}

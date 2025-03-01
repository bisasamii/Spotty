#include "MyLeg.h"
#include "globals.h"

MyLeg::MyLeg(int portTibia, int portFemur, int portShoulder)
  : tibia(portTibia, neutralPositions[portTibia]), 
  femur(portFemur, neutralPositions[portFemur]), 
  shoulder(portShoulder, neutralPositions[portShoulder]),
  _portTibia(portTibia), _portFemur(portFemur), _portShoulder(portShoulder) 
{}

void MyLeg::updateNeutralPositions() {
  tibia.updateNeutralPosition(neutralPositions[_portTibia]);
  femur.updateNeutralPosition(neutralPositions[_portFemur]);
  shoulder.updateNeutralPosition(neutralPositions[_portShoulder]);
}

void MyLeg::moveLeg(int tibiaAngle, int femurAngle, int shoulderAngle) {
  tibia.moveTo(tibiaAngle);
  femur.moveTo(femurAngle);
  shoulder.moveTo(shoulderAngle);
}


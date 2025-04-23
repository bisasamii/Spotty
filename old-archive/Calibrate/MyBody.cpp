#include "MyBody.h"
#include "globals.h"

MyBody::MyBody()
  : legs{
      MyLeg(legPorts[0][0], legPorts[0][1], legPorts[0][2]), //Front Right = 0
      MyLeg(legPorts[1][0], legPorts[1][1], legPorts[1][2]), //Front Left = 1
      MyLeg(legPorts[2][0], legPorts[2][1], legPorts[2][2]), //Back Right = 2
      MyLeg(legPorts[3][0], legPorts[3][1], legPorts[3][2]) //Back Left = 3
    }
{}

void MyBody::updateAllLegNeutralPositions(){
  for (int i = 0; i < 4; i++){
    legs[i].updateNeutralPositions();
  }
}

void MyBody::moveLeg(int legIndex, int tibiaAngle, int femurAngle, int shoulderAngle) {
  if (legIndex >= 0 && legIndex < 4) {
    legs[legIndex].moveLeg(tibiaAngle, femurAngle, shoulderAngle);
  }
}

void MyBody::goNeutral(){
  for(int legIndex = 0; legIndex < 4; legIndex++){
    legs[legIndex].moveLeg(180, 180, 180);
  }
}
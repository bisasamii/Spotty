#ifndef MYBODY_H
#define MYBODY_H

#include "MyLeg.h"

class MyBody {
  public:
    MyBody();

    // Moves a specific leg by index using provided angles
    void updateAllLegNeutralPositions();
    void moveLeg(int legIndex, int tibiaAngle, int femurAngle, int shoulderAngle);
    void goNeutral();
    
  private:
    MyLeg legs[4];  // Four legs of the quadruped
};

#endif

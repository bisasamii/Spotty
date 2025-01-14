#include "globals.h"
#include "pots.h"

//Functions
void init_pots(){ //Init pots as input
  pinMode(POT1_PIN, INPUT);
  pinMode(POT2_PIN, INPUT);
  pinMode(POT3_PIN, INPUT);
  pinMode(POT4_PIN, INPUT);

  Serial.println(F("Pots initialized."));
}

void check_pots() {
  int updis = 0;

  int p1c = analogRead(POT1_PIN);
  if (p1c != p1p) {
    //double-check jumpy pots
    if (p1c == analogRead(POT1_PIN)) {
      p1 = map(p1c, 0, 1023, pot_max, pot_min);
      if (p1 > pot_slack) {
        if (p1c > (p1p + pot_slack) || p1c < (p1p - pot_slack)) {
        }
        p1p = p1c;
        tm_data[6] = p1;
  
        per1 = map(p1, pot_max, pot_min, 100, -100); // convert reading to a percentage
        if (per1 < 3 && per1 > -3) per1 = 0;
        if (per1 != pper1) {
          pper1 = per1;
          updis = 1;
        }
        
      }
    }
  }

  int p2c = analogRead(POT2_PIN);
  if (p2c != p2p) {
    //double-check jumpy pots
    if (p2c == analogRead(POT2_PIN)) {
      p2 = map(p2c, 0, 1023, pot_max, pot_min);
      if (p2 > pot_slack) {
        if (p2c > (p2p + pot_slack) || p2c < (p2p - pot_slack)) {
        }
        p2p = p2c;
        tm_data[7] = p2;
  
        per2 = map(p2, pot_max, pot_min, 100, -100); // convert reading to a percentage
        if (per2 < 3 && per2 > -3) per2 = 0;
        if (per2 != pper2) {
          pper2 = per2;
          updis = 1;
        }
        
      }
    }
  }

  int p3c = analogRead(POT3_PIN);
  if (p3c != p3p) {
    //double-check jumpy pots
    if (p3c == analogRead(POT3_PIN)) {
      p3 = map(p3c, 0, 1023, pot_max, pot_min);
      if (p3 > pot_slack) {
        if (p3c > (p3p + pot_slack) || p3c < (p3p - pot_slack)) {
        }
        p3p = p3c;
        tm_data[8] = p3;
  
        per3 = map(p3, pot_max, pot_min, 100, -100); // convert reading to a percentage
        if (per3 < 3 && per3 > -3) per3 = 0;
        if (per3 != pper3) {
          pper3 = per3;
          updis = 1;
        }
        
      }
    }
  }

  int p4c = analogRead(POT4_PIN);
  if (p4c != p4p) {
    //double-check jumpy pots
    if (p4c == analogRead(POT4_PIN)) {
      p4 = map(p4c, 0, 1023, pot_max, pot_min);
      if (p4 > pot_slack) {
        if (p4c > (p4p + pot_slack) || p4c < (p4p - pot_slack)) {
        }
        p4p = p4c;
        tm_data[9] = p4;
  
        per4 = map(p4, pot_max, pot_min, 100, -100); // convert reading to a percentage
        if (per4 < 3 && per4 > -3) per4 = 0;
        if (per4 != pper4) {
          pper4 = per4;
          updis = 1;
        }
        
      }
    }
  }

//  if (updis) update_display(1);
}
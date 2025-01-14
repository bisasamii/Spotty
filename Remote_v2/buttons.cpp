#include "globals.h"
#include "buttons.h"
#include "oled.h"

//Functions
void init_buttons(){
  pinMode(BTN1_PIN, INPUT_PULLUP); //Fire Buttons
  pinMode(BTN2_PIN, INPUT_PULLUP);
  pinMode(BTN3_PIN, INPUT_PULLUP);
  pinMode(BTN4_PIN, INPUT_PULLUP);

  pinMode(SEL1_PIN, INPUT_PULLUP); //Select Buttons
  pinMode(SEL2_PIN, INPUT_PULLUP);
}

void check_buttons() {
  int b1c = digitalRead(BTN1_PIN);
  if (b1c != b1p) {
    b1 = b1c;
    b1p = b1c;
    tm_data[0] = b1;

    if (b1) {
      led_strip.setPixelColor(0, led_strip.Color(125, 125, 0));
    } else {
      led_strip.setPixelColor(0, led_strip.Color(0, 0, 0));
    }
    led_strip.show();
  }

  int b2c = digitalRead(BTN2_PIN);
  if (b2c != b2p) {
    b2 = b2c;
    b2p = b2c;
    tm_data[1] = b2;

    if (b2) {
      led_strip.setPixelColor(4, led_strip.Color(125, 125, 0));
    } else {
      led_strip.setPixelColor(4, led_strip.Color(0, 0, 0));
    }
    led_strip.show();
  }

  int b3c = digitalRead(BTN3_PIN);
  if (b3c != b3p) {
    b3 = b3c;
    b3p = b3c;
    tm_data[2] = b3;

    if (b3) {
      led_strip.setPixelColor(1, led_strip.Color(125, 125, 0));
    } else {
      led_strip.setPixelColor(1, led_strip.Color(0, 0, 0));
    }
    led_strip.show();
  }

  int b4c = digitalRead(BTN4_PIN);
  if (b4c != b4p) {
    b4 = b4c;
    b4p = b4c;
    tm_data[3] = b4;

    if (b4) {
      led_strip.setPixelColor(3, led_strip.Color(125, 125, 0));
    } else {
      led_strip.setPixelColor(3, led_strip.Color(0, 0, 0));
    }
    led_strip.show();
  }


  int s1c = digitalRead(SEL1_PIN);
  if (s1c != s1p) {
    s1 = s1c;
    s1p = s1c;
    tm_data[4] = s1;

      if (s1 == LOW) {
      // Cycle through modes: 1 -> 2 -> 3 -> 4 -> 1 -> ...
      mode_index++;
      if (mode_index > 4) {
        mode_index = 1;
      }

      cur_rgb_val1[0] = 0; cur_rgb_val1[1] = 255; cur_rgb_val1[2] = 0;
      cur_rgb_val2[0] = 0; cur_rgb_val2[1] = 0; cur_rgb_val2[2] = 0;
      update_display(3);
    }
    
  }

  int s2c = digitalRead(SEL2_PIN);
  if (s2c != s2p) {
    s2 = s2c;
    s2p = s2c;
    tm_data[5] = s2;

      if (s2 == LOW) {
      selected_mode = mode_index;
      display2.clearDisplay();
      display2.setCursor(0, 0);
      display2.setTextSize(2);
      switch (mode_index) {
        case 1:
          display2.print("Walk");
          break;
        case 2:
          display2.print("Freestyle");
          break;
        case 3:
          display2.print("Trot");
          break;
        case 4:
        display2.print("Options");
        break;
      }
      display2.setCursor(0, 20);
      display2.setTextSize(1);
      display2.print("SELECTED!");
      display2.display();
      cur_rgb_val1[0] = 255; cur_rgb_val1[1] = 0; cur_rgb_val1[2] = 0;
      cur_rgb_val2[0] = 0; cur_rgb_val2[1] = 0; cur_rgb_val2[2] = 0;
      }
    
  }
}
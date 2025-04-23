#include "globals.h"
#include "oled.h"

//Functions
void init_oled(){
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);   // initialize with the I2C addr
  display2.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR2);  
}

void startup_oled(){

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("Initializing...");
  display.display();
    
  display2.clearDisplay();
  display2.setTextSize(1);
  display2.setTextColor(WHITE);
  display2.setCursor(10,0);
  display2.println("Initializing...");
  display2.display();
  
  delay(2000);
  display.clearDisplay();
  display2.clearDisplay();
  display2.setTextSize(2);
  
  //animate bitmap
  display.drawBitmap(-10, 5,  smbmp, 128, 64, WHITE);
  display.display();
  display.startscrollright(0x00, 0x07);

  //strobe version banner
  for (int i=25;i>0;i--) {
    int d = (i*4);
    if (i > 22) d = (i*10);
    if (i == 25) d = (i*100);
    display2.setCursor(22, 2);
    display2.setTextSize(2);
    display2.println("SPOTTY");
    display2.setCursor(16, 18);
    display2.print("REMOTE");
    display2.setCursor(86, 22);
    display2.setTextSize(1);
    display2.print(" v");
    display2.println(VERSION);
    display2.display();
    delay(d);
    display2.clearDisplay();
    display2.display();
    if (i == 25) d = (i*10);
    delay(d);
  }
  display2.clearDisplay();
  display2.display();
  delay(2000);

  display2.setCursor(30,12);
  display2.setTextSize(3);
  display2.print("READY!");
  display2.display();

  display.stopscroll();
  display.clearDisplay();
  display.display();
}

void update_display(int dis) {
  if (dis == 1) { //
    display.clearDisplay();

    display.setCursor(0, 16);
    display.setTextSize(1);
    display.print("P1");
    display.setCursor(15, 16);
    display.setTextSize(2);
    display.print(per1);
    display.print("%");
    display.setCursor(67, 16);
    display.setTextSize(1);
    display.print("P2");
    display.setCursor(79, 16);
    display.setTextSize(2);
    display.print(per2);
    display.print("%");

    display.setCursor(0, 40);
    display.setTextSize(1);
    display.print("P3");
    display.setCursor(15, 40);
    display.setTextSize(2);
    display.print(per3);
    display.print("%");
    display.setCursor(67, 40);
    display.setTextSize(1);
    display.print("P4");
    display.setCursor(79, 40);
    display.setTextSize(2);
    display.print(per4);
    display.print("%");

    display.display();
  } else if (dis == 2 ) {
    display.clearDisplay();

    display.setCursor(0, 2);
    display.setTextSize(1);
    display.print("SP");
    display.setCursor(15, 2);
    display.setTextSize(2);
    display.print(rc_data[1]);
    display.setCursor(73, 2);
    display.setTextSize(1);
    display.print("MS");
    display.setCursor(87, 2);
    display.setTextSize(2);
    display.print(rc_data[3]);

    display.setCursor(0, 26);
    display.setTextSize(1);
    display.print("WF");
    display.setCursor(12, 26);
    display.setTextSize(2);
    display.print(float(float(rc_data[4]) / 100));
    display.setCursor(67, 26);
    display.setTextSize(1);
    display.print("WR");
    display.setCursor(80, 26);
    display.setTextSize(2);
    display.print(float(float(rc_data[5]) / 100));

    display.setCursor(0, 50);
    display.setTextSize(1);
    display.print("VO");
    display.setCursor(15, 50);
    display.setTextSize(2);
    display.print(rc_data[2]);

    display.setCursor(50, 50);
    display.setTextSize(1);
    display.print("MO");
    display.setCursor(65, 50);
    display.setTextSize(2);
    display.print(rc_data[6]);

    display.setCursor(90, 50);
    display.setTextSize(1);
    display.print("SM");
    display.setCursor(105, 50);
    display.setTextSize(2);
    display.print(rc_data[7]);

    display.display();

    lastDisplayUpdate = millis();
  } else if (dis == 3) {
      // Show currently selected_mode: 1=Walk, 2=Freestyle, 3=Trot, 4=Options
    display2.clearDisplay();

    display2.setCursor(55, 0);
    display2.setTextSize(1);
    display2.print("MODE ");
    display2.print(mode_index);   // e.g. prints "MODE 1" for Walk, etc.

    display2.setTextSize(2);
    switch (mode_index) {
      case 1:
        display2.setCursor(44, 9);
        display2.print("Walk");
        break;
      case 2:
        display2.setCursor(19, 9);
        display2.print("Freestyle");
        break;
      case 3:
        display2.setCursor(51, 9);
        display2.print("Trot");
        break;
      case 4:
        display2.setCursor(35, 9);
        display2.print("Options");
        break;
    }

    // You can add a small "hint" to press SEL2:
    display2.setCursor(40, 25);
    display2.setTextSize(1);
    display2.print("Press Sel2");

    display2.display();
    lastDisplayUpdate = millis();
  } else if (dis == 4) {
    display.clearDisplay();
    display.setCursor(12, 4);
    display.setTextSize(3);
    display.print("Zzz...");
    display.display();

    display2.clearDisplay();

    lastDisplayUpdate = millis();
  }
}

void oled_refresh() {
  display.display();
  lastOLEDUpdate = millis();
}
void oled2_refresh() {
  display2.display();
  lastOLED2Update = millis();
}
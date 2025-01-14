#include "globals.h"
#include "led.h"

//Functions
void init_led(){
  led_strip.begin();
  led_strip.setBrightness(rgb_bright);
  led_wipe_strip();

  Serial.println(F("LED initialized."));
}

void startup_led(){
  led_pat(1);
}




//Nested Functions
void led_wipe_strip() { // clear all LEDs
  for (int i = 0; i < led_strip.numPixels(); i++) {
    led_strip.setPixelColor(i, led_strip.Color(0, 0, 0));
  }
  led_strip.show();
}

void led_pat(int pat) {
  switch (pat) {
    case 0:
      led_colorWipe(led_strip.Color(0, 0, 0)); // off
      break;
    case 1:
      rainbow(400);
      break;
      }
}

void led_colorWipe(uint32_t c) {
  static int i = 0;
  static int i2 = 1;
  led_wipe_strip();
  led_strip.setPixelColor(i, c);
  led_strip.setPixelColor(i2, c);
  led_strip.show();
  i++;
  i2++;
  if (i >= led_strip.numPixels()) {
    i = 0;
    led_wipe_strip(); // blank out led_strip
  }
  if (i2 >= led_strip.numPixels()) {
    i2 = 0;
    led_wipe_strip(); // blank out led_strip
  }
  lastRGBUpdate = millis();
}

void rainbow(int wait) {
  for (long firstPixelHue = 0; firstPixelHue < 5 * 65536; firstPixelHue += 256) {
    for (int i = 0; i < led_strip.numPixels(); i++) {
      int pixelHue = firstPixelHue + (i * 65536L / led_strip.numPixels());
      led_strip.setPixelColor(i, led_strip.gamma32(led_strip.ColorHSV(pixelHue)));
    }
    led_strip.show(); // Update strip with new contents
    delayMicroseconds(wait);
  }
  lastRGBUpdate = millis();
}
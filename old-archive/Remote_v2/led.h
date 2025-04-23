#ifndef LED_H
#define LED_H


//Functions
void init_led(); //Initializes LEDs for Startup
void startup_led();

void led_wipe_strip(); //Clears all LEDs
void led_pat(int pat); //Calls one of the defined led patterns with different colors and rythms
void led_colorWipe(uint32_t c); //Wipes anmation for led to a different color when called in a loop (input e.g: colorWipeWave(led_strip.Color(0, 32, 0)); -> green wipe wave )
void rainbow(int wait);

#endif
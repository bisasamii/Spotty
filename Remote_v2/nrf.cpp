#include "globals.h"
#include "NRF.h"

//Functions
void init_nrf(){
  radio.setPALevel(RF24_PA_LOW);
  radio.setPayloadSize(sizeof(tm_data));
  radio.setChannel(124);
  radio.openWritingPipe(address);
  radio.enableAckPayload();
  radio.setRetries(5,5); // delay, count

  Serial.println(F("Radio ready to transmit."));
}

bool send_remote_data(){

  bool resp = false;

  if (tm_data) {
    resp = radio.write(&tm_data, sizeof(tm_data));
    if (resp) {
      if (radio.isAckPayloadAvailable()) {
        radio.read(&rc_data, sizeof(rc_data));
      }
      else{
      //handle failed transmission
        Serial.print(F("Transmission failed! "));
        Serial.println(" ");
      }
    }
    lastNRFUpdate = millis();
  }
  return resp;
}
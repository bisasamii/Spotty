#include <Wire.h>
#include "globals.h"
#include "startup_functions.h"
#include "loop_functions.h"



void setup() {
  //Start serial communication and wait for connection
  Serial.begin(9600);
  while (!Serial) {
    ;
  }
  // Load neutral positions from EEPROM
  loadNeutralPositions_EEPROM();

  //Write saved neutral positions to motors
  setMotorsToNeutralPositions();

  //Ask User to enter Calibration-Setup
  bool calibrationCheck = askUserForCalibration();
  if (calibrationCheck) {
    startCalibration();
  }

  //set robot to standby Position
  takeStandbyPosition();
  Serial.println("Robot is now in standby position.");


    // NRF24 Initialization
    if (!radio.begin()) {
        Serial.println("NRF24 not responding. Check connections.");
        while (1);
    }
    radio.setPALevel(RF24_PA_LOW);        // Set power level
    radio.openReadingPipe(0, address); // Open the correct pipe
    radio.enableAckPayload();
    radio.setPayloadSize(14);
    radio.setAutoAck(true); // Usually default
    radio.setChannel(124);               // Use the same channel as the remote
    radio.startListening();              // Start listening for data
    Serial.println("NRF24 receiver initialized and listening...");
}



void loop() {
  
  listen_serial_commands();

  // Check if data is available
    if (radio.available()) {
        // Read the received data
        radio.read(&receivedData, sizeof(receivedData));

        // Debugging: Print the received data
        Serial.print("Received Data: ");
        for (size_t i = 0; i < sizeof(receivedData); i++) {
            Serial.print(receivedData[i]);
            Serial.print(" ");
        }
        Serial.println();

        // Check if Button 1 (BTN1) is pressed
        if (receivedData[0] == 1) { // Ensure this matches the remote's transmission
            Serial.println("Button 1 pressed! Executing action...");
            setMotorsToNeutralPositions(); // Perform the desired action
        }
    }
}











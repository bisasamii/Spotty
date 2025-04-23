#include "functions.h"

void loadNeutralPositions_EEPROM() {
  for (int i = 0; i < ANZAHL_MOT; i++) {
    int storedPosition;
    EEPROM.get(i * sizeof(int), storedPosition); // Read position from EEPROM

    if (storedPosition >= USMIN && storedPosition <= USMAX) {
      neutralPositions[i] = storedPosition;
    } else {
      neutralPositions[i] = neutral_pos; // Use default if no valid stored value
    }
  }
}

void saveToEEPROM(int motorIndex, int position) {
  EEPROM.put(motorIndex * sizeof(int), position); // Store position in EEPROM
}

void calculateCorrectionValues(){
  for(int i = 0; i<ANZAHL_MOT; i++){
    correctionValues[i] = neutral_pos - neutralPositions[i];
  }
}

void init_serial(){

  Serial.begin(9600);
  while (!Serial) {
    ;
  }
}

void init_servos(){
  Serial.println("Initializing Servo Motors...");
  pwm.begin();
  pwm.setOscillatorFrequency(SERVO_OSC);
  pwm.setPWMFreq(SERVO_FREQ);
  delay(10);
}

void init_MP3(){
  mySoftwareSerial.begin(9600);
  delay(1000);

  Serial.println("Initializing DF-Player...");

  //Init DF-Player
  if (!myDFPlayer.begin(mySoftwareSerial)) {
    Serial.println("DFPlayer Fehler!");
    Serial.println("Prüfe Verkabelung und SD-Karte.");
    while (true);  // Error while loop
  }
  
  Serial.println("DFPlayer Mini ist online.");

  //Start-Volume (Values: 0-30)
  myDFPlayer.volume(25);

  //Play first track
  myDFPlayer.play(3);
}

void init_NRF24(){
   if (!radio.begin()) {
        Serial.println("NRF24 not responding. Check connections.");
        while (1);
    }
    radio.setPALevel(RF24_PA_LOW);        // Set power level
    radio.openReadingPipe(0, address);    // Open the correct pipe
    radio.enableAckPayload();
    radio.setPayloadSize(15);
    radio.setAutoAck(true);              // Usually default
    radio.setChannel(124);               // Use the same channel as the remote
    radio.startListening();              // Start listening for data
    Serial.println("NRF24 receiver initialized and listening...");
}

void check_Remote(){
  if (radio.available()) {
      remote_connected = true;
    }
  else{
    remote_connected = false;
  }
}

void init_MagnetSensor(){
  Wire1.begin();
  compass.init();
  Serial.println("Magnet-Sensor erfolgreich initialisiert.");


  // Kurze Wartezeit (Sensor beruhigen lassen)
  delay(500);

  // Baseline messen
  float sumMagnitude = 0.0;
  for (int i = 0; i < BASELINE_SAMPLES; i++) {
    compass.read();
    float x = compass.getX();
    float y = compass.getY();
    float z = compass.getZ();
    float mag = sqrt(x*x + y*y + z*z);

    sumMagnitude += mag;
    delay(80); // etwas warten zwischen den Messungen
  }
  baselineMagnitude = sumMagnitude / BASELINE_SAMPLES;

  Serial.println("Baseline-Messung abgeschlossen.");
  Serial.print("Gemittelte Magnitude: ");
  Serial.println(baselineMagnitude);
  Serial.println("Setup fertig!");
}

bool askUserForCalibration() {
  Serial.println("Kalibrierungssequenz starten? (Y/N)");

  // Wait for user input
  while (Serial.available() == 0) {
    ;
  }

  char input = Serial.read();  // Read serial input

  // Clear input buffer
  while (Serial.available() > 0) {
    Serial.read();
  }

  if (input == 'Y' || input == 'y') {
    Serial.println("Kalibrierungssequenz wird gestartet...");
    return true;
  } else if (input == 'N' || input == 'n') {
    Serial.println("Kalibrierungssequenz wird übersprungen.");
    return false;
  } else {
    Serial.println("Ungültige Eingabe. Bitte nur 'Y' oder 'N' verwenden.");
    return askUserForCalibration();
  }
}

void startCalibration() {
  Serial.println("Kalibrierung läuft...");

  for (int i = 0; i < ANZAHL_MOT; i++) {
    int position = neutralPositions[i]; // Start with loaded position
    bool positionConfirmed = false;

    // Set servo to neutral and let user confirm
    pwm.writeMicroseconds(i, position);
    Serial.println("Motor " + String(i) + " auf Neutralposition gesetzt.");

    while (!positionConfirmed) {
      Serial.println("Position für Motor " + String(i) + " bestätigen? (Y/N)");
      
      // Wait for user input and clear buffer
      while (Serial.available() == 0) {
        ;
      }
      
      char input = Serial.read();
      while (Serial.available() > 0) {
        Serial.read();
      }

      if (input == 'Y' || input == 'y') {
        Serial.println("Position bestätigt.");
        positionConfirmed = true;
      } else if (input == 'N' || input == 'n') {
        Serial.println("Position anpassen mit '+' und '-'.");

        bool adjusting = true;
        while (adjusting) {
          // Wait for adjustment input and clear buffer
          while (Serial.available() == 0) {
            ;
          }
          char adjustInput = Serial.read();
          while (Serial.available() > 0) {
            Serial.read();
          }

          if (adjustInput == '+') {
            position += NEUTRAL_INCREMENT;
            if (position > USMAX) position = USMAX;
            pwm.writeMicroseconds(i, position);
            Serial.println("Position erhöht: " + String(position));
          } else if (adjustInput == '-') {
            position -= NEUTRAL_INCREMENT;
            if (position < USMIN) position = USMIN;
            pwm.writeMicroseconds(i, position);
            Serial.println("Position verringert: " + String(position));
          } else if (adjustInput == 'Y' || adjustInput == 'y') {
            Serial.println("Neue Position für Motor " + String(i) + " bestätigt.");
            saveToEEPROM(i, position); // Save the confirmed position to EEPROM
            positionConfirmed = true;
            adjusting = false;
          }
        }
      }
    }
  }
  Serial.println("Kalibrierung abgeschlossen.");
}

void get_remote_data(){
   // Read the received data
        radio.read(&receivedData, sizeof(receivedData));

    //Write Robot Data to Ack-Array
    ackData[6] = receivedData[14];  //Writes Mode of the Robot back to the Remote

    //Send back Robot Data to Remote as AckPayload
    radio.writeAckPayload(0, &ackData, sizeof(ackData));

        // Debugging: Print the received data
        Serial.print("Received Data: ");
        for (size_t i = 0; i < sizeof(receivedData); i++) {
            Serial.print(receivedData[i]);
            Serial.print(" ");
        }
        Serial.println();
}

void get_joystick_commands(){
  int jlx = receivedData[10];
  int jly = receivedData[11];
  Serial.print("Input X: ");
  Serial.print(jlx);
  Serial.print(", Input Y: ");
  Serial.println(jly);
  // Deadzone threshold to prevent false readings when the joystick is near the center
  const int threshold = 10;

  if (jlx >= 0 && jlx < threshold) {
    currentPosition = "forward";
  }
  // "backwards" when X is near 255 (i.e. greater than center + threshold)
  else if (jlx <= 255 && jlx > (255-threshold)) {
    currentPosition = "backwards";
  }
  
  // Check Y-axis for right/left commands
  // "right" when Y is near 0 (i.e. less than center - threshold)
  else if (jly >= 0 && jly < threshold) {
    currentPosition = "right";
  }
  // "left" when Y is near 255 (i.e. greater than center + threshold)
  else if (jly <= 255 && jly > (255-threshold)) {
    currentPosition = "left";  }

  else{
    currentPosition = "middle";
  }
  

}

void check_magnetic_field(){
  // read sensor values
  compass.read();
  float x = compass.getX();
  float y = compass.getY();
  float z = compass.getZ();

  // calculate total magnetic magnitude
  float currentMagnitude = sqrt(x*x + y*y + z*z);

  // calculate diff to basline
  float diff = fabs(currentMagnitude - baselineMagnitude);

  //If diff is greater than threshhol, there is a magnetic field
  if (diff > MAGNET_DIFF_THRESHOLD) {
    magneticDetection = true;
  } else {
    magneticDetection = false;
  }

  //if there is a magnetic field and the cooldownTime has passed, bark!
  if (magneticDetection == true && millis() - lastPlayTime > cooldownTime){
    myDFPlayer.play(4);
    lastPlayTime = millis();
  }
}
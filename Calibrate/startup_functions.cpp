#include "startup_functions.h"

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
    radio.setPayloadSize(14);
    radio.setAutoAck(true);              // Usually default
    radio.setChannel(124);               // Use the same channel as the remote
    radio.startListening();              // Start listening for data
    Serial.println("NRF24 receiver initialized and listening...");
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

void setMotorsToNeutralPositions() {

  for (int i = 0; i < ANZAHL_MOT; i++) {
    int position = neutralPositions[i];
    pwm.writeMicroseconds(i, position); // Set each motor to its neutral position
    Serial.println("Motor " + String(i) + " auf gespeicherte Neutralposition gesetzt: " + String(position));
  }

  //set currentPosition variable
  currentPosition = "neutral";

}

void setMotorsToPosition(int servoPort, int position) {

  pwm.writeMicroseconds(servoPort, position); // Set each motor to position
  Serial.println("Motor " + String(servoPort) + " auf gespeicherte Neutralposition gesetzt: " + String(position));

  //set currentPosition variable
  currentPosition = "neutral";

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

void takeStandbyPosition(){
  Serial.println("Going into Standby-Position...");
    
    // Define the adjustments for each motor type
    int tibiaAdjustment = 200;
    int femurAdjustment = -100;
    int shoulderAdjustment = 0;

    // Define the leg movement order: Front Right, Back Left, Front Left, Back Right
    int legOrder[] = {0, 1, 2, 3};

    // Loop through each leg and apply the adjustments
    for (int i = 0; i < 4; i++) {
        int leg = legOrder[i];
        // Determine the direction of adjustment (mirrored for left legs)
        int direction = (leg % 2 == 0) ? 1 : -1; // Right legs: positive, Left legs: negative

        // Apply adjustments to Tibia, Femur, and Shoulder
        int tibiaPort = legPorts[leg][0];
        int femurPort = legPorts[leg][1];
        int shoulderPort = legPorts[leg][2];

        int tibiaPosition = neutralPositions[tibiaPort] + direction * tibiaAdjustment;
        int femurPosition = neutralPositions[femurPort] + direction * femurAdjustment;
        int shoulderPosition = neutralPositions[shoulderPort] + direction * shoulderAdjustment;

        // Ensure positions stay within bounds
        tibiaPosition = constrain(tibiaPosition, USMIN, USMAX);
        femurPosition = constrain(femurPosition, USMIN, USMAX);
        shoulderPosition = constrain(shoulderPosition, USMIN, USMAX);

        // Write the positions to the motors
        pwm.writeMicroseconds(tibiaPort, tibiaPosition);
        delay(80);
        pwm.writeMicroseconds(femurPort, femurPosition);
        pwm.writeMicroseconds(shoulderPort, shoulderPosition);

        //change currentPosition variable
        currentPosition = "standby";

        // Output feedback to Serial monitor
        Serial.println("Leg " + String(leg) + " positions set:");
        Serial.println("  Tibia (Port " + String(tibiaPort) + "): " + String(tibiaPosition));
        Serial.println("  Femur (Port " + String(femurPort) + "): " + String(femurPosition));
        Serial.println("  Shoulder (Port " + String(shoulderPort) + "): " + String(shoulderPosition));
    }
}
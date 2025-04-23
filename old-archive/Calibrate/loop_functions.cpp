// loop_functions.cpp
#include "loop_functions.h"

void listen_serial_commands() {
    // Check if there is incoming data on the Serial
    if (Serial.available() > 0) {
        // Read the input from Serial as a string
        String input = Serial.readStringUntil('\n');
        input.trim(); // Remove any extra spaces or newlines

        // Check if the command is "goto_neutral"
        if (input.equalsIgnoreCase("goto_neutral")) {
            Serial.println("Setting all motors to neutral positions...");
            setMotorsToNeutralPositions();
            Serial.println("All motors set to neutral positions.");

        // Check if the command is "goto_standby"
        } else if (input.equalsIgnoreCase("goto_standby")) {
          if (currentPosition = "down"){
            Serial.println("Going from down to standby...");
            sit();
            delay(1000);
            takeStandbyPosition();
            Serial.println("All motors set to standby positions.");
          }
            else {
            Serial.println("Setting all motors to standby positions...");
            takeStandbyPosition();
            Serial.println("All motors set to standby positions.");
            }
        //Check if the command is "walk_forward"
        } else if (input.equalsIgnoreCase("walk_forward")) {
            Serial.println("Walking forward...");
            walk_forward();

        } else if (input.equalsIgnoreCase("trot_forward")) {
            Serial.println("Trotting forward...");
            trot_forward2();

        } else if (input.equalsIgnoreCase("pushups")) {
            Serial.println("Doing pushups...");
            pushups();

        } else if (input.equalsIgnoreCase("sit")) {
            Serial.println("Sitting down...");
            sit();

        } else if (input.equalsIgnoreCase("down")) {
            Serial.println("Going down...");
            down();

        } else if (input.equalsIgnoreCase("paw")) {
            Serial.println("Giving paw...");
            paw();

        } else if (input.equalsIgnoreCase("showoff")) {
            Serial.println("Showing off what I can do...");
            showoff();

        } else {
            // Unknown command feedback
            Serial.println("Unknown command: " + input);
            Serial.println("Type in: help to see all valid commands.");
        }
    }
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

void walk_forward() {
    // Adjustments for each motor type
    int liftHeight = 400;          // How much Tibia lifts
    int smallStepForward = 100;    // Small forward step for Femur
    //int shoulderAdjustment = 0;   // No movement for Shoulder motors

    // Define the leg movement order: Front Right, Back Left, Front Left, Back Right
    int legOrder[] = {0, 3, 1, 2};

    // Infinite loop to keep walking until a new command is issued
    while (true) {
        for (int i = 0; i < 4; i++) {
           int leg = legOrder[i];

            // Determine which ports to use based on leg index
            int tibiaPort = legPorts[leg][0];
            int femurPort = legPorts[leg][1];
            //int shoulderPort = legPorts[leg][2];

            // Get the leg direction for right and left legs
            int direction = (leg % 2 == 0) ? 1 : -1; // Right legs: positive, Left legs: negative

            // Step 1: Lift leg (Tibia)
            int tibiaLiftedPosition = neutralPositions[tibiaPort] + direction * liftHeight;
            pwm.writeMicroseconds(tibiaPort, constrain(tibiaLiftedPosition, USMIN, USMAX));
            delay(80);  // Short delay for smooth lifting

            // Step 2: Move leg forward (Femur)
            //int tibiaForwardPosition = neutralPositions[tibiaPort] + direction * smallStepForward;
            int femurForwardPosition = neutralPositions[femurPort] + direction * smallStepForward;
            pwm.writeMicroseconds(femurPort, constrain(femurForwardPosition, USMIN, USMAX));
            delay(80);  // Smooth transition delay

            // Step 3: Lower leg back down (Tibia+Femur)
            if(leg == 3 || leg == 2){
            pwm.writeMicroseconds(tibiaPort, neutralPositions[tibiaPort]+ direction * 200);
            delay(80);
            pwm.writeMicroseconds(femurPort, neutralPositions[femurPort]+ direction * (-150));
            }
            
            else{ pwm.writeMicroseconds(tibiaPort, neutralPositions[tibiaPort]+ direction * 150);
            delay(80);
            pwm.writeMicroseconds(femurPort, neutralPositions[femurPort]+ direction * (-50));
            }
            // Add a slight pause before moving the next leg for smooth gait
            delay(10);
        }
        
        // Check if a stop command has been issued
        if (Serial.available() > 0) {
            String stopCommand = Serial.readStringUntil('\n');
            stopCommand.trim();
            if (stopCommand.equalsIgnoreCase("stop")) {
                Serial.println("Walking stopped.");
                break;
            }
        }
    }
}

void trot_forward2() {
    // Define motor positions relative to neutral position
    int tibiaStartingPos = 50;       // Slightly forward
    int tibiaInitialLift = 100;      // Small lift for smooth motion

    int femurStartingPos = -50;      // Slightly backward
    int FemurInitialLift = -20;      // Small initial lift
    int FemurBackPos = -120;         // Extends contact with ground

    // Define diagonal leg pairs: {Front Right & Back Left}, {Front Left & Back Right}
    int legPairs[2][2] = {{0, 3}, {1, 2}};

    // Infinite loop to keep trotting until a stop command is issued
    while (true) {
        for (int i = 0; i < 2; i++) {
            int leg1 = legPairs[i][0];  // First leg in the pair
            int leg2 = legPairs[i][1];  // Second leg in the pair

            // Determine which ports to use based on leg index
            int tibiaPort1 = legPorts[leg1][0];
            int femurPort1 = legPorts[leg1][1];

            int tibiaPort2 = legPorts[leg2][0];
            int femurPort2 = legPorts[leg2][1];

            // Get the leg direction for right and left legs
            int direction1 = (leg1 % 2 == 0) ? 1 : -1; // Right legs: positive, Left legs: negative
            int direction2 = (leg2 % 2 == 0) ? 1 : -1; // Right legs: positive, Left legs: negative

            // **Step 1: Set initial positions (Starting Pos)**
            pwm.writeMicroseconds(femurPort1, constrain(neutralPositions[femurPort1] + direction1 * femurStartingPos, USMIN, USMAX));
            pwm.writeMicroseconds(femurPort2, constrain(neutralPositions[femurPort2] + direction2 * femurStartingPos, USMIN, USMAX));
            pwm.writeMicroseconds(tibiaPort1, constrain(neutralPositions[tibiaPort1] + direction1 * tibiaStartingPos, USMIN, USMAX));
            pwm.writeMicroseconds(tibiaPort2, constrain(neutralPositions[tibiaPort2] + direction2 * tibiaStartingPos, USMIN, USMAX));
            delay(100);  // Short delay for stabilization

            // **Step 2: Initial lift (Femur + Tibia)**
            pwm.writeMicroseconds(femurPort1, constrain(neutralPositions[femurPort1] + direction1 * FemurInitialLift, USMIN, USMAX));
            pwm.writeMicroseconds(femurPort2, constrain(neutralPositions[femurPort2] + direction2 * FemurInitialLift, USMIN, USMAX));
            pwm.writeMicroseconds(tibiaPort1, constrain(neutralPositions[tibiaPort1] + direction1 * tibiaInitialLift, USMIN, USMAX));
            pwm.writeMicroseconds(tibiaPort2, constrain(neutralPositions[tibiaPort2] + direction2 * tibiaInitialLift, USMIN, USMAX));
            delay(120);  

            // **Step 3: Move femur forward**
            pwm.writeMicroseconds(femurPort1, constrain(neutralPositions[femurPort1], USMIN, USMAX));
            pwm.writeMicroseconds(femurPort2, constrain(neutralPositions[femurPort2], USMIN, USMAX));
            delay(120);  

            // **Step 4: Lower Tibia (Contact ground)**
            pwm.writeMicroseconds(tibiaPort1, neutralPositions[tibiaPort1]);
            pwm.writeMicroseconds(tibiaPort2, neutralPositions[tibiaPort2]);
            delay(120);  

            // **Step 5: Extend ground contact (Femur bends back more)**
            pwm.writeMicroseconds(femurPort1, constrain(neutralPositions[femurPort1] + direction1 * FemurBackPos, USMIN, USMAX));
            pwm.writeMicroseconds(femurPort2, constrain(neutralPositions[femurPort2] + direction2 * FemurBackPos, USMIN, USMAX));
            delay(150);  

            // Small pause before switching to next pair
            delay(50);
        }

        // Check if a stop command has been issued
        if (Serial.available() > 0) {
            String stopCommand = Serial.readStringUntil('\n');
            stopCommand.trim();
            if (stopCommand.equalsIgnoreCase("stop")) {
                Serial.println("Trotting stopped.");
                break;
            }
        }
    }
}

void walk_test(){
  
}

void pushups(int durationInSeconds) {
    // Small adjustments for pushup motion
    int tibiaPushupHeight = -550;   // Small lift for Tibia (up and down)
    int femurPushupAdjustment = 300; // Small adjustment for Femur (up and down)

    Serial.println("Starting pushups...");

    unsigned long startTime = millis(); // Record the start time
    bool infiniteLoop = (durationInSeconds == -1); // Check if duration is specified

    // Define the leg movement order: Front Right, Back Left, Front Left, Back Right
    int legOrder[] = {3, 2, 1, 0};

    // Infinite loop to keep doing pushups until a stop command is issued or time expires
    while (infiniteLoop || millis() - startTime < (unsigned long)durationInSeconds * 1000) {
        // Step 1: Lower the robot (move Tibia and Femur)
        for (int i = 0; i < 4; i++) {
            int leg = legOrder[i];
            int tibiaPort = legPorts[leg][0];
            int femurPort = legPorts[leg][1];
            int direction = (leg % 2 == 0) ? 1 : -1; // Right legs: positive, Left legs: negative

            int tibiaDownPosition = neutralPositions[tibiaPort] - direction * tibiaPushupHeight;
            int femurDownPosition = neutralPositions[femurPort] - direction * femurPushupAdjustment;

            pwm.writeMicroseconds(tibiaPort, constrain(tibiaDownPosition, USMIN, USMAX));
            pwm.writeMicroseconds(femurPort, constrain(femurDownPosition, USMIN, USMAX));

            if(leg == 2 || leg == 0){ //Alway move front and back legs at same time, in between short delay
              delay(80);
            }
        }

        delay(700);  // Pause to hold the lowered position

        // Step 2: Raise the robot back up to neutral
        for (int i = 0; i < 4; i++) {
            int leg = legOrder[i];
            int tibiaPort = legPorts[leg][0];
            int femurPort = legPorts[leg][1];
            int direction = (leg % 2 == 0) ? 1 : -1; // Right legs: positive, Left legs: negative

            pwm.writeMicroseconds(tibiaPort, neutralPositions[tibiaPort] + direction * 200);
            pwm.writeMicroseconds(femurPort, neutralPositions[femurPort] + direction * (-100));

            if(leg == 2 || leg == 0){ //Alway move front and back legs at same time, in between short delay
              delay(80);
            }
        }

        delay(700);  // Pause to hold the raised position

        // Check if a stop command has been issued via Serial
        if (Serial.available() > 0) {
            String stopCommand = Serial.readStringUntil('\n');
            stopCommand.trim();
            if (stopCommand.equalsIgnoreCase("stop")) {
                Serial.println("Pushups stopped.");
                break;
            }
        }

        //Check if stop command has been issued via Remote Stop Button
        if (radio.available()) {
        get_remote_data();  // This updates `receivedData[]`
        }
        if(receivedData[5] == 1){
          Serial.println("Pushups stopped.");
          break;
        }
    }

    Serial.println("Pushup duration complete.");
}

void sit() {
  //Adjustments for Motor-Types
  int tibiaBack = 650;
  int femurBack = -300;
  int tibiaFront = 10;
  int femurFront = 50;

  Serial.println("Starting to sit down...");

  //Write Positions to Motors
  int tibiaBackPort_R = legPorts[2][0];
  int femurBackPort_R = legPorts[2][1];
  int tibiaFrontPort_R = legPorts[0][0];
  int femurFrontPort_R = legPorts[0][1];
  int tibiaBackPort_L = legPorts[3][0];
  int femurBackPort_L = legPorts[3][1];
  int tibiaFrontPort_L = legPorts[1][0];
  int femurFrontPort_L = legPorts[1][1];

  int tibiaBack_Position_R = neutralPositions[tibiaBackPort_R] + tibiaBack;
  int femurBack_Position_R = neutralPositions[femurBackPort_R] + femurBack;
  int tibiaFront_Position_R = neutralPositions[tibiaFrontPort_R] + tibiaFront;
  int femurFront_Position_R = neutralPositions[femurFrontPort_R] + femurFront;
  int tibiaBack_Position_L = neutralPositions[tibiaBackPort_L] - tibiaBack;
  int femurBack_Position_L = neutralPositions[femurBackPort_L] - femurBack;
  int tibiaFront_Position_L = neutralPositions[tibiaFrontPort_L] - tibiaFront;
  int femurFront_Position_L = neutralPositions[femurFrontPort_L] - femurFront;

  pwm.writeMicroseconds(tibiaBackPort_R, constrain(tibiaBack_Position_R, USMIN, USMAX));
  pwm.writeMicroseconds(femurBackPort_R, constrain(femurBack_Position_R, USMIN, USMAX));
  pwm.writeMicroseconds(tibiaFrontPort_R, constrain(tibiaFront_Position_R, USMIN, USMAX));
  pwm.writeMicroseconds(femurFrontPort_R, constrain(femurFront_Position_R, USMIN, USMAX));
  pwm.writeMicroseconds(tibiaBackPort_L, constrain(tibiaBack_Position_L, USMIN, USMAX));
  pwm.writeMicroseconds(femurBackPort_L, constrain(femurBack_Position_L, USMIN, USMAX));
  pwm.writeMicroseconds(tibiaFrontPort_L, constrain(tibiaFront_Position_L, USMIN, USMAX));
  pwm.writeMicroseconds(femurFrontPort_L, constrain(femurFront_Position_L, USMIN, USMAX));

  //change currentPosition variable
  currentPosition = "sit";
}

void down() {
  //Adjustments for Motor-Types
  int tibiaBack = 650;
  int femurBack = -300;
  int tibiaFront = 650;
  int femurFront = -300;

  Serial.println("Starting to sit down...");

  //Write Positions to Motors
  int tibiaBackPort_R = legPorts[2][0];
  int femurBackPort_R = legPorts[2][1];
  int tibiaFrontPort_R = legPorts[0][0];
  int femurFrontPort_R = legPorts[0][1];
  int tibiaBackPort_L = legPorts[3][0];
  int femurBackPort_L = legPorts[3][1];
  int tibiaFrontPort_L = legPorts[1][0];
  int femurFrontPort_L = legPorts[1][1];

  int tibiaBack_Position_R = neutralPositions[tibiaBackPort_R] + tibiaBack;
  int femurBack_Position_R = neutralPositions[femurBackPort_R] + femurBack;
  int tibiaFront_Position_R = neutralPositions[tibiaFrontPort_R] + tibiaFront;
  int femurFront_Position_R = neutralPositions[femurFrontPort_R] + femurFront;
  int tibiaBack_Position_L = neutralPositions[tibiaBackPort_L] - tibiaBack;
  int femurBack_Position_L = neutralPositions[femurBackPort_L] - femurBack;
  int tibiaFront_Position_L = neutralPositions[tibiaFrontPort_L] - tibiaFront;
  int femurFront_Position_L = neutralPositions[femurFrontPort_L] - femurFront;

  pwm.writeMicroseconds(tibiaBackPort_R, constrain(tibiaBack_Position_R, USMIN, USMAX));
  pwm.writeMicroseconds(femurBackPort_R, constrain(femurBack_Position_R, USMIN, USMAX));
  pwm.writeMicroseconds(tibiaFrontPort_R, constrain(tibiaFront_Position_R, USMIN, USMAX));
  pwm.writeMicroseconds(femurFrontPort_R, constrain(femurFront_Position_R, USMIN, USMAX));
  pwm.writeMicroseconds(tibiaBackPort_L, constrain(tibiaBack_Position_L, USMIN, USMAX));
  pwm.writeMicroseconds(femurBackPort_L, constrain(femurBack_Position_L, USMIN, USMAX));
  pwm.writeMicroseconds(tibiaFrontPort_L, constrain(tibiaFront_Position_L, USMIN, USMAX));
  pwm.writeMicroseconds(femurFrontPort_L, constrain(femurFront_Position_L, USMIN, USMAX));

  //change currentPosition variable
  currentPosition = "down";

}

void paw(int durationInSeconds2) {
   // Adjustments for Motor-Types
  int tibiaFrontUp = 500;
  int tibiaFrontDown = 300;
  int femurFront = 250;

  Serial.println("Starting to sit down...");

  // Write Positions to Motors
  int tibiaFrontPort_R = legPorts[0][0];
  int femurFrontPort_R = legPorts[0][1];

  int tibiaFront_UpPosition_R = neutralPositions[tibiaFrontPort_R] + tibiaFrontUp;
  int tibiaFront_DownPosition_R = neutralPositions[tibiaFrontPort_R] + tibiaFrontDown;
  int femurFront_Position_R = neutralPositions[femurFrontPort_R] + femurFront;

  // Go into sit position
  sit();
  delay(500);
  
  // Lift Femur
  pwm.writeMicroseconds(femurFrontPort_R, constrain(femurFront_Position_R, USMIN, USMAX));

  // Record the start time
  unsigned long startTime = millis();
  bool infiniteLoop = (durationInSeconds2 == -1); // Check if duration is specified

  // Shake Tibia
  while (infiniteLoop || millis() - startTime < (unsigned long)durationInSeconds2 * 1000) {
    pwm.writeMicroseconds(tibiaFrontPort_R, constrain(tibiaFront_UpPosition_R, USMIN, USMAX));
    delay(700);
    pwm.writeMicroseconds(tibiaFrontPort_R, constrain(tibiaFront_DownPosition_R, USMIN, USMAX));
    delay(700);

    // Check if a stop command has been issued
    if (Serial.available() > 0) {
        String stopCommand = Serial.readStringUntil('\n');
        stopCommand.trim();
        if (stopCommand.equalsIgnoreCase("stop")) {
            Serial.println("Paw stopped");
            sit();
            break;
        }
    }
  }

  Serial.println("Paw duration complete.");
  sit(); // Return to sit position when finished
}

void showoff() {
  pushups(6);
  delay(1500);
  sit();
  delay(1500);
  takeStandbyPosition();
  delay(1500);
  down();
  delay(1500);
  takeStandbyPosition();
  delay(1500);
  paw(6);
  delay(1500);
  takeStandbyPosition();
  delay(1500);
  setMotorsToNeutralPositions();
  delay(1500);
  walk_forward();
   
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

  delay(200);
}
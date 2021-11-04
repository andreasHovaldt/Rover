#include <Wire.h>
#include <Zumo32U4.h>
Zumo32U4Encoders encoders;
Zumo32U4LCD LCD;
Zumo32U4Buzzer buzzer;
Zumo32U4ButtonA buttonA;
Zumo32U4Motors motors;


//General variables
unsigned long timeNow = 0;

//Interaction variables
int accCountsR = 0;
int commandSelected = 0; //0 = Forward,  1 = Backwards, 2 = Right, 3 = Left
int speedSelected = 0;
int durationSelected = 0;

//Stage variables
int actualStage = 0; //0 = Select command, 1 = Select speed,  2 = Select duration, 3 = Run robot
bool hasRun_0 = false, hasRun_1 = false, hasRun_2 = false, hasRun_3 = false;

void setup() {
  Serial.begin(9600);
  delay(1000);
}

void loop() {
  //timeNow = millis();

  if (actualStage == 0) { //Stage 0 --> Select command
    if (hasRun_0 == false) {
      Serial.println("I am in stage 0!");
      hasRun_0 = true;
    }
    stage0command(); 
  }

  if (actualStage == 1) { // Stage 1 --> Select speed
    if (hasRun_1 == false) {
      Serial.println("I am in stage 1!");
      hasRun_1 = true;
    }
    stage1speed(); 
  }

  if (actualStage == 2) { //Stage 1 --> Select duration
    if (hasRun_2 == false) {
      Serial.println("I am in stage 2!");
      hasRun_2 = true;
    }
    stage2duration();
  }

  if (actualStage == 3) { //Stage 3 --> Running the robot
    if (hasRun_3 == false) {
      Serial.println("I am in stage 3!");
      hasRun_3 = true;
    }
    LCDstage3runRobot(commandSelected, durationSelected);
    stage3runRobot(commandSelected, speedSelected, durationSelected);
    LCD.clear(); //Should be removed if millis is added to LCDstage3runRobot
  }



  
}

void stage0command() { //Select drive command
  readEncoders(); //Read the encoders
  if (accCountsR > 100) { //If is larger or lower than some threshhold then
    beep();
    commandSelected += 1; //Increment or decrement  the selected command
    accCountsR = accCountsR - 100;
    if (commandSelected > 3) commandSelected = 0;
  }
  else if (accCountsR < -100) {
    beep();
    commandSelected -= 1;
    accCountsR = accCountsR + 100;

    if (commandSelected < 0) commandSelected = 3;
  }

  Serial.println("Count: " + (String)accCountsR + " Command: " + (String)commandSelected);

  LCDstage0command(commandSelected);
  //Show it on the screen
  //Title
  //In which command we are

  if (buttonA.isPressed()) { //Is the button pressed?
    actualStage = 1;  //If yes I jump to next stage and store the command
    buttonA.waitForRelease();
  }
}

void LCDstage0command(int moveCommand) { //Display selection of commands on LCD display
  String nameCommand;
  switch (moveCommand) {
    case 0:
      nameCommand = "Forward";
      break;

    case 1:
      nameCommand = "Backward";
      break;

    case 2:
      nameCommand = "Right";
      break;

    case 3:
      nameCommand = "Left";
      break;

    default:
      nameCommand = "Error! :(";
  }
  LCD.clear();
  LCD.print("Command>");
  LCD.gotoXY(0, 1);
  LCD.print(nameCommand);
}

void stage1speed() { //Select drive speed
  readEncoders(); //Read the encoders
  if (accCountsR > 100) { //If is larger or lower than some threshhold then
    beep();
    speedSelected += 50; //Increment or decrement  the selected command
    accCountsR = accCountsR - 50;
    if (speedSelected > 400) speedSelected = 0;
  }
  else if (accCountsR < -100) {
    beep();
    speedSelected -= 50;
    accCountsR = accCountsR + 50;

    if (speedSelected < 0) speedSelected = 400;
  }

  Serial.println("Count: " + (String)accCountsR + " Speed: " + (String)speedSelected);

  LCDstage1speed(speedSelected);

  if (buttonA.isPressed()) { //Is the button pressed?
    actualStage = 2;  //If yes I jump to next stage and store the command
    buttonA.waitForRelease();
  }
}

void LCDstage1speed(int moveSpeed) { //Display selection of speeds on LCD display
  LCD.clear();
  LCD.print("Speed>");
  LCD.gotoXY(0, 1);
  LCD.print(moveSpeed);
}

void stage2duration() { //Select drive duration
  readEncoders(); //Read the encoders
  if (accCountsR > 50) { //If is larger or lower than some threshhold then
    beep();
    durationSelected += 100; //Increment or decrement  the selected command
    accCountsR = accCountsR - 50;
    if (durationSelected > 10000) durationSelected = 0;
  }
  else if (accCountsR < -50) {
    beep();
    durationSelected -= 100;
    accCountsR = accCountsR + 50;

    if (durationSelected < 0) durationSelected = 10000;
  }

  Serial.println("Count: " + (String)accCountsR + " Speed: " + (String)durationSelected);

  LCDstage2duration(durationSelected);

  if (buttonA.isPressed()) { //Is the button pressed?
    actualStage = 3;  //If yes I jump to next stage and store the command
    buttonA.waitForRelease();
  }
}

void LCDstage2duration(int moveDuration) { //Display selection of duration on LCD display
  LCD.clear();
  LCD.print("Duration>");
  LCD.gotoXY(0, 1);
  LCD.print(moveDuration);
}

void stage3runRobot(int moveCommand, int moveSpeed, int moveDuration) { //Run the robot with specified command, speed and duration
  switch (moveCommand) { //0 = Forward,  1 = Backwards, 2 = Right, 3 = Left
    case 0:
      moveForward(moveSpeed, moveDuration);
      break;

    case 1:
      moveBackwards(moveSpeed, moveDuration);
      break;

    case 2:
      moveTurn(moveCommand, moveSpeed, moveDuration);
      break;

    case 3:
      moveTurn(moveCommand, moveSpeed, moveDuration);
      break;

    default:
      moveStop();
      Serial.println("stage 3 Error");
      //Add LCD error
  }
  moveStop();
}

void LCDstage3runRobot(int moveCommand, int moveDuration) { //Display what the robot is doing on LCD display
  String nameCommand;
  switch (moveCommand) { //0 = Forward,  1 = Backwards, 2 = Right, 3 = Left
    case 0:
      nameCommand = "Moving Forward!";
      break;

    case 1:
      nameCommand = "Moving Backwards!";
      break;

    case 2:
      nameCommand = "Turning Right!";
      break;

    case 3:
      nameCommand = "Turning Left!";
      break;

    default:
      nameCommand = "Error! :(";
      Serial.println("Error (LCDstage3)");
  }
  LCD.clear();
  LCD.gotoXY(0, 1);
  LCD.print(nameCommand);
  //millis --> delay(moveDuration);
  //LCD.clear();
}

void moveForward(int moveSpeed, int moveDuration) { //Function for moving the robot forwards
  motors.setSpeeds(moveSpeed, moveSpeed);
  delay(moveDuration);
}

void moveBackwards(int moveSpeed, int moveDuration) { //Function for moving the robot backwards
  motors.setSpeeds(-moveSpeed, -moveSpeed);
  delay(moveDuration);
}

void moveTurn(int moveCommand, int moveSpeed, int moveDuration) { //Function for turning the robot
  switch (moveCommand) {
    case 3:
      motors.setSpeeds(-moveSpeed, moveSpeed); //Turn left
      break;

    case 2:
      motors.setSpeeds(moveSpeed, -moveSpeed); //Turn right
      break;

    default:
      motors.setSpeeds(0, 0);
      break;

  }
  delay(moveDuration);
}

void moveStop() { //Function for stopping the motors of the robot
  motors.setSpeeds(0, 0);
}

void beep() { //Play small beep noise from buzzer
  buzzer.playNote(NOTE_A(4), 20, 15);
  delay(30);
}

void readEncoders() { //Read encoders
  accCountsR = accCountsR + encoders.getCountsAndResetRight();
}

//Included libraries
#include <Wire.h>
#include <Zumo32U4.h>
Zumo32U4Encoders encoders;
Zumo32U4LCD LCD;
Zumo32U4Buzzer buzzer;
Zumo32U4ButtonA buttonA;
Zumo32U4Motors motors;

//Interaction variables
int accCountsR = 0;
int commandSelected = 0; //0 = Forward,  1 = Backwards, 2 = Right, 3 = Left
int speedSelected = 0;
int durationSelected = 0;

//Stage variables
int actualStage = 0; //0 = Select command, 1 = Select speed,  2 = Select duration, 3 = Run robot
bool hasRun_0 = false, hasRun_1 = false, hasRun_2 = false, hasRun_3 = false;
int moveCountdown = 5;
bool countdownDone = false;

void setup() {
  Serial.begin(9600);
  delay(1000);
}

void loop() {
  if (actualStage == 0) { //Stage 0 --> Select command
    if (hasRun_0 == false) {
      Serial.println("I am in stage 0!");
      hasRun_0 = true;
    }
    stage0();
  }

  if (actualStage == 1) { // Stage 1 --> Select speed
    if (hasRun_1 == false) {
      Serial.println("I am in stage 1!");
      hasRun_1 = true;
    }
    stage1();
  }

  if (actualStage == 2) { //Stage 2 --> Select duration
    if (hasRun_2 == false) {
      Serial.println("I am in stage 2!");
      hasRun_2 = true;
    }
    stage2();
  }

  if (actualStage == 3) { //Stage 3 --> Running the robot
    if (hasRun_3 == false) {
      Serial.println("I am in stage 3!");
      hasRun_3 = true;
    }
    countdown();
    if (countdownDone == true); {
      stage3LCD(commandSelected, durationSelected);
      stage3(commandSelected, speedSelected, durationSelected);
    }
  }
}

void stage0() {                                                   //Select drive command
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

  stage0LCD(commandSelected);
  //Show it on the screen
  //Title
  //In which command we are

  if (buttonA.isPressed()) { //Is the button pressed?
    actualStage = 1;  //If yes I jump to next stage and store the command
    buttonA.waitForRelease();
  }
}

void stage0LCD(int moveCommand) {                                 //Display selection of commands on LCD display
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
  delay(100);
}



void stage1() { //Select drive speed
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

  stage1LCD(speedSelected);

  if (buttonA.isPressed()) { //Is the button pressed?
    actualStage = 2;  //If yes I jump to next stage and store the command
    buttonA.waitForRelease();
  }
}

void stage1LCD(int moveSpeed) {                                   //Display selection of speeds on LCD display
  LCD.clear();
  LCD.print("Speed>");
  LCD.gotoXY(0, 1);
  LCD.print(moveSpeed);
  delay(100);
}



void stage2() { //Select drive duration
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

  stage2LCD(durationSelected);

  if (buttonA.isPressed()) { //Is the button pressed?
    actualStage = 3;  //If yes I jump to next stage and store the command
    buttonA.waitForRelease();
  }
}

void stage2LCD(int moveDuration) {                                //Display selection of duration on LCD display
  LCD.clear();
  LCD.print("Duration>");
  LCD.gotoXY(0, 1);
  LCD.print(moveDuration);
  delay(100);
}



void stage3(int moveCommand, int moveSpeed, int moveDuration) {   //Run the robot with specified command, speed and duration
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
  victory();
  programReset();
}

void stage3LCD(int moveCommand, int moveDuration) {               //Display what the robot is doing on LCD display
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
      Serial.println("Error (stage3LCD)");
  }
  LCD.clear();
  LCD.gotoXY(0, 1);
  LCD.print(nameCommand);
  delay(100);
}



void moveForward(int moveSpeed, int moveDuration) {               //Function for moving the robot forwards
  motors.setSpeeds(moveSpeed, moveSpeed);
  delay(moveDuration);
}

void moveBackwards(int moveSpeed, int moveDuration) {             //Function for moving the robot backwards
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

void moveStop() {                                                 //Function for stopping the motors of the robot
  motors.setSpeeds(0, 0);
}



void countdown() {                                                //Countsdown and displays it on LCD display
  if (moveCountdown == 5) {
    for (int i = 0; i <= 5; i++) {
      Serial.println(moveCountdown);
      LCD.clear();
      LCD.print("Moving in:");
      LCD.gotoXY(0, 1);
      LCD.print(moveCountdown);
      moveCountdown = moveCountdown - 1;
      delay(1000);
      if (moveCountdown == 0) {
        countdownDone = true;
      }
    }
    Serial.println("moveCountdown done!");
    
  }
}

void beep() {                                                     //Play small beep noise from buzzer
  buzzer.playNote(NOTE_A(4), 20, 15);
  delay(30);
}

void victory() {                                                  //Display success message on LCD display and play victory noise from buzzer
  LCD.clear();
  LCD.gotoXY(0, 1);
  LCD.print("SUCCESS! :)");
  buzzer.playNote(NOTE_D_SHARP(5), 167, 15);
  delay(334);
  buzzer.playNote(NOTE_D_SHARP(5), 167, 15);
  delay(167);
  buzzer.playNote(NOTE_D_SHARP(5), 167, 15);
  delay(167);
  buzzer.playNote(NOTE_A_SHARP(5), 167, 15);
  delay(167);
  delay(2000);
}

void readEncoders() {                                             //Read encoders
  accCountsR = accCountsR + encoders.getCountsAndResetRight();
}

void programReset(){
  actualStage = 0;
  moveCountdown = 5;
  accCountsR = 0;
  encoders.getCountsAndResetRight();
  encoders.getCountsAndResetLeft();
  delay(100);
}

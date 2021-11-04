//Hej med dig 
#include <Wire.h>
#include <Zumo32U4.h>
Zumo32U4Encoders encoders;
Zumo32U4LCD LCD;
Zumo32U4Buzzer buzzer;
Zumo32U4ButtonA buttonA;

bool hasRun_0 = false, hasRun_1 = false, hasRun_2 = false, hasRun_3 = false;

int accCountsR = 0;
int commandSelected = 0; //0 = Forward,  1 = Backwards, 2 = Right, 3 = Left
int speedSelected = 0;
int durationSelected = 0;

int actualStage = 0; //0 = Select command, 1 = Select speed,  2 = Select duration, 3 = Run robot

void setup() {
  Serial.begin(9600);
}

void loop() {
  delay(200);
  if (actualStage == 0) {
    if (hasRun_0 == false) {
      Serial.println("I am in stage 0!");
      hasRun_0 = true;
    }
    stage0command(); //Stage 0 --> select command
  }

  if (actualStage == 1) {
    if (hasRun_1 == false) {
      Serial.println("I am in stage 1!");
      hasRun_1 = true;
    }
    stage1speed(); // Stage 1 --> Select speed
  }

  if (actualStage == 2) {
  if (hasRun_2 == false) {
      Serial.println("I am in stage 2!");
      hasRun_2 = true;
    }
    stage2duration();
  }



  //Stage 3 --> Running the robot
}

void stage0command() {
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

void LCDstage0command(int commandSelec) {
  String nameCommand;
  switch (commandSelec) {
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
      nameCommand = "Error";
  }
  LCD.clear();
  LCD.print("Command>");
  LCD.gotoXY(0, 1);
  LCD.print(nameCommand);
}

void stage1speed() {
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

  LCDStage1speed(speedSelected);

  if (buttonA.isPressed()) { //Is the button pressed?
    actualStage = 2;  //If yes I jump to next stage and store the command
    buttonA.waitForRelease();
  }
}

void LCDStage1speed(int moveSpeed) {
  LCD.clear();
  LCD.print("Speed>");
  LCD.gotoXY(0, 1);
  LCD.print(moveSpeed);
}

void stage2duration() {
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

void LCDstage2duration(int moveDuration) {
  LCD.clear();
  LCD.print("Duration>");
  LCD.gotoXY(0, 1);
  LCD.print(moveDuration);
}

void beep() {
  buzzer.playNote(NOTE_A(4), 20, 15);
  delay(30);
}

void readEncoders() {
  accCountsR = accCountsR + encoders.getCountsAndResetRight();
}

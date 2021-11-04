//Stage1 test // Nu tester jeg ogs //Tobias
#include <Wire.h>
#include <Zumo32U4.h>
Zumo32U4Encoders encoders;
Zumo32U4LCD LCD;
Zumo32U4Buzzer buzzer;
Zumo32U4ButtonA buttonA;

int accCountsR = 0;
int commandSelected = 0; //0 = Forward,  1 = Backwards, 2 = Right, 3 = Left

int actualStage = 0;  //

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  delay(200);
  if (actualStage == 0){
  stage0(); //Stage 0 --> select command
  }

  if (actualStage == 1){
    Serial.println("I am in stage 1!");
    // Stage 1 --> Select speed and time
  }

  

  //Stage 2 --> Running the robot
}

void stage0() {
  readEncoderStage0(); //Read the encoders
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

      if (commandSelected <0) commandSelected = 3;
    }
      
    Serial.println("Count: " + (String)accCountsR + " Command: " + (String)commandSelected);

    LCDStage0(commandSelected);
    //Show it on the screen
      //Title
      //In which command we are

    if (buttonA.isPressed()){ //Is the button pressed?
      actualStage = 1;  //If yes I jump to next stage and store the command
      buttonA.waitForRelease();
    }
      
}

void LCDStage0(int commandSelec) {
  String nameCommand;
  switch(commandSelec) {
    case 0:
      nameCommand = "Forward";
      break;

    case 1:
      nameCommand = "Backward";
      break;

    case 3:
      nameCommand = "Right";
      break;

    case 4:
      nameCommand = "Left";
      break;

    default:
      nameCommand = "Error";
  }
  LCD.clear();
  LCD.print("Command>");
  LCD.gotoXY(0,1);
  LCD.print(nameCommand);

}

void readEncoderStage0() {
  accCountsR = accCountsR + encoders.getCountsAndResetRight();
}

void beep() {
  buzzer.playNote(NOTE_A(4),20,15);
  delay(30);
}

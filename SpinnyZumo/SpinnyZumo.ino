#include <Wire.h>
#include <Zumo32U4.h>

Zumo32U4Motors motors;
Zumo32U4ButtonA buttonA;

int motorSpeed = 200;

void setup() {
  // put your setup code here, to run once:
    buttonA.waitForButton();
    delay(1000);
}

void loop() {
  //spin();
  //circle();
  //turnAround();
  doubleCircle();
}

void spin() {
  motors.setLeftSpeed(motorSpeed);
  motors.setRightSpeed(- motorSpeed);
  
  delay(2000);

  motors.setLeftSpeed(0);
  motors.setRightSpeed(0);

  delay(50);

  motors.setLeftSpeed(- motorSpeed);
  motors.setRightSpeed(motorSpeed);

  delay(2000);

  motors.setLeftSpeed(0);
  motors.setRightSpeed(0);

  delay(50);
}

void circle() {
  motors.setLeftSpeed(motorSpeed);
  motors.setRightSpeed(motorSpeed / 5);

  delay(3700);
  
  motors.setLeftSpeed(0);
  motors.setRightSpeed(0);

  delay(200);

  turnAround();

  motors.setLeftSpeed(motorSpeed / 5);
  motors.setRightSpeed(motorSpeed);

  delay(3700);

  motors.setLeftSpeed(0);
  motors.setRightSpeed(0);

  delay(200);

  turnAround();
  
  
  /*
  delay(50);

  motors.setLeftSpeed(motorSpeed / 5);
  motors.setRightSpeed(motorSpeed);

  delay(2000);

  motors.setLeftSpeed(0);
  motors.setRightSpeed(0);

  delay(50); */
}

void turnAround () {
  motors.setLeftSpeed(300);
  motors.setRightSpeed(- 300);

  delay(425);

  motors.setLeftSpeed(0);
  motors.setRightSpeed(0);

  delay(200);
}

void doubleCircle() {
  motors.setLeftSpeed(motorSpeed);
  motors.setRightSpeed(motorSpeed / 5);

  delay(3700);

  motors.setLeftSpeed(0);
  motors.setRightSpeed(0);

  delay(200);

  turnAround();

  motors.setLeftSpeed(motorSpeed / 5);
  motors.setRightSpeed(motorSpeed);

  delay(3700);

  motors.setLeftSpeed(0);
  motors.setRightSpeed(0);

  delay(200);

  turnAround();

  
}

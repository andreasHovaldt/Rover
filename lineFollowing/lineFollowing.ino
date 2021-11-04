#include <Wire.h>
#include <Zumo32U4.h>

Zumo32U4LineSensors lineSensors;
Zumo32U4Motors motors;
Zumo32U4ButtonA buttonA;
Zumo32U4LCD lcd;

int16_t lastError = 0;

#define NUM_SENSORS 5
unsigned int lineSensorValues[NUM_SENSORS];

// Set up custom characters in LCD so we can
// show bar graphs
void loadCustomCharacters() {
    static const char levels[] PROGMEM = {
        0, 0, 0, 0, 0, 0, 0, 63, 63, 63, 63, 63, 63, 63
    };
    lcd.loadCustomCharacter(levels + 0, 0); // 1 bar
    lcd.loadCustomCharacter(levels + 1, 1); // 2 bar
    lcd.loadCustomCharacter(levels + 2, 2); // 3 bar
    lcd.loadCustomCharacter(levels + 3, 3); // 4 bar
    lcd.loadCustomCharacter(levels + 4, 4); // 5 bar
    lcd.loadCustomCharacter(levels + 5, 5); // 6 bar
    lcd.loadCustomCharacter(levels + 6, 6); // 7 bar
}

void printBar(uint8_t height) {
    if (height > 8) {height = 8; }
    const char barChars[] = {' ', 0, 1, 2, 3, 4, 5, 6, 255};
    lcd.print(barChars[height]);
}

void calibrateSensors() {
    lcd.clear();

    // Wait 1 second, then begin auto sensor calibration
    // by rotating in place to sweep the sensors over line
    delay(1000);
    for(unit16_t i = 0; i < 120; i++) {
        if (i > 30 && <= 90) {
        motors.setSpeeds(-200, 200);
        } else {
            motors.setSpeeds(200,-200);
        }

        lineSensors.calibrate();
    }
    motors.setSpeeds(0,0);
}

// Display bar graph of sensor readings on LCD
// Returns after user presses A
void showReadings() {
    lcd.clear();

    while(!buttonA.getSingleDebouncePress()) {
        lineSensors.readCalibrated(lineSensorValues);

        lcd.gotoXY(0,0);
        for (uint16_t i = 0; i < NUM_SENSORS; i++) {
            uint8_t barHeight = map(lineSensorValues[i], 0, 1000, 0, 8);
            printBar(barHeight);
        }
    }
}

void setup() {
    // Just uncomment below if the motors are flipped
    //motors.flipLeftMotor(true);
    //motors.flipRightMotor(true);

    lineSensors.initFiveSensors();

    loadCustomCharacters();

    // Wait for button A press and release
    lcd.clear();
    lcd.print(F("Press A"));
    lcd.gotoXY(0,1);
    lcd.print(F("to calibrate"));
    buttonA.waitForButton();

    calibrateSensors();

    showReadings();
}

void loop() {
    // Get pos of line. Need "lineSensorValues" even though
    // we aren't interested in each sensor reading
    int16_t position = lineSensors.readLine(lineSensorValues);

    // "Error" is distance from center of line
    // corresponding position is 2000
    int16_t error = position - 2000;

    // Get motor speed difference with proportional and derivative
    // PID terms (integral part not useful for line following)
    // Proportional constant of 1/4 and derivative constant 6.
    // Trial and error if seems wonky, depends on line course
    int16_t speedDifference = error / 4 + 6*(error - lastError);

    lastError = error;

    // Get individual motor speed. Sign of speedDifference
    // determines turn left or right
    int16_t leftSpeed = (int16_t)maxSpeed + speedDifference;
    int16_t rightSpeed = (int16_t)maxSpeed - speedDifference;

    // Constrain motor speed to between 0 and maxSpeed
    // One motor will always turn maxSpeed, other will be
    // maxSpeed -speedDifference if that positive, else
    // stationary. 
    leftSpeed = constrain(leftSpeed, 0, (int16_t)maxSpeed);
    rightSpeed = constrain(rightSpeed, 0, (int16_t)maxSpeed);

    motors.setSpeeds(leftSpeed, rightSpeed);
}

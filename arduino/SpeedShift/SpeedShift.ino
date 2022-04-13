#include <Arduino.h>
#include "AdvancedSevenSegment.h"
#include <Servo.h>
#include "JC_Button.h"
/*                                a   b   c   d   e  f   g  .*/
AdvanceSevenSegment sevenSegment(D2, D1, D5, D6, D7, D3, D4, D0);
Servo servo;

const String hex = "123456789ABCDEF";
const uint32_t gearCount = 12;
int currentGear = 1;
uint32_t configuratingGear;
uint32_t position;
const unsigned long
SHORT_PRESS(250),
            LONG_PRESS(500);

bool configShort;
bool shiftDownShort;
bool shiftUpShort;
bool configLong;
bool shiftDownLong;
bool shiftUpLong;

uint8_t lastAction = 0; // 1 = up, -1 = down, 0 = default

uint32_t lastdisplay;

Button shiftDownButton(A0, 25, false, false, 536);
Button shiftUpButton(A0, 25, false, false, 806);
Button configButton(A0, 25, false, false, 1024);

int gearPositions[] = {
  10,
  20,
  30,
  45,
  60,
  70,
  85,
  105,
  120,
  140,
  165,
  180,
};


void setup() {
  sevenSegment.clean();

  servo.attach(D8);
  //move(servo, 0);
  servo.write(0);
  shiftDownButton.begin();
  shiftUpButton.begin();
  configButton.begin();
  lastdisplay = millis();
  Serial.begin(9600);
}

void loop() {
  shiftDownButton.read();
  shiftUpButton.read();
  configButton.read();

  delay(1); // Add Stability
  
  if (configButton.wasPressed()) {
    //Serial.println("Config");
    sevenSegment.setDot(1);
    if (configuratingGear != 0 ) {
      //EEPROM.update(configuratingGear, gearPositions[configuratingGear - 1]);
      Serial.println("Saving gear " + String(configuratingGear) + " to " + String(gearPositions[configuratingGear - 1]));
      configuratingGear = 0;
      sevenSegment.setDot(0);
      lastdisplay = millis() - 3000;
    } else {
      // Correction for last registered up or downpress
      if (lastAction == 1) {
        if (currentGear > 1) currentGear = currentGear - 1;
      } else if (lastAction == -1) {
        if (currentGear < gearCount) currentGear = currentGear + 1;
      }
      configuratingGear = currentGear;
      move(servo, 0);
      Serial.println("Start configurating " + String(configuratingGear));
    }
  }

  if (shiftDownButton.wasPressed() && !shiftUpButton.isPressed() && !shiftUpButton.wasPressed()) {
    Serial.println("DOWN");
    if (configuratingGear) {
      gearPositions[configuratingGear - 1] = gearPositions[configuratingGear - 1] - 1;
      Serial.println("Changed gear " + String(configuratingGear) + " position to: " + String(gearPositions[configuratingGear - 1]));
      move(servo, 0);
    } else {
      lastAction = -1;
      move(servo, -1);
    }
  }

  if (shiftUpButton.wasPressed() && !shiftDownButton.isPressed() && !shiftDownButton.wasPressed()) {
    //Serial.println("UP");
    if (configuratingGear) {
      gearPositions[configuratingGear - 1] = gearPositions[configuratingGear - 1] + 1;
      Serial.println("Changed gear " + String(configuratingGear) + " position to: " + String(gearPositions[configuratingGear - 1]));
      move(servo, 0);
    } else {
      lastAction = 1;
      move(servo, 1);
    }
  }

  // Detect release Downshift
  if (configuratingGear == 0) {
    if (shiftDownButton.wasReleased()) {
      if (shiftDownShort && !shiftDownLong) {
        move(servo, -2);
        Serial.println("SHORT DOWN");
        shiftDownShort = false;
      } else if (shiftDownLong) {
        move(servo, -4);
        Serial.println("LONG DOWN");
        shiftDownShort = false;
        shiftDownLong = false;
      } else {
        //Serial.println("RELEASED DOWN");
      }
    } else if (shiftDownButton.pressedFor(SHORT_PRESS) && !shiftDownButton.pressedFor(LONG_PRESS)) {
      shiftDownShort = true;
    } else if (shiftDownButton.pressedFor(LONG_PRESS)) {
      shiftDownLong = true;
    }

    // Detect release Upshift
    if (shiftUpButton.wasReleased()) {
      if (shiftUpShort && !shiftUpLong) {
        move(servo, 2);
        Serial.println("SHORT UP");
        shiftUpShort = false;
      } else if (shiftUpLong) {
        move(servo, 4);
        Serial.println("LONG UP");
        shiftUpShort = false;
        shiftUpLong = false;
      } else {
        //Serial.println("RELEASED UP");
      }
    } else if (shiftUpButton.pressedFor(SHORT_PRESS) && !shiftUpButton.pressedFor(LONG_PRESS)) {
      shiftUpShort = true;
    } else if (shiftUpButton.pressedFor(LONG_PRESS)) {
      shiftUpLong = true;
    }
  }
  //Valid range is from 1 to 9
  if (currentGear >= 1 && currentGear <= gearCount) {
    if (configuratingGear == 0 && millis() > lastdisplay + 3500 ) {
      sevenSegment.clean();
    }else {
        if (currentGear < 10) {
    sevenSegment.setNumber(currentGear);
  } else {
    sevenSegment.setCharacter(hex[currentGear -1]);
  }
    }
  }  


}

void move(Servo servo, int amount) {
  currentGear = currentGear + amount;
  if (currentGear < 1) currentGear = 1;
  if (currentGear > gearCount) currentGear = gearCount;
  if (position != gearPositions[currentGear - 1]) {
    position = gearPositions[currentGear - 1];
    Serial.println("goto: " + String(position) + ", gear: " + hex[currentGear -1]);
    servo.write(position);
    delay(15); // waits 15ms for the servo to reach the position
  }
  lastdisplay = millis();
}

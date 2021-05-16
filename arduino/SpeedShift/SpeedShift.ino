/* Arduino Tutorial - 7 Segment
   One 7 segment is one digit, from 0 to  9.
   Dev: Vasilakis Michalis // Date: 25/7/2015 // www.ardumotive.com */

//Library
#include "SevenSeg.h"
#include "JC_Button.h"
#include <Servo.h>
#include <EEPROM.h>

//Defines the segments A-G: SevenSeg(A, B, C, D, E, F, G);
SevenSeg disp (10, 9, 8, 7, 6, 11, 12); // UNO
//SevenSeg disp (6, 5, A3, A1, A0, 8, 9); // NANO
//SevenSeg disp (4, 3, 7, 6, 5, 2, 1); // D1 Mini

//Number of 7 segments
const int numOfDigits = 1;
//CC(or CA) pins of segment
int digitPins [numOfDigits] = {4};

//Variables
int flag;
int position;
int configuratingGear;
bool dot;
const unsigned long
SHORT_PRESS(250),
            LONG_PRESS(500);

int gearPositions[] = {
  10,
  45,
  55,
  65,
  75,
  85,
  95,
  105,
  115,
  125,
  135,
  145,
};

const String hex = "0123456789ABCDEF";

int gearCount = 12;

int currentGear = 1;

Servo servo;

Button shiftDownButton(A3, 25, false, false);
Button shiftUpButton(A4, 25, false, false);

bool shiftDownShort;
bool shiftUpShort;
bool shiftDownLong;
bool shiftUpLong;
bool bothPressed;

bool lastAction; // true = up, false = down

long lastdisplay;

void setup() {

  // NANO
  //pinMode(A0, OUTPUT); // D1 Mini, Only enable this pin
  //  pinMode(A1, OUTPUT);
  //  pinMode(A2, OUTPUT);
  //  pinMode(A3, OUTPUT);
  //  pinMode(A4, OUTPUT);

  Serial.begin(9600);
  //Defines the number of digits to be "numOfDigits" and the digit pins to be the elements of the array "digitPins"
  disp.setDigitPins ( numOfDigits , digitPins );
  disp.setDPPin(5); // NANO, UNO
  //disp.setDPPin(5); // D1 Mini
  //Only for common cathode 7segments
  disp.setCommonCathode();
  //Control brightness (values 0-100);
  disp.setDutyCycle(50);
  disp.writeDigit(' ');

  shiftDownButton.begin();
  shiftUpButton.begin();

  servo.attach(13);

  // Read last saved position
  //if (EEPROM.read(0) != currentGear) {
  //  currentGear = EEPROM.read(0);
  //}

  //      Serial.println("Writing initial values");
  //  for (int i = 0; i < gearCount + 1; i++) {
  //    EEPROM.write(i, gearPositions[i -1]);
  //  }
  currentGear = 1;

  gearPositions[0] = EEPROM.read(1);
  gearPositions[1] = EEPROM.read(2);
  gearPositions[2] = EEPROM.read(3);
  gearPositions[3] = EEPROM.read(4);
  gearPositions[4] = EEPROM.read(5);
  gearPositions[5] = EEPROM.read(6);
  gearPositions[6] = EEPROM.read(7);
  gearPositions[7] = EEPROM.read(8);
  gearPositions[8] = EEPROM.read(9);
  gearPositions[9] = EEPROM.read(10);
  gearPositions[10] = EEPROM.read(11);
  gearPositions[11] = EEPROM.read(12);
  Serial.println("Read all gear positions");
  for (int i = 0; i < gearCount; i++) {
    Serial.println(String(i + 1) + ": " + String(gearPositions[i]));
  }

  move(servo, 0);
  configuratingGear = 0;

  lastdisplay = millis();
}

void loop()
{
  //Check if incoming data is available:
  if (Serial.available() > 0)
  {
    // If it is, we'll use parseInt() to pull out only numbers:
    currentGear = Serial.parseInt();
    flag = 0;
  }


  //Valid range is from 1 to 9
  if (currentGear >= 1 && currentGear <= gearCount) {

    if (configuratingGear == 0 && millis() > lastdisplay + 3500 ) {
      disp.writeDigit(' ');
      disp.clearDP();
      dot = false;
    } else {
      if (dot)
        disp.setDP();
      //Print number to 7 segment display
      disp.writeDigit(hex[currentGear]);
    }

    //Print message to serial monito only once
    if (flag == 0) {
      //Print number to serial monitor
      //Serial.println(hex[currentGear]);
      flag = 1;
    }
  }

  shiftDownButton.read();
  shiftUpButton.read();

  if (!bothPressed && shiftUpButton.isPressed() && shiftDownButton.isPressed()) {
    //Serial.println("BOTH PRESSED");
    //Serial.println(currentGear);
    bothPressed = true;
    //EEPROM.update(0, currentGear);
    dot = true;
    if (configuratingGear != 0 ) {
      EEPROM.update(configuratingGear, gearPositions[configuratingGear - 1]);
      Serial.println("Saving gear " + String(configuratingGear) + " to " + String(gearPositions[configuratingGear - 1]));
      configuratingGear = 0;
      dot = false;
      lastdisplay = millis() - 3000;
    } else {
      // Correction for last registered up or downpress
      if (lastAction) {
        if (currentGear > 1) currentGear = currentGear - 1;
      } else {
        if (currentGear < gearCount) currentGear = currentGear + 1;
      }
      configuratingGear = currentGear;
      move(servo, 0);
      Serial.println("Start configurating " + String(configuratingGear));
    }
  } else {
    // Start shifting
    if (shiftDownButton.wasPressed() && !shiftUpButton.isPressed() && !shiftUpButton.wasPressed()) {
      //Serial.println("DOWN");
      if (configuratingGear) {
        gearPositions[configuratingGear - 1] = gearPositions[configuratingGear - 1] - 1;
        Serial.println("Changed gear " + String(configuratingGear) + " position to: " + String(gearPositions[configuratingGear - 1]));
        bothPressed = false;
        move(servo, 0);
      } else {
        lastAction = false;
        move(servo, -1);
      }
    }
    if (shiftUpButton.wasPressed() && !shiftDownButton.isPressed() && !shiftDownButton.wasPressed()) {
      //Serial.println("UP");
      if (configuratingGear) {
        gearPositions[configuratingGear - 1] = gearPositions[configuratingGear - 1] + 1;
        Serial.println("Changed gear " + String(configuratingGear) + " position to: " + String(gearPositions[configuratingGear - 1]));
        bothPressed = false;
        move(servo, 0);
      } else {
        lastAction = true;
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
        bothPressed = false;
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
        bothPressed = false;
      } else if (shiftUpButton.pressedFor(SHORT_PRESS) && !shiftUpButton.pressedFor(LONG_PRESS)) {
        shiftUpShort = true;
      } else if (shiftUpButton.pressedFor(LONG_PRESS)) {
        shiftUpLong = true;
      }
    }
  }

}

void move(Servo servo, int gear) {
  currentGear = currentGear + gear;
  if (currentGear < 1) currentGear = 1;
  if (currentGear > gearCount) currentGear = gearCount;
  if (position != gearPositions[currentGear - 1]) {
    position = gearPositions[currentGear - 1];
    servo.write(position);
    Serial.println("goto: " + String(position)  + ", gear: " + hex[currentGear]);
    flag = 0;
  }
  lastdisplay = millis();
}

/* Arduino Tutorial - 7 Segment
   One 7 segment is one digit, from 0 to  9.
   Dev: Vasilakis Michalis // Date: 25/7/2015 // www.ardumotive.com */

//Library
#include "SevenSeg.h"
#include "JC_Button.h"
#include <Servo.h>
#include <EEPROM.h>

//Defines the segments A-G: SevenSeg(A, B, C, D, E, F, G);
//SevenSeg disp (10, 9, 8, 7, 6, 11, 12); // UNO
SevenSeg disp (6, 5, A3, A1, A0, 8, 9); // Mini UNO

//Number of 7 segments
const int numOfDigits = 1;
//CC(or CA) pins of segment
int digitPins [numOfDigits] = {4};

//Variables
int flag;
int position;
const unsigned long
SHORT_PRESS(250),
            LONG_PRESS(500);

const int gearPositions[] = {
  10,
  20,
  30,
  40,
  50,
  60,
  70,
  80,
  90,
  100,
  110,
  120,
};

const String hex = "0123456789ABCDEF";

int gearCount = 12;

int currentGear = 1;

Servo servo;

Button shiftDownButton(2, 25, false, false);
Button shiftUpButton(3, 25, false, false);

bool shiftDownShort;
bool shiftUpShort;
bool shiftDownLong;
bool shiftUpLong;
bool bothPressed;

void setup() {

  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
  pinMode(A4, OUTPUT);

  Serial.begin(9600);
  //Defines the number of digits to be "numOfDigits" and the digit pins to be the elements of the array "digitPins"
  disp.setDigitPins ( numOfDigits , digitPins );
  //Only for common cathode 7segments
  disp.setCommonCathode();
  //Control brightness (values 0-100);
  disp.setDutyCycle(50);

  shiftDownButton.begin();
  shiftUpButton.begin();

  servo.attach(13);

  if (EEPROM.read(0) != currentGear) {
    currentGear = EEPROM.read(0);
  }
  move(servo, 0);
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
    //Print number to 7 segment display
    disp.writeDigit(hex[currentGear]);

    //Print message to serial monito only once
    if (flag == 0) {
      //Print number to serial monitor
      Serial.print("Number on 7 segment display:");
      Serial.println(hex[currentGear]);
      flag = 1;
    }
  }

  shiftDownButton.read();
  shiftUpButton.read();

  if (!bothPressed && shiftUpButton.isPressed() && shiftDownButton.isPressed()) {
    Serial.println("BOTH PRESSED");
    Serial.println(currentGear);
    bothPressed = true;
    EEPROM.update(0, currentGear);
  } else {
    // Start shifting
    if (shiftDownButton.wasPressed()) {
      Serial.println("DOWN");
      if (!shiftUpButton.wasPressed())
        move(servo, -1);
    }
    if (shiftUpButton.wasPressed()) {
      Serial.println("UP");
      if (!shiftDownButton.wasPressed())
        move(servo, 1);
    }

    // Detect release Downshift
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
        Serial.println("RELEASED DOWN");
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
        Serial.println("RELEASED UP");
      }
      bothPressed = false;
    } else if (shiftUpButton.pressedFor(SHORT_PRESS) && !shiftUpButton.pressedFor(LONG_PRESS)) {
      shiftUpShort = true;
    } else if (shiftUpButton.pressedFor(LONG_PRESS)) {
      shiftUpLong = true;
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
  }
}

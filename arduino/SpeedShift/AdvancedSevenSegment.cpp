#include <Arduino.h>
#include "AdvancedSevenSegment.h"

AdvanceSevenSegment::AdvanceSevenSegment(int a, int b, int c, int d, int e, int f, int g, int dot) {
    _a = a;
    _b = b;
    _c = c;
    _d = d;
    _e = e;
    _f = f;
    _g = g;
    _dot = dot;
    _cc = false;
    pinMode(a, OUTPUT);
    pinMode(b, OUTPUT);
    pinMode(c, OUTPUT);
    pinMode(d, OUTPUT);
    pinMode(e, OUTPUT);
    pinMode(f, OUTPUT);
    pinMode(g, OUTPUT);
    pinMode(dot, OUTPUT);
}

void AdvanceSevenSegment::setNumber(int number) {
    switch (number) {
        case 0:
            print(1, 1, 1, 1, 1, 1, 0);
            break;
        case 1:
            print(0, 1, 1, 0, 0, 0, 0);
            break;
        case 2:
            print(1, 1, 0, 1, 1, 0, 1);
            break;
        case 3:
            print(1, 1, 1, 1, 0, 0, 1);
            break;
        case 4:
            print(0, 1, 1, 0, 0, 1, 1);
            break;
        case 5:
            print(1, 0, 1, 1, 0, 1, 1);
            break;
        case 6:
            print(1, 0, 1, 1, 1, 1, 1);
            break;
        case 7:
            print(1, 1, 1, 0, 0, 0, 0);
            break;
        case 8:
            print(1, 1, 1, 1, 1, 1, 1);
            break;
        case 9:
            print(1, 1, 1, 1, 0, 1, 1);
            break;

            // for unknown number display dash
        default:
            setDash();

    }

}

void AdvanceSevenSegment::setCharacter(char c) {
    //support UpperCase chars
    c = tolower(c);

    switch (c) {
        case 'a':
            print(1, 1, 1, 0, 1, 1, 1);
            break;
        case 'b':
            print(0,0,1,1,1,1,1);
            break;
        case 'c':
            print(1, 0, 0, 1, 1, 1, 0);
            break;
        case 'd':
            print(0, 1, 1, 1, 1, 0, 1);
            break;
        case 'e':
            print(1, 0, 0, 1, 1, 1, 1);
            break;
        case 'f':
            print(1, 0, 0, 0, 1, 1, 1);
            break;
        case 'h':
            print(0, 1, 1, 0, 1, 1, 1);
            break;
        case 'l':
            print(0, 0, 0, 1, 1, 1, 0);
            break;
        case 'p':
            print(1, 1, 0, 0, 1, 1, 1);
            break;
        case 'q':
            print(1, 1, 1, 0, 0, 1, 1);
            break;
        case 'u':
            print(0, 1, 1, 1, 1, 1, 0);
            break;
        case 'y':
            print(0, 1, 1, 1, 0, 1, 1);
            break;

            // for unknown character print dash
        default:
            setDash();
    }

}

void AdvanceSevenSegment::print(int a, int b, int c, int d, int e, int f, int g) {
    if(_cc) {
      digitalWrite(_a, a);
      digitalWrite(_b, b);
      digitalWrite(_c, c);
      digitalWrite(_d, d);
      digitalWrite(_e, e);
      digitalWrite(_f, f);
      digitalWrite(_g, g);
    } else {
      digitalWrite(_a, !a);
      digitalWrite(_b, !b);
      digitalWrite(_c, !c);
      digitalWrite(_d, !d);
      digitalWrite(_e, !e);
      digitalWrite(_f, !f);
      digitalWrite(_g, !g);
    }
}

void AdvanceSevenSegment::setDot(int state) {
    if(_cc) {
      digitalWrite(_dot, state);
    } else {
      digitalWrite(_dot, !state);
    }
}

void AdvanceSevenSegment::setDash() {
    print(0, 0, 0, 0, 0, 0, 1);
}

void AdvanceSevenSegment::refresh(int ms) {
    clean();
    setDot(1);
    delay(ms);
    clean();
    delay(50);
}

void AdvanceSevenSegment::clean() {
    setDot(0);
    print(0, 0, 0, 0, 0, 0, 0);
}

void AdvanceSevenSegment::setCommonCathode() {
    _cc = true;
}

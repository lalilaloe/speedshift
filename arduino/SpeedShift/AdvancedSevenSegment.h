#ifndef AdvanceSevenSegment_H
#define AdvanceSevenSegment_H
#include "Arduino.h"

class AdvanceSevenSegment {

public:
    //first pins setup
    explicit AdvanceSevenSegment(int a, int b, int c, int d, int e, int f, int g, int dot);

    //for display numbers[0..9] use this function
    void setNumber(int number);

    /*for display some specific char
    supported char: {'a','b', 'c','d', 'e', 'f', 'h', 'l', 'p', 'q', 'u', 'y'}*/
    void setCharacter(char c);

    //used for manual config
    void print(int a, int b, int c, int d, int e, int f, int g);

    //display dot led by [0 , 1] state
    void setDot(int state);

    //display center dash
    void setDash();

    //clean and show dot for (default: 1500 ms)
    void refresh(int ms = 1000);

    //set LOW for all pins
    void clean();

    //set display type to Common Cathode
    void setCommonCathode();

private:
    int _a, _b, _c, _d, _e, _f, _g, _cc, _dot;

};
#endif

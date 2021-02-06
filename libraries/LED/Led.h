/*************************
led.h
*************************/
#ifndef Led_h
#define Led_h
#include "Arduino.h"

class Led {
public: 
Led(int pin);
void fast();
void slow();
private:
int _pin;
};

#endif
/***************
Emancipator Library first development
led.cpp
***************/
#include "Arduino.h"
#include "Led.h"

Led::Led(int pin){
pinMode(pin, OUTPUT);
_pin = pin; }

void Led::slow(){
digitalWrite(_pin, HIGH);
  delay(500);
  digitalWrite(_pin, LOW);
  delay(500);
  digitalWrite(_pin, HIGH);
  delay(500);
  digitalWrite(_pin, LOW);
  delay(500);
  digitalWrite(_pin, HIGH);
  delay(500);
  digitalWrite(_pin, LOW);
  delay(500);  }

void Led::fast(){
digitalWrite(_pin, HIGH);
  delay(200);
  digitalWrite(_pin, LOW);
  delay(200);
  digitalWrite(_pin, HIGH);
  delay(200);
  digitalWrite(_pin, LOW);
  delay(200);
  digitalWrite(_pin, HIGH);
  delay(200);
  digitalWrite(_pin, LOW);
  delay(200);  }
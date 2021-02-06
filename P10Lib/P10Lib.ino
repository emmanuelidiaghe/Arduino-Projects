/*
 * P10 section of the
 * Emancipator library
 */ 
#define usingArduino
#define usingP10

#define DISPLAYS_ACROSS 3
#define DISPLAYS_DOWN 1
#define max_char 100

#include "C://Emancipator/Emancipator.h"

void setup()
{
  start();
}

void loop()
{
  input();

  ask();

  scroll();
}


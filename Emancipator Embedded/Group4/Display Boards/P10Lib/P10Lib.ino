/*
 * P10 section of the
 * Emancipator library
 */ 
#define usingArduino
#define usingSerialP10

char inputMsg1 = "";
char inputMsg2 = "";
char inputMsg3 = "";
char inputMsg4 = "";
char inputMsg5 = "";
char inputMsg6 = "";
char inputMsg7 = "";

#define DISPLAYS_ACROSS 3
#define DISPLAYS_DOWN 1
#define max_char 100

#include "Arial_black_16.h"
#include "C://Emancipator/Emancipator.h"
P10 disp;

void setup()
{
  disp.start();
}

void loop()
{
  disp.input();

  disp.scroll();
}


#include <SPI.h>                                           // for DMD display
#include <DMD2.h>                                           // also, for DMD display
#include "SystemFont5x7.h"                                 // font used for brightness setting confirmation

// DMD2 brightness tester.  Cycles through values to show effect of dimming.
 
SoftDMD dmd(1,1);  // DMD controls the entire display

// cycle through these values
const int dimmerValues[12] = {
  0, 10, 16, 25, 40, 64, 100, 160, 250, 400, 640, 1024};
unsigned long brightnessUpdateTime;
int dimmerIdx;
int incrementer = 1;

// the setup routine runs once when you press reset:
void setup() {
  dmd.setBrightness(255);
  dmd.selectFont(SystemFont5x7);
  dmd.begin();
}

// ---------------------------------------------------------------------------------
//
void loop() {
  if(millis() > brightnessUpdateTime) {
    int Brightness = dimmerValues[dimmerIdx];
    dimmerIdx += incrementer;
    if(dimmerIdx < 0) { 
      dimmerIdx = 1; 
      incrementer = 1;
    }
    if(dimmerIdx > 11) {
      dimmerIdx = 10; 
      incrementer = -1;
    }
    dmd.setBrightness(Brightness);
//    dmd.drawFilledBox(0,0,31,15, false); // Passing false here clears the box
    String power = String(Brightness);
    dmd.drawString(0,0,"PWM:");
    dmd.drawString(0,8,String(Brightness));
    brightnessUpdateTime = millis() + 500;
  }
}

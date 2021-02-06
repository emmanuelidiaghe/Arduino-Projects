/*
  Use the Marquee function to make an LED-sign type display

  This marquee function scrolls in all directions, random distances for each
  direction. If you edit the code in the "switch" statement below then you can
  make a simpler marquee that only scrolls in one direction.
*/
#include <SPI.h>
#include <DMD2.h>
#include <fonts/Arial_black_16.h>

#define DISPLAYS_WIDE 3
#define DISPLAYS_HIGH 1

SoftDMD dmd(DISPLAYS_WIDE,DISPLAYS_HIGH);
DMD_TextBox box(dmd);//, 0, 0, 32, 16);

const char *MESSAGE = "WELCOME TO EMMY_BEST RESTAURANT & BAR    ";

void setup() {
  dmd.setBrightness(255);
  dmd.selectFont(Arial_Black_16);
  dmd.begin();

  const char *next = MESSAGE;
  while(*next) {
    Serial.print(*next);
    box.print(*next);
    delay(200);
    next++;
  }
  dmd.clearScreen();
  delay(500);
}
int phase1 = 0;
int phase2 = 0;
int phase3 = 0;
int phase4 = 0;
int count1 = 0;

void loop() {
  while(count1==0 || count1<4)
  {
  dmd.drawString(0, 0, F("PEPP.SOUP  "));
  int steps1 = 50; // Each time we scroll a random distance
  for(int i = 0; i < steps1; i++) {
    // Do a different type of scroll, depending on which phase we are in
    switch(phase1) {
      case 0:
       dmd.marqueeScrollX(1); break;
      case 1:
       dmd.marqueeScrollX(-1); break;
       case 2:
       dmd.marqueeScrollY(1); break;
       case 3:
       dmd.marqueeScrollY(-1); break;
    }
    delay(10);
  }
  // Move to the next phase
  phase1 = (phase1 + 1) % 4;
  count1++;
  }
  dmd.clearScreen();
  delay(100);

  while(count1==4 || count1<8)
  {
  dmd.drawString(0, 0, F("  GULDER     "));
  int steps2 = 50; // Each time we scroll a random distance
  for(int l = 0; l < steps2; l++) {
    // Do a different type of scroll, depending on which phase we are in
    switch(phase2) {
      case 0:
       dmd.marqueeScrollX(1); break;
      case 1:
       dmd.marqueeScrollX(-1); break;
       case 2:
       dmd.marqueeScrollY(1); break;
       case 3:
       dmd.marqueeScrollY(-1); break;
    }
    delay(10);
  }
  // Move to the next phase
  phase2 = (phase2 + 1) % 4;
  count1++;
  }
  dmd.clearScreen();
  delay(500);

  while(count1==8 || count1<12)
  {
  dmd.drawString(0, 0, F("   STOUT    "));
  int steps3 = 50; // Each time we scroll a random distance
  for(int k = 0; k < steps3; k++) {
    // Do a different type of scroll, depending on which phase we are in
    switch(phase3) {
      case 0:
       dmd.marqueeScrollX(1); break;
      case 1:
       dmd.marqueeScrollX(-1); break;
       case 2:
       dmd.marqueeScrollY(1); break;
       case 3:
       dmd.marqueeScrollY(-1); break;
    }
    delay(10);
  }
  // Move to the next phase
  phase3 = (phase3 + 1) % 4;
  count1++;
  }
  dmd.clearScreen();
  delay(500);

  while(count1==12 || count1<16)
  {
  dmd.drawString(0, 0, F("HENEIKEN   "));
  int steps4 = 50; // Each time we scroll a random distance
  for(int j = 0; j < steps4; j++) {
    // Do a different type of scroll, depending on which phase we are in
    switch(phase4) {
      case 0:
       dmd.marqueeScrollX(1); break;
      case 1:
       dmd.marqueeScrollX(-1); break;
       case 2:
       dmd.marqueeScrollY(1); break;
       case 3:
       dmd.marqueeScrollY(-1); break;
    }
    delay(10);
  }
  // Move to the next phase
  phase4 = (phase4 + 1) % 4;
  count1++;
  }
  dmd.clearScreen();
  delay(500);

  const char *next = MESSAGE;
  while(*next) {
    Serial.print(*next);
    box.print(*next);
    delay(200);
    next++;
  }
  count1 = 0;
  dmd.clearScreen();
  delay(500);
}

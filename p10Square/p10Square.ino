
#include <SPI.h> 
#include <DMD.h>
#include <TimerOne.h>
//#include "Arial14.h"
#include "C:\Users\Emancipator\Documents\Arduino\libraries\DMD\fonts\Droid_Sans_12.h"
#include "Arial_black_16.h"

#define DISPLAYS_ACROSS 2
#define DISPLAYS_DOWN 1
DMD dmd(DISPLAYS_ACROSS, DISPLAYS_DOWN);

int phase = 0;

void ScanDMD()
{ 
  dmd.scanDisplayBySPI();
}

void setup(void)
{
   Timer1.initialize( 5000 );          
   Timer1.attachInterrupt( ScanDMD );
   
   dmd.clearScreen( true );
}

void loop(void)
{
   //dmd.selectFont(Arial_14); 
   dmd.selectFont(Arial_Black_16);   
  // dmd.selectFont(Droid_Sans_12);
   delay(1000);


   dmd.drawString(0,0,"SHOES",5,GRAPHICS_NORMAL);
   delay(5000);
 
}


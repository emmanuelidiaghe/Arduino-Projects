
#include <SPI.h>        
#include <DMD.h>        
#include <TimerOne.h> 
#include <EEPROMAnything.h>  
#include "Arial_black_16.h"

#define DISPLAYS_ACROSS 3
#define DISPLAYS_DOWN 1
#define max_char 100                       //number max of characters in your message
DMD dmd(DISPLAYS_ACROSS, DISPLAYS_DOWN);

char message[max_char];    // stores your message
char message1[max_char];   // EEPROM message
char r_char;               // reads each character
byte index = 0;            // defines the position into your array
int count = 0;             // defines EEPROM Read or Write
int i;   

void ScanDMD()
{ 
  dmd.scanDisplayBySPI();
}

void setup(void)
{
   Timer1.initialize( 5000 );           
   Timer1.attachInterrupt( ScanDMD );
  
   dmd.clearScreen( true );
   Serial.begin(9600);

   EEPROM_readAnything(0, message1);
}

void loop(void)
{
   if(Serial.available())
   {       
        for(i=0; i<99; i++){
            message[i] = '\0';
        }
        index=0;
        count=0;
    }
 
    while(Serial.available() > 0)
    {
       dmd.clearScreen( true );
       if(index < (max_char-1)) 
       {         
           r_char = Serial.read();      // Reads a character
           message[index] = r_char;     // Stores the character in message array
           index++;                     // Increment position
          // message[index] = '\0';       // Delete the last position
       }
       count = 1;
   }

   if(count == 1)
   {
    delay(100);
    EEPROM_writeAnything(0, message);
    count = 2;
   }

   if (count == 2)
   {
    delay(100);
    EEPROM_readAnything(0, message1);
    count = 3;
   }
   
   dmd.selectFont(Arial_Black_16);
   dmd.drawMarquee(message1,max_char,(32*DISPLAYS_ACROSS)-1 ,0);
   long start=millis();
   long timer=start;
   boolean ret=false;
   while(!ret)
   {
     if ((timer+30) < millis()) {
       ret=dmd.stepMarquee(-1,0);
       timer=millis();
     }
   }
   delay(10);
}

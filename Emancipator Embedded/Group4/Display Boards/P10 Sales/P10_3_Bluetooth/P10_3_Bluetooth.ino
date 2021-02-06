
#include <SPI.h>        
#include <DMD.h>        
#include <TimerOne.h>   
#include "Arial_black_16.h"

#define DISPLAYS_ACROSS 3
#define DISPLAYS_DOWN 1
#define max_char 100                       //number max of characters in your message
DMD dmd(DISPLAYS_ACROSS, DISPLAYS_DOWN);

char message[max_char];    // stores you message
char r_char;               // reads each character
byte index = 0;            // defines the position into your array
int i;            
char greeting[] = "";

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
   strcpy(message,greeting);
}

void loop(void)
{
   if(Serial.available())
   {       
        for(i=0; i<99; i++){
            message[i] = '\0';
        }
        index=0;
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
   }

   dmd.selectFont(Arial_Black_16);
   dmd.drawMarquee(message ,max_char,(32*DISPLAYS_ACROSS)-1 ,0);
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
}

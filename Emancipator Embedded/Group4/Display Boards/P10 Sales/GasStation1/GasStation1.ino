/*
 * This program runs for two boards per segment
 * with a total of three segments:
 * PMS 145
 * AGO 185
 * DPK 120
 */
#include <SPI.h> 
#include <DMD.h>
#include <TimerOne.h>
#include <SoftReset.h>
#include <EEPROMAnything.h>
#include "Arial_black_16.h"

#define DISPLAYS_ACROSS 2
#define DISPLAYS_DOWN 3
#define max_char 10
DMD dmd(DISPLAYS_ACROSS, DISPLAYS_DOWN);

char _PMS[max_char];    
char _AGO[max_char];
char _DPK[max_char];
char r_char;
byte index = 0;
int count = 0;
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

   Serial.println("Send anything to enter UPDATE mode");

   EEPROM_readAnything(0, _PMS);
   EEPROM_readAnything(5, _AGO);
   EEPROM_readAnything(10, _DPK);
}

void loop(void)
{
  if(Serial.available())
   {       
        index=0;
        count++;
   }

    while(Serial.available() > 0)
    {
      dmd.clearScreen(true);

      if(count == 1)
      {
          if(index < (max_char-1)) 
       {         
           r_char = Serial.read(); 
           index++; 
       }
      }
      
      if(count == 2)
      {
          if(index < (max_char-1)) 
       {         
           r_char = Serial.read();
           _PMS[index] = r_char; 
           index++; 
       }
      }
      
      if(count == 3)
      {
          if(index < (max_char-1)) 
       {         
           r_char = Serial.read();
           _AGO[index] = r_char;
           index++;
       } 
      }
      
      if(count == 4)
      {
       if(index < (max_char-1)) 
       {         
           r_char = Serial.read();
           _DPK[index] = r_char;
           index++;
       } 
      }
   }

   if(count == 1) {
    Serial.println();
    Serial.println();
    Serial.println("ENTER PMS PRICE");}
   if(count == 2) {
    Serial.println();
    Serial.println();
    Serial.println("ENTER AGO PRICE");}
   if(count == 3) {
    Serial.println();
    Serial.println();
    Serial.println("ENTER DPK PRICE");}
   if(count == 4) {
    for(int k=0;k<101;k++){
      Serial.println(k);
      Serial.print("%");
      delay(50);
    }
    delay(1000);
    Serial.println();
    Serial.println();
    EEPROM_writeAnything(0, _PMS);
    EEPROM_writeAnything(5, _AGO);
    EEPROM_writeAnything(10, _DPK);
    Serial.println("MESSAGE UPDATED!");
    count = 8;
   }
      

   dmd.selectFont(Arial_Black_16);
   dmd.drawString(0,0,"PMS",3,GRAPHICS_NORMAL);
   dmd.drawString(0,16,"AGO",3,GRAPHICS_NORMAL);
   dmd.drawString(0,32,"DPK",3,GRAPHICS_NORMAL);

   dmd.drawString(35,0,_PMS, 3,GRAPHICS_NORMAL);
   dmd.drawString(36,16,_AGO,3,GRAPHICS_NORMAL);
   dmd.drawString(35,32,_DPK,3,GRAPHICS_NORMAL);
   delay(1000);
}

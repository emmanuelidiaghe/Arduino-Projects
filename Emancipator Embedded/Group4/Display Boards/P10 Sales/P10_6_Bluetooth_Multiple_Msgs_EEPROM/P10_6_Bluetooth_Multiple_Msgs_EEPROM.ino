
#include <SPI.h> 
#include <DMD.h>
#include <TimerOne.h>
#include <EEPROMAnything.h>
#include "Arial_black_16.h"

#define DISPLAYS_ACROSS 3
#define DISPLAYS_DOWN 1
#define max_char 100
DMD dmd(DISPLAYS_ACROSS, DISPLAYS_DOWN);

char message1[max_char];    
char message2[max_char];
char message3[max_char];
char message4[max_char];
char message5[max_char];
char message6[max_char];
char message7[max_char];
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

   Serial.println("Send any command to enter UPDATE mode");

   EEPROM_readAnything(0, message1);
   EEPROM_readAnything(72, message2);
   EEPROM_readAnything(82, message3);
   EEPROM_readAnything(92, message4);
   EEPROM_readAnything(102, message5);
   EEPROM_readAnything(112, message6);
   EEPROM_readAnything(183, message7);
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
           message1[index] = r_char; 
           index++; 
       }
      }
      
      if(count == 3)
      {
          if(index < (max_char-1)) 
       {         
           r_char = Serial.read();
           message2[index] = r_char;
           index++;
       } 
      }
      
      if(count == 4)
      {
       if(index < (max_char-1)) 
       {         
           r_char = Serial.read();
           message3[index] = r_char;
           index++;
       } 
      }
      
      if(count == 5)
      {
       if(index < (max_char-1)) 
       {         
           r_char = Serial.read();      
           message4[index] = r_char;
           index++;
       } 
      }
      
      if(count == 6)
      {
       if(index < (max_char-1)) 
       {         
           r_char = Serial.read();
           message5[index] = r_char;
           index++;
       }
      }
      
      if(count == 7)
      {
       if(index < (max_char-1)) 
       {         
           r_char = Serial.read();
           message6[index] = r_char;
           index++;
       }
      }
      
      if(count == 8)
      {
       if(index < (max_char-1)) 
       {         
           r_char = Serial.read();
           message7[index] = r_char;
           index++;
       }
    }
 }

   if(count == 1) {
    Serial.println();
    Serial.println();
    Serial.println("ENTER WELCOME MESSAGE");}
   if(count == 2) {
    Serial.println();
    Serial.println();
    Serial.println("WHAT CAN YOU OFFER? 1");}
   if(count == 3) {
    Serial.println();
    Serial.println();
    Serial.println("WHAT CAN YOU OFFER? 2");}
   if(count == 4) {
    Serial.println();
    Serial.println();
    Serial.println("WHAT CAN YOU OFFER? 3");}
   if(count == 5) {
    Serial.println();
    Serial.println();
    Serial.println("WHAT CAN YOU OFFER? 4");}
   if(count == 6) {
    Serial.println();
    Serial.println();
    Serial.println("WHAT CAN YOU OFFER? 5");}
   if(count == 7) {
    Serial.println();
    Serial.println();
    Serial.println("ENTER THANK-YOU MESSAGE");}
   if(count == 8) {
    for(int k=0;k<101;k++){
      Serial.println(k);
      Serial.print("%");
      delay(50);
    }
    delay(1000);
    Serial.println();
    Serial.println();
   EEPROM_writeAnything(0, message1);
   EEPROM_writeAnything(72, message2);
   EEPROM_writeAnything(82, message3);
   EEPROM_writeAnything(92, message4);
   EEPROM_writeAnything(102, message5);
   EEPROM_writeAnything(112, message6);
   EEPROM_writeAnything(183, message7);
    Serial.println("MESSAGE UPDATED!");
    count = 9;
   }
   
   dmd.clearScreen( true );
      
   delay(500);
   dmd.selectFont(Arial_Black_16);
   dmd.drawMarquee(message1,70,(32*DISPLAYS_ACROSS)-1,0);
   long start=millis();
   long timer=start;
   boolean ret=false;
   while(!ret){
     if ((timer+30) < millis()) {
       ret=dmd.stepMarquee(-1,0);
       timer=millis();
     }
   }

   dmd.clearScreen(true);
   delay(500);

   dmd.selectFont(Arial_Black_16);
   dmd.drawString(0,0,"ANYTHING...",17,GRAPHICS_NORMAL);
   delay(5000);
   dmd.clearScreen(true);

   dmd.selectFont(Arial_Black_16);
   dmd.drawBox(0,0,95,15,GRAPHICS_NORMAL );
   dmd.drawMarquee(message2,9,7,0);
   //dmd.drawString(0,0,"PEPP. SOUP  ",11,GRAPHICS_NORMAL);
   delay(5000);
   long start6=millis();
   long timer6=start6;
   boolean ret6=false;
   while(!ret6){
     if ((timer6+30) < millis()) {
       ret6=dmd.stepMarquee(-1,0);
       timer6=10;
     }
   }

   dmd.clearScreen(true);

   dmd.selectFont(Arial_Black_16);
   dmd.drawBox(0,0,95,15,GRAPHICS_NORMAL );
   dmd.drawMarquee(message3,9,7,0);
   //dmd.drawString(30,1,"HARP    ",15,GRAPHICS_NORMAL);
   delay(5000);
   long start7=millis();
   long timer7=start7;
   boolean ret7=false;
   while(!ret7){
     if ((timer7+30) < millis()) {
       ret7=dmd.stepMarquee(-1,0);
       timer7=10;
     }
   }

   dmd.clearScreen(true);

   dmd.selectFont(Arial_Black_16);
   dmd.drawBox(0,0,95,15,GRAPHICS_NORMAL );
   dmd.drawMarquee(message4,9,7,0);
   //dmd.drawString(20,0,"STOUT   ",15,GRAPHICS_NORMAL);
   delay(5000);
   long start8=millis();
   long timer8=start8;
   boolean ret8=false;
   while(!ret8){
     if ((timer8+30) < millis()) {
       ret8=dmd.stepMarquee(-1,0);
       timer8=10;
     }
   }

   dmd.clearScreen(true);

   dmd.selectFont(Arial_Black_16);
   dmd.drawBox(0,0,95,15,GRAPHICS_NORMAL );
   dmd.drawMarquee(message5,9,7,0);
   //dmd.drawString(15,0,"GULDER  ",8,GRAPHICS_NORMAL);
   delay(5000);
   long start9=millis();
   long timer9=start9;
   boolean ret9=false;
   while(!ret9){
     if ((timer9+30) < millis()) {
       ret9=dmd.stepMarquee(-1,0);
       timer9=10;
     }
   }

   dmd.clearScreen(true);

   dmd.selectFont(Arial_Black_16);
   dmd.drawBox(0,0,95,15,GRAPHICS_NORMAL );
   dmd.drawMarquee(message6,9,7,0);
   //dmd.drawString(10,0,"HEINEKEN  ",10,GRAPHICS_NORMAL);
   delay(5000);
   long start0=millis();
   long timer0=start0;
   boolean ret0=false;
   while(!ret0){
     if ((timer0+30) < millis()) {
       ret0=dmd.stepMarquee(-1,0);
       timer0=10;
     }
   }

   dmd.clearScreen(true);

   dmd.selectFont(Arial_Black_16);
   dmd.drawMarquee(message7,70,(32*DISPLAYS_ACROSS)-1,0);
   long start3=millis();
   long timer3=start3;
   boolean ret3=false;
   while(!ret3){
     if ((timer3+30) < millis()) {
       ret3=dmd.stepMarquee(-1,0);
       timer3=millis();
     }
   }
   delay(100);

   dmd.clearScreen(true);

}

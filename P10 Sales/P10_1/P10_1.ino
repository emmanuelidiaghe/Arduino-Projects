
#include <SPI.h> 
#include <DMD.h>
#include <TimerOne.h>
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
   dmd.clearScreen( true );
   dmd.selectFont(Arial_Black_16);   
   delay(1000);

   dmd.drawMarquee("WELCOME TO CLUB DSTRCT41 ",25,(32*DISPLAYS_ACROSS)-1,0);
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

   dmd.drawMarquee("PLEASE RELAX AND HAVE A GOOD NIGHT WITH US...     ",50,(32*DISPLAYS_ACROSS)-1,0);
   long start1=millis();
   long timer1=start1;
   boolean ret1=false;
   while(!ret1){
     if ((timer1+30) < millis()) {
       ret1=dmd.stepMarquee(-1,0);
       timer1=millis();
     }
   }
   dmd.clearScreen(true);
   delay(500);

   dmd.drawString(0,0,"ANYTHING...",17,GRAPHICS_NORMAL);
   delay(5000);
   dmd.clearScreen(true);

   dmd.drawBox(0,0,95,15,GRAPHICS_NORMAL );
   dmd.drawMarquee("PEPP.SOUP ",9,7,0);
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

   dmd.drawBox(0,0,95,15,GRAPHICS_NORMAL );
   dmd.drawMarquee("HARP ",4,25,0);
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

   dmd.drawBox(0,0,95,15,GRAPHICS_NORMAL );
   dmd.drawMarquee("STOUT ",5,20,0);
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

   dmd.drawBox(0,0,95,15,GRAPHICS_NORMAL );
   dmd.drawMarquee("GULDER ",6,16,0);
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

   dmd.drawBox(0,0,95,15,GRAPHICS_NORMAL );
   dmd.drawMarquee("HENEIKEN ",8,10,0);
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

   dmd.drawMarquee("HOPE YOU ARE HAVING A GOOD TIME HERE...          ",50,(32*DISPLAYS_ACROSS)-1,0);
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

   dmd.drawMarquee("  WE HOPE TO SEE YOU AGAIN...        ",35,(32*DISPLAYS_ACROSS)-1,0);
   long start4=millis();
   long timer4=start4;
   boolean ret4=false;
   while(!ret4){
     if ((timer4+30) < millis()) {
       ret4=dmd.stepMarquee(-1,0);
       timer4=millis();
     }
   }
   delay(100);

   dmd.clearScreen(true);

   dmd.drawMarquee("    THANKS FOR COMING     ",35,(32*DISPLAYS_ACROSS)-1,0);
   long start5=millis();
   long timer5=start5;
   boolean ret5=false;
   while(!ret5){
     if ((timer5+30) < millis()) {
       ret5=dmd.stepMarquee(-1,0);
       timer5=millis();
     }
   }
}


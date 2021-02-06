/*******************
 * RTC_SDA=A4, RTC_SCL=a5
 */
#include <SPI.h>
#include <DMD2.h>
#include <fonts/Droid_Sans_16.h>
#include <fonts/Arial14.h>
#include <RTClib.h>

#define Length 1
#define Width 1

SoftDMD dmd ( Length , Width );
DMD_TextBox box(dmd);  // "box" provides a text box to automatically write to/scroll the display
RTC_DS1307 rtc ;  // The RTC chip used

int Year, Month, Date, Day, Hour, Minute, Second ;
char dmdBuff [ 10 ] ;

 void setup ( ) {
  Serial.begin(9600);
  
  // DMD setup
  dmd.setBrightness ( 255 ) ;
  dmd.begin ( ) ;
  dmd.clearScreen ( ) ;
  
  rtc.begin ( ) ; 
  // Set RTC according to Laptop / PC
  //rtc.adjust ( DateTime ( F ( __DATE__ ) , F ( __TIME__ ) ) ) ;
  // January 21, 2014 at 3am you would call:
  //rtc.adjust (DateTime (2020, 12, 18, 8 ,57, 50));
 }

 void showTime () {
  // Program displays Clock
  dmd.clearScreen ( ) ;
  DateTime now = rtc.now () ;
  Year = now.year ();
  Month = now.month ();
  Date = now.day ();
  Day = now.dayOfTheWeek ();
  Hour = now.hour ();
  Minute = now.minute ();
  Second = now.second ();
  dmd.selectFont ( Droid_Sans_16 ) ;

  if(Hour<10 && Minute >=10) {
  sprintf ( dmdBuff , "%1d" , Hour) ;
  dmd.drawString ( 2 , 1 , dmdBuff ) ;
  sprintf ( dmdBuff , "%2d" , Minute ) ;
  dmd.drawString ( 15 , 1 , dmdBuff ) ;
  dmd.drawString ( 11 , 0 , ":" ) ;
  delay(500);
  dmd.drawString ( 11 , 0 , " " ) ;
  dmd.drawString ( 15 , 1 , dmdBuff ) ;
  delay(500);
  }
  else if(Minute<10 && Hour>=10) {
    sprintf ( dmdBuff , "%2d" , Hour) ;
  dmd.drawString ( 0 , 1 , dmdBuff ) ;
  sprintf ( dmdBuff , "%1d" , Minute ) ;
  dmd.drawString ( 19 , 1 , dmdBuff ) ;
  dmd.drawString ( 17 , 0 , ":" ) ;
  delay(500);
  dmd.drawString ( 17 , 0 , " " ) ;
  dmd.drawString ( 19 , 1 , dmdBuff ) ;
  delay(500);
  }
  else if(Hour<10 && Minute<10) {
    sprintf ( dmdBuff , "%1d" , Hour) ;
  dmd.drawString ( 3 , 1 , dmdBuff ) ;
  sprintf ( dmdBuff , "%1d" , Minute ) ;
  dmd.drawString ( 20 , 1 , dmdBuff ) ;
  dmd.drawString ( 15 , 0 , ":" ) ;
  delay(500);
  dmd.drawString ( 15 , 0 , " " ) ;
  dmd.drawString ( 20 , 1 , dmdBuff ) ;
  delay(500);
  }
  else {
  sprintf ( dmdBuff , "%2d" , Hour) ;
  dmd.drawString ( 0 , 1 , dmdBuff ) ;
  sprintf ( dmdBuff , "%2d" , Minute ) ;
  dmd.drawString ( 17 , 1 , dmdBuff ) ;
  dmd.drawString ( 15 , 0 , ":" ) ;
  delay(500);
  dmd.drawString ( 15 , 0 , " " ) ;
  dmd.drawString ( 17 , 1 , dmdBuff ) ;
  delay(500);
  }
 }

 void loop () 
 {
  showTime();
 }



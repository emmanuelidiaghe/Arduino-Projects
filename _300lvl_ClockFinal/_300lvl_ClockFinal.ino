/*******************
 * RTC_SDA=A4, RTC_SCL=a5
 */
#include <DMD2.h>
#include <fonts/SystemFont5x7.h>
#include <RTClib.h>

#define Length 2
#define Width 1

SoftDMD dmd ( Length , Width ) ;  // Length x WIDTH
 
 RTC_DS1307 rtc ;
 
 int Year, Month, Date, Hour, Minute, Second ;
 
 char dmdBuff [ 10 ] ;

 void setup ( ) {
  
  // DMD setup
  dmd.setBrightness ( 255 ) ;
  dmd.selectFont ( SystemFont5x7 ) ;
  dmd.begin ( ) ;
  dmd.clearScreen ( ) ;
  
  rtc.begin ( ) ;
 
  // Set RTC according to Laptop / PC
  //rtc.adjust ( DateTime ( F ( __DATE__ ) , F ( __TIME__ ) ) ) ;
  // January 21, 2014 at 3am you would call:
   //rtc.adjust (DateTime (2019, 7, 23, 16, 20, 00));
 }

 void showTime () {
  // Program displays Clock
  DateTime now = rtc.now () ;
  Year = now.year ();
  Month = now.month ();
  Date = now.day ();
  Hour = now.hour ();
  Minute = now.minute ();
  Second = now.second ();
 
  sprintf ( dmdBuff , "%2d:%2d:%2d" , Hour , Minute , Second ) ;
  dmd.drawString ( 7 , 0 , dmdBuff ) ;
  sprintf ( dmdBuff , "%2d/%2d/%4d" , Date , Month , Year ) ;
  dmd.drawString ( 2 , 9 , dmdBuff ) ;
  //dmd.marqueeScrollX(1);
 }

 void loop ( ) 
 {
  showTime();
 }



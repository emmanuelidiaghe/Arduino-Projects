/*******************
 * RTC_SDA=A4, RTC_SCL=a5
 */
#include <DMD2.h>
#include <fonts/SystemFont5x7.h>
#include <RTClib.h>
 
 // Deciphering Functions
 #define bOK A0
 #define bUP A1
 #define bDOWN A2
 #define Length 2
 #define Width 1
 // Function Declaration
 
 SoftDMD dmd ( Length , Width ) ;  // Length x WIDTH

 
 RTC_DS1307 rtc ;  // The RTC chip used

 
 // Variable Declaration
 int Year, Month, Date, Hour, Minute, Second ;
 
 char dmdBuff [ 10 ] ;

 void setup ( ) {
  Serial.begin(9600);
  
  // DMD setup
  dmd.setBrightness ( 255 ) ;
  dmd.selectFont ( SystemFont5x7 ) ;
  dmd.begin ( ) ;
  dmd.clearScreen ( ) ;

  
  // Setup RTC
  rtc.begin ( ) ;
  // How to Setting Clock use 2 functions below, manually or automatically singcron PC, then upload the program
  // Once uploaded, check the clock is appropriate, clear the clock setting code below, and re-upload.
 
  // Set RTC according to Laptop / PC
  rtc.adjust ( DateTime ( F ( __DATE__ ) , F ( __TIME__ ) ) ) ;
  // January 21, 2014 at 3am you would call:
  // rtc.adjust (DateTime (2014, 1, 21, 3, 0, 0));
 
  // The Mode pin is used in Input
  pinMode ( bOK , INPUT_PULLUP ) ;
  pinMode ( bUP , INPUT_PULLUP ) ;
  pinMode ( bDOWN , INPUT_PULLUP ) ;
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
  dmd.drawString ( 0 , 0 , dmdBuff ) ;
  sprintf ( dmdBuff , "%2d/%2d/%4d" , Date , Month , Year ) ;
  dmd.drawString ( 2 , 9 , dmdBuff ) ;
 }

 void serialTime()
 {
  DateTime now = rtc.now () ;
  Year = now.year ();
  Month = now.month ();
  Date = now.day ();
  Hour = now.hour ();
  Minute = now.minute ();
  Second = now.second ();
  
  Serial.print(Hour);
  Serial.print(":");
  Serial.print(Minute);
  Serial.print(":");
  Serial.print(Second);
  Serial.println();
  delay(1000);
 }

 void loop ( ) 
 {
  //serialTime();
  showTime();
 }



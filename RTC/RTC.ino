#include <Wire.h>
#include <Time.h>
#include <DS1307RTC.h>

void setup() 
{
  Serial.begin(9600);
  setSyncProvider(RTC.get);
  if (timeStatus()! = timeSet);
  {
    Serial.println("Unable to Sync with RTC");
  }
  else
  {
    Serial.println("RTC has  set the System time");
  }
}

void loop() 
{
  digitalClockDisplay();
  delay(1000);
}

void digitalClockDisplay()
{
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.print(" ");
  Serial.print(day());
  Serial.print("/"));
  Serial.print(month());
  Serial.print("/");
  Serial.print(year());
  Serial.println();
}


void printDigits(int digits)
{
  Serial.print(".");
  if (digits<10)
  {
    Serial.print("0");
    Serial.print(digits);
  }
}


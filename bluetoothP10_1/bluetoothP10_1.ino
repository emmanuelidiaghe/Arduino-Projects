
#include <LiquidCrystal.h>

LiquidCrystal lcd(7, 6, 5, 4, 3, 2);
String mssg;
void setup() {
  lcd.begin(16, 2);
  Serial.begin(9600);
  lcd.clear();
}

void loop() {
  while(Serial.available() == 0)
  {
  
  }
  
  delay(500);
  
  while(Serial.available() > 0)
  {
  mssg = Serial.readString();
  lcd.clear();

  lcd.setCursor(0,0);
  lcd.print(mssg);
  }
}


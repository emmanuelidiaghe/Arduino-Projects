//#include <TimerOne.h>

int pinNum = 2;
void setup() 
{
  while(pinNum < 10)
  {
    pinMode(pinNum, OUTPUT);
    pinNum++;
    delay(100);
  }
  pinNum = 2;
  //Timer1.initialize(100000);
  //Timer1.attachInterrupt( timerIsr ); 
}
 
void loop()
{
  for(pinNum = 2; pinNum < 10; pinNum++)
  {
  digitalWrite(pinNum, digitalRead(pinNum) ^ 1);
  delay(125);
  }
  for(pinNum = 9; pinNum > 1; pinNum--)
  {
  digitalWrite(pinNum, digitalRead(pinNum) ^ 1);
  delay(125);
  }
}


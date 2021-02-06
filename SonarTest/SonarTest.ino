#include<NewPing.h>

#define BUZZER 4
#define TRIGGER 2
#define ECHO 3
#define Max_Distance 200

NewPing DistanceSensor(TRIGGER, ECHO, Max_Distance);

int buzz;
void setup() 
{
  pinMode(BUZZER, OUTPUT);
  digitalWrite(BUZZER, LOW);
  Serial.begin(9600);
}

void loop()
{
  unsigned int cm = DistanceSensor.ping_cm();
  if(cm != 0)
  {
    Serial.print("Distance: ");
    Serial.print(cm);
    Serial.println("cm");
  }
  
  buzz = cm;
  if(buzz<50 && buzz!= 0)
  {
    digitalWrite(BUZZER, HIGH);
  }
  else digitalWrite(BUZZER, LOW);
  
  delay(100);
}

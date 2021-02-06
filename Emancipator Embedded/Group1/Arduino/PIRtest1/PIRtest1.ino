
/*
 * PIR sensor tester
 */

int val      = 0;
int inputPin = 2;
int buzzer   = 3;
 
void setup()
{
  pinMode(buzzer, OUTPUT);
  pinMode(inputPin, INPUT);
 
  Serial.begin(9600);
}
 
void loop()
{
  val = digitalRead(inputPin);

    digitalWrite(buzzer, LOW);
    Serial.println("Motion ended!");
    delay(10);
    
  while (val == HIGH)
  {         
    val = digitalRead(inputPin); 
    digitalWrite(buzzer, HIGH);
    Serial.println("Motion Started");
    delay(10);
  }  
}


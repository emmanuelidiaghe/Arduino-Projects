void setup()
{
  Serial.begin(9600);
  pinMode(2, OUTPUT);
}

void loop()
{
  if(Serial.available()>0)
  {
    char data = Serial.read();
    Serial.println(data);
    if (data == 'U') digitalWrite(2, HIGH);
    
    if (data == 'D') digitalWrite(2, LOW);
  }
  delay(50);
}

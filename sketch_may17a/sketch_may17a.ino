void setup() {
  pinMode(14, OUTPUT);
  digitalWrite(14, HIGH);
  
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
digitalWrite(14, LOW);
delay(1000);
digitalWrite(14, HIGH);
delay(1000);
}

void setup() {
  // put your setup code here, to run once:
pinMode(A0, OUTPUT);
analogWrite(A0, 0);
}

void loop() {
  /*for(int x=0; x<1023; x++){
    analogWrite(A0, x);
    delay(1);
  }*/
  analogWrite(A0, 200);
  // put your main code here, to run repeatedly:
}

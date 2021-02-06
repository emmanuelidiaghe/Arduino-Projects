void setup() {
  pinMode (13,OUTPUT); pinMode (11, OUTPUT);
  digitalWrite (13,0); digitalWrite (11,0);

}

void loop() {
 digitalWrite (13,1);
digitalWrite (11,0);
delay(500);
digitalWrite (13,0);
digitalWrite (11,1);
delay(500); 

}

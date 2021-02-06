int btn1 = 2;
int btn2 = 3;
int btn3 = 4;
int btn4 = 5;
char btn[100];

#define HIGH 5
#define LOW 0


void setup() {
Serial.begin(9600);

pinMode(btn1, INPUT_PULLUP);
pinMode(btn2, INPUT_PULLUP);
pinMode(btn3, INPUT_PULLUP);
pinMode(btn4, INPUT_PULLUP);

}

void loop() {
  int value = digitalRead(2);
  Serial.println("");
 /* Serial.print("Press Button: ");
  if(value==LOW) Serial.print("0");
  else Serial.print("5"); */
  delay(1000);

  sprintf(btn, "%d", value);
  Serial.print(btn);
}

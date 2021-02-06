/***** The front of the robot facing you,Right==MotorA, Left==MotorB*******/

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

MDNSResponder mdns;


const char *ssid = "Emancipator";
const char *password = "emmanuelidiags";

ESP8266WebServer server(80);

int PWMA = 5;
int AIN1 = 4;
int AIN2 = 0;

int STBY = 15;

int PWMB = 14;
int BIN1 = 12;
int BIN2 = 13;

String webPage = "";


void StandBy()
{
  digitalWrite(STBY, HIGH);
}

void DisEngage()
{
  digitalWrite(STBY, LOW);
}

void MotorForward()
{
  analogWrite(PWMA, 255);
  analogWrite(PWMB, 255);
  
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, HIGH);
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, HIGH);
}

void MotorBackward()
{
  analogWrite(PWMA, 600);
  analogWrite(PWMB, 600);
  
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);
}

void MotorLeftward()
{
  analogWrite(PWMA, 300);
  analogWrite(PWMB, 300);
  
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, HIGH);
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, HIGH);
}

void MotorRightward()
{
  analogWrite(PWMA, 300);
  analogWrite(PWMB, 300);
  
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, HIGH);
  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, HIGH);
}

void Stop()
{
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, LOW);
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, LOW);
}

void Gear_1()
{
  analogWrite(PWMA, 350);
  analogWrite(PWMB, 350);
  
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, HIGH);
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, HIGH);
}

void Gear_2()
{
  //60% of 1023
  analogWrite(PWMA, 550);
  analogWrite(PWMB, 550);
  
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, HIGH);
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, HIGH);
}

void Gear_3()
{
  //80% of 1023
  analogWrite(PWMA, 800);
  analogWrite(PWMB, 800);
  
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, HIGH);
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, HIGH);
}

void Gear_4()
{
  analogWrite(PWMA, 1023);
  analogWrite(PWMB, 1023);
  
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, HIGH);
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, HIGH);
}

void setup() 
{
  webPage += "<body style=background-color:pink><h1 style=color:blue;font-size:500%;>EMANCIPATOR ROBOT</h1> <p style=color:black;font-size:150%;><i>As the need for wireless control systems increases in man's quest for sophistication, the need to also solve problems brought about by this sophistication increases. This has led to the invention of several automated devices, with robotic cars being no exception, due to its numerous advantages. </i></p>";
  webPage += "<p style=color:black;font-size:150%;><i> The EMANCIPATOR_BOT is a WiFi-controlled robotic system and can be controlled using and android phone connected to the Wi-Fi module(ESP8266) on the robot</i></p><br>";
  webPage += "<p style=color:black;font-size:150%;><i> For the records, this ROBOT was designed by IDIAGHE EMMANUEL EMANCIPATOR on this day: 26/07/17, 11:02 PM. </i></p><br>";
 
  pinMode(PWMA, OUTPUT);
  analogWrite(PWMA, 0);
  
  pinMode(AIN1, OUTPUT);
  digitalWrite(AIN1, LOW);
  
  pinMode(AIN2, OUTPUT);
  digitalWrite(AIN2, LOW);
  
  pinMode(PWMB, OUTPUT);
  analogWrite(PWMB, 0);
  
  pinMode(BIN1, OUTPUT);
  digitalWrite(BIN1, LOW);
  
  pinMode(BIN2, OUTPUT);
  digitalWrite(BIN2, LOW);
  
  pinMode(STBY, OUTPUT);
  digitalWrite(STBY, LOW);


  delay(1000);
  Serial.begin(9600);
  Serial.println();
  Serial.print("Configuring access point...");
  /* You can remove the password parameter if you want the AP to be open. */
  WiFi.softAP(ssid, password);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  if (mdns.begin("esp8266", WiFi.localIP())) {
    Serial.println("MDNS responder started");
     Serial.println(WiFi.localIP());
  }

  server.on("/", []() 
  {
    server.send(200, "text/html", webPage);
  });
  server.on("/backward", [](){
    server.send(200, "text/html", webPage);
    MotorBackward();
    Serial.println("BACKWARD");
    delay(100); 
  });
  server.on("/forward", []() {
    server.send(200, "text/html", webPage);
    MotorForward();
    Serial.println("FORWARD");
    delay(100);
  });
  server.on("/right", []() {
    server.send(200, "text/html", webPage);
    MotorRightward();
    Serial.println("RIGHTWARD");
    delay(100);
  });
  server.on("/left", []() {
    server.send(200, "text/html", webPage);
    MotorLeftward();
    Serial.println("LEFTWARD");
    delay(100);
  });
   server.on("/standby", []() {
    server.send(200, "text/html", webPage);
    StandBy();
    Serial.println("STANDBY");
    delay(100);
  });
   server.on("/disengage", []() {
    server.send(200, "text/html", webPage);
    DisEngage();
    Serial.println("DISENGAGED");
    delay(100);
  });
  server.on("/gear1", []() {
    server.send(200, "text/html", webPage);
    Gear_1();
    Serial.println("Gear 2");
    delay(100);
  });
  server.on("/gear2", []() {
    server.send(200, "text/html", webPage);
    Gear_2 ();
    Serial.println("Gear 2");
    delay(100);
  });
  server.on("/gear3", []() {
    server.send(200, "text/html", webPage);
    Gear_3();
    Serial.println("Gear 3");
    delay(100);
  });
  server.on("/gear4", []() {
    server.send(200, "text/html", webPage);
    Gear_4();
    Serial.println("Gear 4");
    delay(100);
  });
  server.on("/stop", []() {
    server.send(200, "text/html", webPage);
    Stop();
    delay(100);
  });

  
  server.begin();
  Serial.println("HTTP server started");
}


void loop() 
{
  server.handleClient();
}

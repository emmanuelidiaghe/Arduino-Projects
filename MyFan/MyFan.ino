
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

MDNSResponder mdns;


/* Set these to your desired credentials. */
const char *ssid = "Emmanuel";
const char *password = "emmanuelidiags";

ESP8266WebServer server(80);


String webPage = "";
String feedback = "";
String currentPage = "";

int speed1 = 12;
int speed2 = 13;
int speed3 = 15;


void setup() {

  webPage += "<body style=background-color:pink><h1 style=color:blue;font-size:500%;>EMANCIPATOR WIFI</h1> <p style=color:black;font-size:150%;><i> This is a simple wifi-controlled system which is the basis of most Home Automation Systems. Its major component is the wifi module,which in this case, is the ESP8266.</i></p>";
  webPage += "<p style=color:black;font-size:150%;><i> For the records, this was designed by IDIAGHE EMMANUEL EMANCIPATOR on this day: 22/05/18. </i></p>";
  webPage += "<p style=color:green;font-size:300%;><b>Speed 1...<br><a href=\"1on\"><button style=color:white;font-size:250%;height:150px;width:350px;background-color:blue;><b>ON</button></a>&nbsp<br>";
  webPage += "<p style=color:green;font-size:300%;><b>Speed 2...<br><a href=\"2on\"><button style=color:white;font-size:250%;height:150px;width:350px;background-color:blue;><b>ON</button></a>&nbsp;<br>";
  webPage += "<p style=color:green;font-size:300%;><b>Speed 3...<br><a href=\"3on\"><button style=color:white;font-size:250%;height:150px;width:350px;background-color:blue;><b>ON</button></a>&nbsp;";
  webPage += "<p style=color:green;font-size:300%;><b>    OFF...<br><a href=\"off\"><button style=color:white;font-size:250%;height:150px;width:350px;background-color:blue;><b>OFF</button></a>&nbsp;";
  
  pinMode(speed1, OUTPUT);
  digitalWrite(speed1, HIGH);
  pinMode(speed2, OUTPUT);
  digitalWrite(speed2, HIGH);
  pinMode(speed3, OUTPUT);
  digitalWrite(speed3, HIGH);


  delay(1000);
  Serial.begin(115200);
  Serial.println();
  Serial.print("Configuring access point...");
  /* You can remove the password parameter if you want the AP to be open. */
  WiFi.begin(ssid, password);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  if (mdns.begin("esp8266", WiFi.localIP())) {
    Serial.println("MDNS responder started");
     Serial.println(WiFi.localIP());
  }

  server.on("/", []() {
    feedback = "<p><b>Waiting for user_input...Please click a button</b></p>";
    currentPage = webPage + feedback;
    server.send(200, "text/html", currentPage);
  });
  server.on("/1on", []() {
    feedback = "<p><b>Speed 1 is ON</b></p>";
    currentPage = webPage + feedback;
    server.send(200, "text/html", currentPage);
    digitalWrite(speed1, LOW);
    digitalWrite(speed2, HIGH);
    digitalWrite(speed3, HIGH);
    Serial.println("Speed 1 is on");
    delay(1000);
  });
  server.on("/2on", []() {
    feedback = "<p><b>Speed 2 is ON</b></p>";
    currentPage = webPage + feedback;
    server.send(200, "text/html", currentPage);
    digitalWrite(speed1, HIGH);
    digitalWrite(speed2, LOW);
    digitalWrite(speed3, HIGH);
     Serial.println("Speed 2 is on");
    delay(1000);
  });
  server.on("/3on", []() {
    feedback = "<p><b>Speed 3 is ON</b></p>";
    currentPage = webPage + feedback;
    server.send(200, "text/html", currentPage);
    digitalWrite(speed1, HIGH);
    digitalWrite(speed2, HIGH);
    digitalWrite(speed3, LOW);
     Serial.println("Speed 3 is on");
    delay(1000);
  });
  server.on("/off", []() {
    feedback = "<p><b>Fan is OFF</b></p>";
    currentPage = webPage + feedback;
    server.send(200, "text/html", currentPage);
    digitalWrite(speed1, HIGH);
    digitalWrite(speed2, HIGH);
    digitalWrite(speed3, HIGH);
     Serial.println("Fan is off");
    delay(1000);
  });
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}

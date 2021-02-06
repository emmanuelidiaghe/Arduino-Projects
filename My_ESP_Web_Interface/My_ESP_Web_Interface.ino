
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

int gpio0_pin = 0;
int gpio2_pin = 2;


void setup() {

  webPage += "<body style=background-color:pink><h1 style=color:blue;font-size:500%;>EMANCIPATOR WIFI</h1> <p style=color:black;font-size:150%;><i> This is a simple wifi-controlled system which is the basis of most Home Automation Systems. Its major component is the wifi module,which in this case, is the ESP8266.</i></p>";
  webPage += "<p style=color:black;font-size:150%;><i> For the records, this was designed by IDIAGHE EMMANUEL EMANCIPATOR on this day: 30/06/17. </i></p><br>";
  webPage += "<p style=color:green;font-size:300%;><b>GPIO 1.....<br><a href=\"socket1On\"><button style=color:white;font-size:300%;height:200px;width:400px;background-color:blue;><b>ON</button></a>&nbsp;<a href=\"socket1Off\"><button style=color:black;font-size:300%;height:200px;width:400px;background-color:red><b>OFF</button></a></p><br><br>";
  webPage += "<p style=color:green;font-size:300%;><b>GPIO 2.....<br><a href=\"socket2On\"><button style=color:white;font-size:300%;height:200px;width:400px;background-color:blue;><b>ON</button></a>&nbsp;<a href=\"socket2Off\"><button style=color:black;font-size:300%;height:200px;width:400px;background-color:red><b>OFF</button></a></p><br><hr></body>";
  
  pinMode(gpio0_pin, OUTPUT);
  digitalWrite(gpio0_pin, LOW);
  pinMode(gpio2_pin, OUTPUT);
  digitalWrite(gpio2_pin, LOW);


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
  server.on("/socket1On", []() {
    feedback = "<p><b>GPIO 1 is ON</b></p>";
    currentPage = webPage + feedback;
    server.send(200, "text/html", currentPage);
    digitalWrite(gpio0_pin, HIGH);
    Serial.println("GPIO 1 is on");
    delay(1000);
  });
  server.on("/socket1Off", []() {
    feedback = "<p><b>GPIO 1 is OFF</b></p>";
    currentPage = webPage + feedback;
    server.send(200, "text/html", currentPage);
    digitalWrite(gpio0_pin, LOW);
     Serial.println("GPIO 1 is off");
    delay(1000); 
  });
  server.on("/socket2On", []() {
    feedback = "<p><b>GPIO 2 is ON</b></p>";
    currentPage = webPage + feedback;
    server.send(200, "text/html", currentPage);
    digitalWrite(gpio2_pin, HIGH);
     Serial.println("GPIO 2 is on");
    delay(1000);
  });
  server.on("/socket2Off", []() {
    feedback = "<p><b>GPIO 2 is OFF</b></p>";
    currentPage = webPage + feedback;
    server.send(200, "text/html", currentPage);
    digitalWrite(gpio2_pin, LOW);
     Serial.println("GPIO 2 is off");
    delay(1000);
  });
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}

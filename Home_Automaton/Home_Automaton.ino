 /************
  * The ESP8266 is connected to power loads through mechancical relays
  * BC547 is used for switching the ouput of the ESP-01
  */
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

MDNSResponder mdns;

const char* ssid = "babe";  //Enter you Wifi SSID here
const char* password = "googlechrome"; //Enter your password here

ESP8266WebServer server(80);

String mainPage = ""; //The default page
String feedback = ""; //Gives staus of the switch
String currentPage = ""; //Combines main and feedback page

int Pin1 = 13;
int Pin2 = 12;
int Pin3 = 14;

void setup(void){
  mainPage += "<h1 align=\"center\">Smart Junction Box</h1><h2 align=\"center\">by CircuitDigest</h2><h1 align=\"center\"><p>Switch 1 <a href=\"switch1On\"><button>ON</button></a>&nbsp;<a href=\"switch1Off\"><button>OFF</button>  </a></p>";
  mainPage += "<p>Switch 2 <a href=\"switch2On\"><button>ON</button></a>&nbsp;<a href=\"switch2Off\"><button>OFF</button>  </a></p>";
  feedback = "<h3 align=\"center\">Both Switch 1 and switch 2 are OFF</h3>";

  currentPage = mainPage+feedback;
  // preparing GPIOs
  pinMode(Pin1, OUTPUT);
  digitalWrite(Pin1, HIGH);
  pinMode(Pin2, OUTPUT);
  digitalWrite(Pin2, HIGH);
  pinMode(Pin3, OUTPUT);
  digitalWrite(Pin3, HIGH);
  
  delay(1000);
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  if (mdns.begin("esp8266", WiFi.localIP())) {
    Serial.println("MDNS responder started");
  }
  
  server.on("/", [](){
    currentPage = mainPage+feedback;
    server.send(200, "text/html", currentPage);
    currentPage = "";
  });

  
  server.on("/switch1On", [](){
    feedback = "<h3 align=\"center\">Switch 1 turned ON</h3>";
    currentPage=mainPage+feedback;
    server.send(200, "text/html", currentPage);
    currentPage="";
    digitalWrite(Pin1, LOW);
    digitalWrite(Pin2, HIGH);
    digitalWrite(Pin3, HIGH);
    delay(1000);
  });
  server.on("/switch2On", [](){
    feedback = "<h3 align=\"center\">Switch 2 turned ON</h3>";
    currentPage=mainPage+feedback;
    server.send(200, "text/html", currentPage);
    currentPage="";
    digitalWrite(Pin2, LOW);
    digitalWrite(Pin1, HIGH);
    digitalWrite(Pin3, HIGH);
    delay(1000);
  });
  server.on("/switch3On", [](){
    feedback = "<h3 align=\"center\">Switch 3 turned ON</h3>";
    currentPage=mainPage+feedback;
    server.send(200, "text/html", currentPage);
    currentPage="";
    digitalWrite(Pin3, LOW);
    digitalWrite(Pin2, HIGH);
    digitalWrite(Pin1, HIGH);
    delay(1000); 
  });
  server.on("/Off", [](){
    feedback = "<h3 align=\"center\">Switches turned OFF</h3>";
    currentPage=mainPage+feedback;
    server.send(200, "text/html", currentPage);
    currentPage="";
    digitalWrite(Pin3, HIGH);
    digitalWrite(Pin2, HIGH);
    digitalWrite(Pin1, HIGH);
    delay(1000); 
  });

  server.begin();
  Serial.println("Smart Junction Box is up and running");
}
 
void loop(void){
  server.handleClient();
}

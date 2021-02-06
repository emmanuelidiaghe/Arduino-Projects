 /*******************
  * The IP here can ge accessed globally
  */
  
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

MDNSResponder mdns;

const char* ssid = "Emancipator";  //Enter you Wifi SSID here
const char* password = "emmanuelidiags"; //Enter your password here

ESP8266WebServer server(80);

String mainPage = ""; //The default page
String feedback = ""; //Gives staus of the switch
String currentPage = ""; //Combines main and feedback page

int GPIO_0 = 0; //Pin defanition
int GPIO_2 = 2; //Pin defanition

void setup(void){
  mainPage += "<h1 align=\"center\">Aquarium controller</h1><h2 align=\"center\">powered by CircuitDigest</h2><h1 align=\"center\"><p>Light: <a href=\"switch1On\"><button>ON</button></a>&nbsp;<a href=\"switch1Off\"><button>OFF</button>  </a></p>";
  mainPage += "<p>Filter: <a href=\"switch2On\"><button>ON</button></a>&nbsp;<a href=\"switch2Off\"><button>OFF</button>  </a></p>";
  feedback = "<h3 align=\"center\">Both Light and Filter is down</h3>";

  currentPage = mainPage+feedback;
  // preparing GPIOs
  pinMode(GPIO_0, OUTPUT);
  digitalWrite(GPIO_0, LOW);
  pinMode(GPIO_2, OUTPUT);
  digitalWrite(GPIO_2, LOW);
  
  delay(1000);
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Get a constant IP everytime
IPAddress ip(192, 168, 0, 104); // This should be the IP of my ESP module
IPAddress gateway(192, 168, 0, 1); // set gateway to match your network most gateway will be the same by default
Serial.print(F("Setting static ip to : "));
Serial.println(ip);
IPAddress subnet(255, 255, 255, 0); // set subnet mask to match your network
WiFi.config(ip, gateway, subnet);

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
    feedback = "<h3 align=\"center\">Light is UP ON</h3>";
    currentPage=mainPage+feedback;
    server.send(200, "text/html", currentPage);
    currentPage="";
    digitalWrite(GPIO_0, HIGH);
    delay(1000);
  });
  server.on("/switch1Off", [](){
    feedback = "<h3 align=\"center\">Light is DOWN</h3>";
    currentPage=mainPage+feedback;
    server.send(200, "text/html", currentPage);
    currentPage="";
    digitalWrite(GPIO_0, LOW);
    delay(1000); 
  }); 

  
  server.on("/switch2On", [](){
    feedback = "<h3 align=\"center\">Filter is UP</h3>";
    currentPage=mainPage+feedback;
    server.send(200, "text/html", currentPage);
    currentPage="";
    digitalWrite(GPIO_2, HIGH);
    delay(1000);
  });
  server.on("/switch2Off", [](){
    feedback = "<h3 align=\"center\">Filter is DOWN</h3>";
    currentPage=mainPage+feedback;
    server.send(200, "text/html", currentPage);
    currentPage="";
    digitalWrite(GPIO_2, LOW);
    delay(1000); 
  });

  server.begin();
  Serial.println("Smart Junction Box is up and running");
}
 
void loop(void){
  server.handleClient();
} 

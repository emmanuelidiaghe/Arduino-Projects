/*
 * 
 * 
 */
const char *ssid = "jcool";
const char *password = "johnesan";

String webPage = "";

#define PinNum 2
#define GPIO1  0
#define GPIO2  1
//#define GPIO3  2  
//#define GPIO4  3

#define usingArduino
#define usingHomeAutomation

#include "C://Emancipator/Emancipator.h"

Home automation;

void setup() 
{
  webPage += "<body style=background-color:pink><h1 style=color:blue;font-size:500%;>EMANCIPATOR WIFI</h1> <p style=color:black;font-size:150%;><i> This is a simple wifi-controlled system which is the basis of most Home Automation Systems. Its major component is the wifi module,which in this case, is the ESP8266.</i></p>";
  webPage += "<p style=color:black;font-size:150%;><i> For the records, this was designed by IDIAGHE EMMANUEL EMANCIPATOR on this day: 30/06/17. </i></p><br>";
  webPage += "<p style=color:green;font-size:300%;><b>GPIO 1.....<br><a href=\"socket1On\"><button style=color:white;font-size:300%;height:200px;width:400px;background-color:blue;><b>ON</button></a>&nbsp;<a href=\"socket1Off\"><button style=color:black;font-size:300%;height:200px;width:400px;background-color:red><b>OFF</button></a></p><br><br>";
  webPage += "<p style=color:green;font-size:300%;><b>GPIO 2.....<br><a href=\"socket2On\"><button style=color:white;font-size:300%;height:200px;width:400px;background-color:blue;><b>ON</button></a>&nbsp;<a href=\"socket2Off\"><button style=color:black;font-size:300%;height:200px;width:400px;background-color:red><b>OFF</button></a></p><br><hr></body>";

  Home_Init();

  automation.init();

  automation.IPconfigure();

  automation.HTML();

  automation.socket1On();

  automation.socket1Off();

  automation.socket2On();

  automation.socket2Off();

  //automation.socket3On();

  //automation.socket3Off();

  //automation.socket4On();

  //automation.socket4Off(); 
}

void loop() 
{
  automation.run();
}

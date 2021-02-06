/*
 * 
 *
 */
const char *ssid = "Emancipator";
const char *password = "emmanuelidiags";
String webPage = "";

#define PWMA  5
#define AIN1  4
#define AIN2  0
#define STBY  15
#define PWMB  14
#define BIN1  12
#define BIN2  13

#define FWDspeed  255
#define BCKspeed  600
#define LFTspeed  300
#define RHTspeed  300

#define SpeedG1   350
#define SpeedG2   550
#define SpeedG3   800
#define SpeedG4   1023

#define usingEmancipatorBot
#define usingArduino

#include "c://Emancipator/Emancipator.h"

Connect net;

void setup() 
{
  webPage += "<body style=background-color:pink><h1 style=color:blue;font-size:400%;>EMANCIPATOR ROBOT</h1> <p style=color:black;font-size:150%;><i>As the need for wireless control systems increases in man's quest for sophistication, the need to also solve problems brought about by this sophistication increases. This has led to the invention of several automated devices, with robotic cars being no exception, due to its numerous advantages. </i></p>";
  webPage += "<p style=color:black;font-size:150%;><i> The <b>EMANCIPATOR_BOT</b> is a WiFi-controlled robotic system and can be controlled using and android phone connected to the Wi-Fi module(ESP8266) on the robot</i></p>";
  webPage += "<p style=color:black;font-size:80%;><b> For the records, this ROBOT was designed by IDIAGHE EMMANUEL EMANCIPATOR on this day: 26/07/17, 11:02 PM. </b></p>";
 
  Robot_Init();
  
  net.init();

  net.IPconfigure();

  net.driveHTML();

  net.driveStandBy();

  net.driveBackward();

  net.driveForward();

  net.driveLeftward();

  net.driveRightward();

  net.driveGear1();

  net.driveGear2();

  net.driveGear3();

  net.driveGear4();

  net.driveDisengage();

  net.driveStop();
}


void loop() 
{
  net.run();
}


ODE: SELECT ALL
/*---------------------------------------------------
                 _____
                </\|/\>
          _-_-_-_ *|* _-_-_-_
         -_-_-_-_-\@/-_-_-_-_-
         

__________________________________________________________________________________________________
......Use 5v to 3v Level Shifters as 5V devices like PIR and HC-SR04 can damage your device.......
--------------------------------------------------------------------------------------------------
 ___/'¬_         /\
{_-_ ^  \_______//
     /           \
     \ _  _____  /
     || ||     |||
     || ||     |||  
---------------------------------------------------*/
#include <ESP8266WiFi.h>
#include "FS.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to YOUR LCD ADDRESS !!!
// WiFi connection
const char* ssid = "YourSSID";
const char* password = "YourPassword";
String emailbutton = "off";
String lastdata;
String DsTable;             //Discarded parts if the Strings......
String tmtxt1;              //Strings from the Web....
String tmtxt2;
String stheDate;            //System Start Time...
String theDate;             // The current TimeStamp from Google....
boolean got_text = false;   //Print the Text to LCD if true......
unsigned long ulReqcount;                            // how often has a valid page been requested
unsigned long ulReconncount;                         // how often did we connect to WiFi
unsigned long Dfsize;                                // size of the datalog file

ADC_MODE(ADC_VCC);

// needed to avoid link error on ram check
extern "C" 
{
#include "user_interface.h"
}
// Create an instance of the server on Port 80-89 = Sensor 1 - 10
WiFiServer server(80);
WiFiClient client;

void wsysreset()
{
  //Perform challenge to user before getting here to reboot !!!
  //Make an attempt to send mail or other backup of log files then reboot
    ESP.restart();
}
/////////////////////
// the setup routine
/////////////////////
void setup() 
{
  Wire.begin();

  // setup globals
  pinMode(0, INPUT);  //Use the Flash Button.........to send email etc.......
  // initialize the lcd 
  lcd.init();
  lcd.backlight();
  lcd.print("SensorInit Begin");  
  delay(1000);
  // initialize the SPIFFS
  if (!SPIFFS.begin()) {
       lcd.print("SPIFFSinitFailed");
       while(1);
       }
  // clear any old data....
     if (!SPIFFS.format()) {
      lcd.print("format failed");
      while(1);      
    }
  lcd.print("Sensor Init Done"); 
  delay(250);  
  // inital connect
  WiFi.mode(WIFI_STA);
  delay(1000);
    // Connect to WiFi network
  lcd.clear();
  lcd.print("Connecting to ");
  lcd.setCursor(0, 1);
  lcd.print(ssid);
  delay(1000);  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    lcd.print(".");
  }
  server.begin();
 delay(500); 
}


///////////////////
// (re-)start WiFi
///////////////////
void WiFiStart()
{
  // Connect to WiFi network
  lcd.clear();
  lcd.print("Connecting to ");
  lcd.setCursor(0, 1);
  lcd.print(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    lcd.print(".");
  }
  lcd.clear();
  lcd.print("WiFi connected");
  
  // Start the server
  lcd.setCursor(0, 1);
  server.begin();
  lcd.print("Server started");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Network Started ");
  // Print the IP address       
  lcd.setCursor(0, 1);
  lcd.print("IP= ");  
  lcd.print(WiFi.localIP());
  lcd.clear();
  lcd.print("Monitor");
  lcd.setCursor(0,1);
  lcd.print(theDate);
}




//////////////////////////
// create HTTP 1.1 header
//////////////////////////
String MakeHTTPHeader(unsigned long ulLength)
{
  String sHeader;
  
  sHeader  = F("HTTP/1.1 200 OK\r\nContent-Length: ");
  sHeader += ulLength;
  sHeader += F("\r\nContent-Type: text/html\r\nConnection: close\r\n\r\n");
  
  return(sHeader);
}


////////////////////
// make html footer
////////////////////
String MakeHTTPFooter()
{
  String sResponse;
  
  sResponse  = F(" "); 
  sResponse += F("<BR>environmental.monitor.log@gmail.com<BR><FONT SIZE=-2>ESP8266 With LCD<BR><FONT SIZE=-2>Compiled Using ver. 1.6.5-1160-gef26c5f, built on Sep 30, 2015</body></html>");
  return(sResponse);
}

/////////////
// main loop
/////////////
void loop() 
{
  //////////////////////////////
  // check if WLAN is connected
  //////////////////////////////
  
  if (WiFi.status() != WL_CONNECTED)
  {
    WiFiStart();
  }
  
  ///////////////////////////////////
  // Check if a client has connected
  ///////////////////////////////////
  WiFiClient client = server.available();
  if (!client) 
  {
    return;
  }
  
  // Wait until the client sends some data
  unsigned long ultimeout = millis()+250;
  while(!client.available() && (millis()<ultimeout) )
  {
    delay(1);
  }
  if(millis()>ultimeout) 
  { 
    return; 
  }
  
  /////////////////////////////////////
  // Read the first line of the request
  /////////////////////////////////////
  String sRequest = client.readStringUntil('\r');
  client.flush();
  
  // stop client, if request is empty
  if(sRequest=="")
  {
    client.stop();
    return;
  }
  
  // get path; end of path is either space or ?
  // Syntax is e.g. GET /?show=1234 HTTP/1.1
  String sPath="",sParam="", sCmd="";
  String sGetstart="GET ";
  int iStart,iEndSpace,iEndQuest;
  iStart = sRequest.indexOf(sGetstart);
  if (iStart>=0)
  {
    iStart+=+sGetstart.length();
    iEndSpace = sRequest.indexOf(" ",iStart);
    iEndQuest = sRequest.indexOf("?",iStart);
    
    // are there parameters?
    if(iEndSpace>0)
    {
      if(iEndQuest>0)
      {
        // there are parameters
        sPath  = sRequest.substring(iStart,iEndQuest);
        sParam = sRequest.substring(iEndQuest,iEndSpace);
      }
      else
      {
        // NO parameters
        sPath  = sRequest.substring(iStart,iEndSpace);
      }
    }
  }
 
  
  ///////////////////////////
  // format the html response
  ///////////////////////////
  String sResponse,sResponse2,sHeader;
  
  /////////////////////////////////
  //  Update LCD with text from Web
  /////////////////////////////////
  //_________________________________________________________________
  // /ajax_inputs&L1=123456789&L2=abcdefghi&nocache=968337.7823963541
  //  1-9 on Line 1 and a - i on line 2
  //¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬
if (sPath.startsWith("/ajax_inputs&L1="))
   {
     ulReqcount++;
     tmtxt1="";
     tmtxt2="";
    got_text = true;                    // print the received text to the LCD if found
File in = SPIFFS.open("/temp.txt", "w");      
  if (in) { 
    in.print(sPath);
    in.close();
      }
    else {
      lcd.clear();
      lcd.print("rOr Temp.TXT");
      while(1);
    }
     in = SPIFFS.open("/temp.txt", "r"); 
     in.setTimeout(0);
String Dtmtxt = in.readStringUntil('=');
       tmtxt1 += in.readStringUntil('&');
       Dtmtxt = in.readStringUntil('=');
       tmtxt2 += in.readStringUntil('&');
     in.close();
       tmtxt1.replace("%20", " ");
       tmtxt2.replace("%20", " ");
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(tmtxt1);
        lcd.setCursor(0, 1);
        lcd.print(tmtxt2);
   }
  //////////////////////////////////
  // format the html page for gauges
  //////////////////////////////////
 if(sPath=="/")
  {
    ulReqcount++;
    sResponse  = F("<html>\n<head>\n<title>Web Monitor Text to LCD</title>\n<script>\n\n");    
    sResponse += F("strLine1 = \"\";\nstrLine2 = \"\";\n\nfunction SendText()\n{\nnocache = \"&nocache=\" + Math.random() * 1000000;\nvar request = new XMLHttpRequest();\n\nstrLine1 = \"&L1=\" + document.getElementById(\"txt_form\").line1LCD.value;\nstrLine2 = \"&L2=\" + document.getElementById(\"txt_form\").line2LCD.value;\n\n");
    sResponse += F("request.open(\"GET\", \"ajax_inputs\" + strLine1 + strLine2 + nocache, true);\nrequest.send(null);\n}\n</script>\n<body>\n<h1>Web Monitor<BR>LCD Text Input</h1>\n");
    sResponse += F("<BR><a href=\"/\">Text Input Page</a><BR><a href=\"/diag\">Diagnostics Information Page</a><BR>\n");  
    sResponse += F("<body onload=\"GetESP8266IO()\">\n<h1><FONT SIZE=-1>Enter text to send to ESP8266 LCD:</h1>\n<form id=\"txt_form\" name=\"frmText\">\n<label>Line 1: <input type=\"text\" name=\"line1LCD\" size=\"16\" maxlength=\"16\" /></label><br /><br />\n<label>Line 2: <input type=\"text\" name=\"line2LCD\" size=\"16\" maxlength=\"16\" /></label>\n");
    sResponse += F("</form>\n<br />\n<input type=\"submit\" value=\"Send Text\" onclick=\"SendText()\" />\n\n");    
    sResponse2 = F("<font color=\"#c0ff84\"><body bgcolor=\"#c384ff\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=yes\"><BR>LCD Text Input<BR>\n");
    sResponse2 += F("</div>\n<div style=\"clear:both;\"></div><p>");
    sResponse2 += MakeHTTPFooter().c_str();
        // Send the response to the client 
    client.print(MakeHTTPHeader(sResponse.length()+sResponse2.length()).c_str());
    client.print(sResponse);
    client.print(sResponse2);
}
  else if(sPath=="/yfireset")
  {
    ulReqcount++;
    
                                client.println("HTTP/1.1 200 OK"); 
                                client.println("Content-Type: text/html");
                                client.println("Connection: close");
                                client.println();
                                client.println("<!DOCTYPE HTML>");
                                client.print("<html><head><title>Web Monitor</title></head><body>");
                                client.print("<font color=\"#c0ff84\"><body bgcolor=\"#c384ff\">");
                                client.print("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=yes\">");
                                client.print("<h1>Web Monitor<BR>WiFi Reset Page </h1>");
                                client.print("<BR><a href=\"/\">Text Input Page</a><BR><BR><BR><BR>Restarted WiFiConnections = ");
                                client.print(ulReconncount);
                                client.print("<BR><FONT SIZE=-2>environmental.monitor.log@gmail.com<BR><FONT SIZE=-2>ESP8266 With LCD<BR>");
                                client.print("<FONT SIZE=-2>Compiled Using ver. 1.6.5-1160-gef26c5f, built on Sep 30, 2015<BR>");
                                client.println("<IMG SRC=\"https://raw.githubusercontent.com/genguskahn/ESP8266-For-DUMMIES/master/SoC/DimmerDocs/organicw.gif\" WIDTH=\"250\" HEIGHT=\"151\" BORDER=\"1\"></body></html>");

  WiFiStart();
  }
  else if(sPath=="/diag")
  {   
     float   servolt1 = ESP.getVcc();
     long int spdcount = ESP.getCycleCount();
     delay(1);
     long int spdcount1 = ESP.getCycleCount();
     long int speedcnt = spdcount1-spdcount; 
     FlashMode_t ideMode = ESP.getFlashChipMode();
     ulReqcount++;
File logF = SPIFFS.open("/humidlog.CSV", "r");      
     Dfsize = logF.size();
                                String duration1 = " ";
                                int hr,mn,st;
                                st = millis() / 1000;
                                mn = st / 60;
                                hr = st / 3600;
                                st = st - mn * 60;
                                mn = mn - hr * 60;
                                if (hr<10) {duration1 += ("0");}
                                duration1 += (hr);
                                duration1 += (":");
                                if (mn<10) {duration1 += ("0");}
                                duration1 += (mn);
                                duration1 += (":");
                                if (st<10) {duration1 += ("0");}
                                duration1 += (st);     
                                client.println("HTTP/1.1 200 OK"); 
                                client.println("Content-Type: text/html");
                                client.println("Connection: close");
                                client.println();
                                client.println("<!DOCTYPE HTML>");
                                client.print("<html><head><title>Web Monitor</title></head><body>");
                                client.print("<font color=\"#c0ff84\"><body bgcolor=\"#c384ff\">");
                                client.print("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=yes\">");
                                client.print("<h1>Web Monitor<BR>SDK Diagnostic Information</h1>");
                                client.print("<BR><a href=\"/\">Monitor LCD Text Input Page</a><BR>Restarted WiFiConnections = ");
                               String diagdat="";
                                diagdat+=ulReconncount;
                                diagdat+="<BR>  Web Page Requests = ";
                                diagdat+=ulReqcount;
                                diagdat+="<BR>  WiFi Station Hostname = ";
                                diagdat+=wifi_station_get_hostname();
                                diagdat+="<BR>  Free RAM = ";
                                client.print(diagdat);
                                client.print((uint32_t)system_get_free_heap_size()/1024);
                                diagdat=" KBytes<BR>  Logged Sample Count = n/a";
//                                diagdat+=ulMeasCount;
                                diagdat+="<BR>  Total Sample points in 24 Hours = 1440<BR>  Minimum Sample Logging Interval = 1 Minutes<BR>  SDK Version = ";                                 
                                diagdat+=ESP.getSdkVersion();
                                diagdat+="<BR>  Boot Version = ";
                                diagdat+=ESP.getBootVersion();
                                diagdat+="<BR>  Free Sketch Space  = ";
                                diagdat+=ESP.getFreeSketchSpace()/1024;
                                diagdat+=" KBytes<BR>  Sketch Size  = ";
                                diagdat+=ESP.getSketchSize()/1024;
                                diagdat+=" KBytes<BR>";
                                client.print(diagdat);
                                client.printf("  Flash Chip id = %08X\n", ESP.getFlashChipId());
                                client.print("<BR>");
                                client.printf("  Flash Chip Mode = %s\n", (ideMode == FM_QIO ? "QIO" : ideMode == FM_QOUT ? "QOUT" : ideMode == FM_DIO ? "DIO" : ideMode == FM_DOUT ? "DOUT" : "UNKNOWN"));
                                diagdat="<BR>  Flash Size By ID = ";
                                diagdat+=ESP.getFlashChipRealSize()/1024;
                                diagdat+=" KBytes<BR>  Flash Size (IDE) = "; 
                                diagdat+=ESP.getFlashChipSize()/1024;
                                diagdat+=" KBytes<BR>  Flash Speed = ";
                                diagdat+=ESP.getFlashChipSpeed()/1000000;
                                diagdat+=" MHz<BR>  ESP8266 CPU Speed = ";
                                diagdat+=ESP.getCpuFreqMHz();
                                diagdat+=" MHz<BR>";
                                client.print(diagdat);
                                client.printf("  ESP8266 Chip id = %08X\n", ESP.getChipId());
                                diagdat="<BR>  System Instruction Cycles Per Second = ";
                                diagdat+=speedcnt*1000;  
                                diagdat+="<BR>  Last System Restart Time = n/a";
//                                diagdat+=MyNtP;
                                diagdat+="<BR>  Last System Restart Reason = ";
                                diagdat+=ESP.getResetInfo();                               
                                diagdat+="<BR>  System Time = n/a";
//                                diagdat+=TMnow;                                
                                diagdat+=" (Last Recorded Sample Time)<BR>  System VCC = ";
                                diagdat+=servolt1/1000, 3;
                                diagdat+=" V <BR>  Datalog File Size in Bytes = ";
                                diagdat+=Dfsize;
                                diagdat+="<BR>  System Uptime =";
                                diagdat+=duration1;
                                client.print(diagdat);
                                client.print("<BR><FONT SIZE=-2>environmental.monitor.log@gmail.com<BR><FONT SIZE=-2>ESP8266 With DHT11 Sensor & LCD<BR><FONT SIZE=-2>Compiled Using ver. 1.6.5-1160-gef26c5f, built on Sep 30, 2015<BR>");
                                client.println("<IMG SRC=\"https://raw.githubusercontent.com/genguskahn/ESP8266-For-DUMMIES/master/SoC/DimmerDocs/organicw.gif\" WIDTH=\"250\" HEIGHT=\"151\" BORDER=\"1\"></body></html>");
                                diagdat="";
  }
   else if(sPath=="/srestart")
  {
                                client.println("HTTP/1.1 200 OK"); 
                                client.println("Content-Type: text/html");
                                client.println("Connection: close");
                                client.println();
                                client.println("<!DOCTYPE HTML>");
                                client.print("<html><head><title>Web Monitor</title></head><body>");
                                client.print("<font color=\"#c0ff84\"><body bgcolor=\"#c384ff\">");
                                client.print("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=yes\">");
                                client.print("<h1>Web Monitor<BR>Please wait 10 Seconds......<BR><FONT SIZE=+2>System Reset Page !!!! </h1>");
                                client.print("<BR><a href=\"/monitor\">Sensor Gauges Page</a><BR><a href=\"/graphic\">Sensor Graph Page</a><BR><a href=\"/table\">Sensor Datalog Page</a><BR><a href=\"/diag\">Diagnostics Information Page</a><BR><a href=\"/\">Monitor LCD Text Input Page</a><BR><BR><BR><BR>Restarted WiFiConnections = ");
                                client.print(ulReconncount);
                                client.print("<BR><BR><BR><FONT SIZE=-2>environmental.monitor.log@gmail.com<BR><FONT SIZE=-2>ESP8266 With DHT11 Sensor & LCD<BR>");
                                client.print("<FONT SIZE=-2>Compiled Using ver. 1.6.5-1160-gef26c5f, built on Sep 30, 2015<BR>");
                                client.println("<IMG SRC=\"https://raw.githubusercontent.com/genguskahn/ESP8266-For-DUMMIES/master/SoC/DimmerDocs/organicw.gif\" WIDTH=\"250\" HEIGHT=\"151\" BORDER=\"1\"></body></html>");
client.stop();
wsysreset();
  }
else 
////////////////////////////
// 404 for non-matching path
////////////////////////////
  {
    sResponse="<html><head><title>404 Not Found</title></head><body><h1>Not Found</h1><p>The requested URL was not found on this server, What did you sk for?.</p></body></html>";
    ulReqcount++;
    sHeader  = F("HTTP/1.1 404 Not found\r\nContent-Length: ");
    sHeader += sResponse.length();
    sHeader += F("\r\nContent-Type: text/html\r\nConnection: close\r\n\r\n");
    
    // Send the response to the client
    client.print(sHeader);
    client.print(sResponse);
  }
  
  // and stop the client
  client.stop();
}


User avatar



#include <ESP8266WiFi.h>
#include "FS.h"
#include <LiquidCrystal.h>

LiquidCrystal lcd (2,14,16,5,4,0);

const char* ssid = "alexooo";
const char* password = "gaara123";
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
  lcd.begin(16, 2);

  lcd.setCursor(1, 0);
  lcd.print("EEE371 PROJECT");  

  lcd.setCursor(4, 1);
  lcd.print("GROUP 5");
  
  delay(1000);
  
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
  sResponse += F("<BR>Lecturer: ENGR AGBONTAEN<BR><FONT SIZE=-2>Group Members:<BR><FONT SIZE=-2>1. VICTORY AGHA<BR><FONT SIZE=-2>2. EMMANUEL AGBENIN<BR><FONT SIZE=-2>3. ERU MICHAEL<BR><FONT SIZE=-2>4. DANIEL ONYEKACHUKWU EMECHETE<BR><FONT SIZE=-2>5. IKHIMWIN EMMANUEL<BR><FONT SIZE=-2>6. ABIODUN IKUDANEJE<BR><FONT SIZE=-2>7. ABRAHAM IBIZUGBE<BR><FONT SIZE=-2>8. ALEX AFONUGHE<BR><FONT SIZE=-2>9. AGHEDO KELVIN<BR><FONT SIZE=-2>10. OMERE AISOSA<BR></body></html>");
  sResponse += F("<BR><BR><FONT SIZE=-2>ESP8266 With LCD<BR><FONT SIZE=-2></body></html>");
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
    sResponse += F("request.open(\"GET\", \"ajax_inputs\" + strLine1 + strLine2 + nocache, true);\nrequest.send(null);\n}\n</script>\n<body>\n<h1 align=\"center\";style=color:black;font-size:200%>UNIVERSITY OF BENIN<BR>E.E.E. DEPARTMENT</h1>\n<h2 align=\"center\";style=color:grey;font-size:100%>EEE371 PROJECT (Group 5)</h2>");   
    sResponse += F("<body onload=\"GetESP8266IO()\">\n<h1><FONT SIZE=-1>Enter text to send to LCD:</h1>\n<form id=\"txt_form\" name=\"frmText\">\n<label>Line 1: <input type=\"text\" name=\"line1LCD\" size=\"16\" maxlength=\"16\" /></label><br /><br />\n<label>Line 2: <input type=\"text\" name=\"line2LCD\" size=\"16\" maxlength=\"16\" /></label>\n");
    sResponse += F("</form>\n<br />\n<input type=\"submit\" value=\"Send Text\" onclick=\"SendText()\" />\n\n");    
    sResponse2 = F("<font color=black><body bgcolor=white><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=yes\"><BR><BR>\n");
    sResponse2 += F("</div>\n<div style=\"clear:both;\"></div><p>");
    sResponse2 += MakeHTTPFooter().c_str();
        // Send the response to the client 
    client.print(MakeHTTPHeader(sResponse.length()+sResponse2.length()).c_str());
    client.print(sResponse);
    client.print(sResponse2);
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


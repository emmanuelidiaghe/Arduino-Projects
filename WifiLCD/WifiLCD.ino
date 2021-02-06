
#define usingWiFiLCD

#include "C://Emancipator/Emancipator.h"

#include <LiquidCrystal.h>

LiquidCrystal lcd (2,14,16,5,4,0);

const char* ssid = "alexooo";
const char* password = "gaara123";

void setup() 
{
  lcd.setCursor(1, 0);
  lcd.print("EEE371 PROJECT");  

  lcd.setCursor(4, 1);
  lcd.print("GROUP 5");
  
  delay(1000);

   disp();
}


void loop() 
{
  
  //////////////////////////////////
  // format the html page for gauges
  //////////////////////////////////
 if(sPath=="/")
  {
    ulReqcount++;
    sResponse  = F("<html>\n<head>\n<title>Web Monitor Text to LCD</title>\n<script>\n\n");    
    sResponse += F("strLine1 = \"\";\nstrLine2 = \"\";\n\nfunction SendText()\n{\nnocache = \"&nocache=\" + Math.random() * 1000000;\nvar request = new XMLHttpRequest();\n\nstrLine1 = \"&L1=\" + document.getElementById(\"txt_form\").line1LCD.value;\nstrLine2 = \"&L2=\" + document.getElementById(\"txt_form\").line2LCD.value;\n\n");
    sResponse += F("request.open(\"GET\", \"ajax_inputs\" + strLine1 + strLine2 + nocache, true);\nrequest.send(null);\n}\n</script>\n<body>\n<h1 align=\"center\";style=color:black;font-size:200%>UNIVERSITY OF BENIN<BR>E.E.E. DEPARTMENT</h1>\n<h2 align=\"center\";style=color:grey;font-size:100%>EEE371 PROJECT (Group 7)</h2>");   
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


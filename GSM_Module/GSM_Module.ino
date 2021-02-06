/*
* Arduino Home Security GSM Alarm
* An Arduino + SIM900A Project
* T.K.Hareendran
* Tested at TechNode PROTOLABZ
* 21 August 2014
* http:// www.electroschematics.com
*/
//Connect the Tx pin of the GSM modem to D3
//Connect the Rx pin of the GSM modem to D4
//SMS Trigger Key/Input connected to D7 (Active LOW)
//CALL Trigger Key connected to D8 (Active LOW)
//END Key Connected to D9 (Active LOW)
#include <SoftwareSerial.h>
SoftwareSerial mySerial(3,4); // RX and TX pins to communicate with GSM module
#define msg_key 7
#define call_key 8
#define end_key 9
String number ="0000000000"; // Add the 10-Digit Mobile Number to which message / call is to be made,by replacing the 0's
void setup()
{
   Serial.begin(9600);
   mySerial.begin(9600);
   pinMode(msg_key,INPUT);
   pinMode(call_key,INPUT);
   pinMode(end_key,INPUT);
   digitalWrite(msg_key,HIGH);
   digitalWrite(call_key,HIGH);
   digitalWrite(end_key,HIGH);
}
void loop()
{
   //Sends an sms everytime msg_key is pressed
   if (digitalRead(msg_key)==LOW) // Check if the sms key is being pressed
   {
      mySerial.println("AT+CMGF=1"); // Set the Mode as Text Mode
      delay(150);
      mySerial.println("AT+CMGS=\"+00"+number+"\""); // Specify the Destination number in international format by             replacing the 0's
      delay(150);
      mySerial.print("Warning! Intruder Alert!"); // Enter the message 
      delay(150);
      mySerial.write((byte)0x1A); // End of message character 0x1A : Equivalent to Ctrl+z
      delay(50);
      mySerial.println();
   }
   //Makes a call when call_key is pressed
   else if (digitalRead(call_key)==LOW) // Check if the call key is being pressed
   {
      mySerial.println("ATD+91"+number+";"); //Specify the number to call
      while(digitalRead(call_key)==LOW);
      delay(50);
   }
   //Hang the call
   else if (digitalRead(end_key)==LOW) // Check if the hang up key is being pressed
   {
      mySerial.println("ATH");
      while(digitalRead(end_key)==LOW);
      delay(50);
   }
}

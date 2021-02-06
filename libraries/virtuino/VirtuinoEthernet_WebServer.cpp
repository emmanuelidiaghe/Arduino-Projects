/* Virtuino Ethernet Shield web server library ver 1.50
 * Created by Ilias Lamprou
 * Updated May 1 2017
 * 
 * Download latest Virtuino android app from the link: https://play.google.com/store/apps/details?id=com.javapapers.android.agrofarmlitetrial
 * Contact address for questions or comments: iliaslampr@gmail.com
 */



//========= Virtuino General methods  

//  void vDigitalMemoryWrite(int digitalMemoryIndex, int value)       write a value to a Virtuino digital memory   (digitalMemoryIndex=0..31, value range = 0 or 1)
//  int  vDigitalMemoryRead(int digitalMemoryIndex)                   read  the value of a Virtuino digital memory (digitalMemoryIndex=0..31, returned value range = 0 or 1)
//  void vMemoryWrite(int memoryIndex, float value);                  write a value to Virtuino memory             (memoryIndex=0..31, value range as float value)
//  float vMemoryRead(int memoryIndex);                               read a value of  Virtuino memory             (memoryIndex=0..31, returned a float value
//  run();                                                            neccesary command to communicate with Virtuino android app  (on start of void loop)
//  int getPinValue(int pin);                                         read the value of a Pin. Usefull to read the value of a PWM pin
 


#include "VirtuinoEthernet_WebServer.h"
//#include <SPI.h>
#include <Ethernet.h>



 
//====================================================================================== VirtuinoEthernet_WebServer init
//======================================================================================
  VirtuinoEthernet_WebServer::VirtuinoEthernet_WebServer(int port){
  serverPort=port;
}

 
 
 
//====================================================================================== run
//======================================================================================
 void VirtuinoEthernet_WebServer::run(){
    EthernetServer server(serverPort);  
    server.begin();
    EthernetClient client = server.available();
  if (client) {
    char c;
   if (DEBUG) Serial.println(et_clientConnected);
    
    while (client.connected()) {
      if (client.available()) {
        c = client.read();
        lineBuffer.concat(c);
        if (DEBUG)  Serial.write(c);
      }
      if (c == '\n' ) {
              int pos = lineBuffer.indexOf("GET /");
              if (pos!=-1){                                   // We have a GET message
                   if (DEBUG) Serial.println("\n\r LineBuffer="+lineBuffer);
                  String response = checkVirtuinoCommand(lineBuffer.substring(pos+5));
                   if (DEBUG) Serial.println("\n\r response="+response);
                  delay(10);
                  client.flush();
                  client.println("HTTP/1.1 200 OK");
                  client.println("Content-Type: text/html");
                  client.println("Connection: close");
                  client.println();
                  client.println(response);
                  lineBuffer="";
                  break;
                } // if pos
             lineBuffer="";
        } // if c=='\n'
    } // if client.available
    delay(1);
    client.stop();  // close the connection:
    if (DEBUG) Serial.println(et_clientDisconnected);
  }
  }
  
//====================================================================================== checkVirtuinoCommand
//======================================================================================
String  VirtuinoEthernet_WebServer::checkVirtuinoCommand(String command){
  String returnedString="";
  
  int pos=command.lastIndexOf(et_COMMAND_END_CHAR);                      
  if (pos>5){                                          // check the size of command
      String command1= command.substring(0,pos+1);     // clear the command
      if (DEBUG) Serial.println("\r\nCommand = "+ command1);

       //------------------  get command password
       String commandPassword="";
       int k=command1.indexOf(et_COMMAND_START_CHAR);
       if (k>0) commandPassword = command1.substring(0,k);
       if (DEBUG) Serial.println("\r\nCommand password:"+commandPassword ); 
       
        
      if ((password.length()==0) || (commandPassword.equals(password))) {                       // check if password correct
           while (command1.indexOf(et_COMMAND_START_CHAR)>=0){
              int cStart=command1.indexOf(et_COMMAND_START_CHAR);
              int cEnd=command1.indexOf(et_COMMAND_END_CHAR);
              if (cEnd-cStart>5){
              String oneCommand = command1.substring(cStart+1,cEnd);                               // get one command
                char commandType = getCommandType(oneCommand);
                  if (commandType!='E') {
                     int pin= getCommandPin(oneCommand);
                     if (pin!=-1){
                        boolean returnInfo=false;
                        float value=0;
                        if (oneCommand.charAt(4)=='?') returnInfo=true;
                        else value = getCommandValue(oneCommand);
                        String commandresponse=executeReceivedCommand(commandType,pin , value,returnInfo);
                         lastCommunicationTime=millis();
                        returnedString += commandresponse;
                     } else  returnedString +=getErrorCommand(et_ERROR_PIN);  // response  error pin number   !E00=1$   
                  } else returnedString +=getErrorCommand(et_ERROR_TYPE);  // response  error type   !E00=3$   
          
              } else{
                returnedString +=getErrorCommand(et_ERROR_SIZE);  // response  error size of command   !E00=4$   
              }
              command1=command1.substring(cEnd+1);
           }  // while
      } else returnedString=getErrorCommand(et_ERROR_PASSWORD);     // the password is not correct
  }
  else  returnedString=getErrorCommand(et_ERROR_SIZE);         // the size of command is not correct
 
  return returnedString;
}


 //====================================================================================== getCommandType
 //======================================================================================
  //This function returns the command type which was received from app
  //The second character of command string determines the command type
  // I  digital input read command    
  // Q  digital ouput write
  // D  digital memory read - write commang
  // A  analog input analog input read command
  // O  analog output write command
  // V  float memory read - write command
  // C  connect or disconect command    
  // E  error
  
  // Other characters are recognized as an error and function returns the character E
  //This is a system fuction. Don't change this code
  
  char  VirtuinoEthernet_WebServer::getCommandType(String aCommand){
    char commandType= aCommand.charAt(0);
    if (!(commandType == 'I' || commandType == 'Q' || commandType == 'D' ||commandType == 'A' ||commandType == 'O' || commandType == 'V' || commandType == 'C')){
      commandType='E'; //error  command
    }
    return commandType;
  }

  //======================================================================================  getCommandPin
  //======================================================================================
  //This function returns the pin number of the command string which was received from app
  //Fuction checks if pin number is correct. If not it returns -1 as error code
  //This is a system fuction. Don't change this code
 
  int  VirtuinoEthernet_WebServer::getCommandPin(String aCommand){
    char p1= aCommand.charAt(1);
    char p2= aCommand.charAt(2);
    String s="-1";
    if (isDigit(p1) && isDigit(p2)) {
       s="";
       s+=p1;
       s+=p2;
    }
     return s.toInt();
  }

//======================================================================================  getCommandValue
//======================================================================================
float VirtuinoEthernet_WebServer::getCommandValue(String aCommand){
     float value=0;
     String valueString=aCommand.substring(4);
     boolean er=false;
       for (int i=0;i<valueString.length();i++){
        char c=valueString.charAt(i);
        if (! ((c=='.') || (c=='-') || (isDigit(valueString.charAt(i))))) er=true;
       }
       if (er==false) value=valueString.toFloat();
      
    return value;
  }

//====================================================================================== getErrorCommand
//======================================================================================
String VirtuinoEthernet_WebServer::getErrorCommand(byte code){
  return et_COMMAND_START_CHAR+et_COMMAND_ERROR+String(code)+et_COMMAND_END_CHAR;
}

//====================================================================================== executeCommandFromApp
//======================================================================================
  String VirtuinoEthernet_WebServer::executeReceivedCommand(char activeCommandType, int activeCommandPin, float activeCommandValue, boolean returnInfo){
    //------------------------------------
   // Every time we have a complete command from bluetooth the global value commandState is set to 1 
   // The value activeCommandType contains command type such as Digital output, Analog output etc.
   // The value activeCommandPin contains the pin number of the command
   // The value activeCommandValue contains the value (0 or 1 for digital, 0-255 for analog outputs, 0-1023 for analog memory) 
   // In this void we have to check if the command is correct and then execute the command 
   // After executing the command we have to send a response to app
   // At last we must reset the command state value to zero 
      
    String response=getErrorCommand(et_ERROR_UNKNOWN); 
    String pinString="";
    
    if (activeCommandPin<10) pinString = "0"+String(activeCommandPin);
    else pinString=String(activeCommandPin);
    switch (activeCommandType) {
      case 'I':                         
            if ((activeCommandPin>=0) & (activeCommandPin < et_arduinoPinsSize))
              response =et_COMMAND_START_CHAR+String(activeCommandType)+pinString+"="+String(digitalRead(activeCommandPin))+et_COMMAND_END_CHAR;  // response 
            else   response =getErrorCommand(et_ERROR_PIN );  // response  error pin number   !E00=1$       
          break;
      case 'Q': 
            if ((activeCommandPin>=0) & (activeCommandPin < et_arduinoPinsSize)){
                 if (returnInfo) response =et_COMMAND_START_CHAR+String(activeCommandType)+pinString+"="+String(digitalRead(activeCommandPin))+et_COMMAND_END_CHAR;  // response 
                 else 
                   if ((activeCommandValue == 0) || (activeCommandValue == 1)) {
                          digitalWrite(activeCommandPin,activeCommandValue);
                          arduinoPinsValue[activeCommandPin]=activeCommandValue;
                          response =et_COMMAND_START_CHAR+String(activeCommandType)+pinString+"="+String(digitalRead(activeCommandPin))+et_COMMAND_END_CHAR;  // response 
                   } else    response =getErrorCommand(et_ERROR_VALUE);
            } else   response =getErrorCommand(et_ERROR_PIN );  // response  error pin number   !E00=1$   
          break; 

       case 'D':
            if ((activeCommandPin>=0) & (activeCommandPin<et_virtualDigitalMemorySize)){ 
                if (returnInfo) {
                  response =et_COMMAND_START_CHAR+String(activeCommandType)+pinString+"="+String(virtualDigitalMemory[activeCommandPin])+et_COMMAND_END_CHAR;  // response 
                 }
                else
                {
                      virtualDigitalMemory[activeCommandPin]= activeCommandValue; 
                      response =et_COMMAND_START_CHAR+String(activeCommandType)+pinString+"="+String(virtualDigitalMemory[activeCommandPin])+et_COMMAND_END_CHAR;  // response 
                }// else    response =getErrorCommand(et_ERROR_VALUE);
            } else   response =getErrorCommand(et_ERROR_PIN );  // response  error pin number   !E00=1$   
            break; 
      
       case 'V': 
           if ((activeCommandPin>=0) & (activeCommandPin<et_virtualAnalogMemorySize)){
               if (returnInfo) response =et_COMMAND_START_CHAR+String(activeCommandType)+pinString+"="+String(virtualFloatMemory[activeCommandPin])+et_COMMAND_END_CHAR;  // response
               else { 
                    virtualFloatMemory[activeCommandPin]= activeCommandValue; 
                    response =et_COMMAND_START_CHAR+String(activeCommandType)+pinString+"="+String(virtualFloatMemory[activeCommandPin])+et_COMMAND_END_CHAR;  // response
                    } 
           } else   response =getErrorCommand(et_ERROR_PIN );
          break;
       case 'O': 
           if ((activeCommandPin>=0) & (activeCommandPin < et_arduinoPinsSize)){
                   
                   if (returnInfo) {
                      int pwm_value = pulseIn(activeCommandPin, HIGH);
                      pwm_value = pwm_value /7.85;
                      response =et_COMMAND_START_CHAR+String(activeCommandType)+pinString+"="+String(pwm_value)+et_COMMAND_END_CHAR;  // response 
                  }
                   else if ((activeCommandValue>=0) && (activeCommandValue<256)) {
                      //arduinoPinsMap[activeCommandPin]=3; 
                      arduinoPinsValue[activeCommandPin]=(int) activeCommandValue;
                      analogWrite(activeCommandPin,(int)activeCommandValue);
                      response =et_COMMAND_START_CHAR+String(activeCommandType)+pinString+"="+String((int)activeCommandValue)+et_COMMAND_END_CHAR;  // response 
                  } else response =getErrorCommand(et_ERROR_VALUE);
           } else   response =getErrorCommand(et_ERROR_PIN );;
          break;

         case 'A':                        
            if ((activeCommandPin>=0) & (activeCommandPin < arduinoAnalogPinsSize))
              response ="!"+String(activeCommandType)+pinString+"="+String(analogRead(analogInputPinsMap[activeCommandPin]))+"$";  // response 
            else   response =getErrorCommand(et_ERROR_PIN );  // response  error pin number   !E00=1$       
          break;  
         
           case 'C':                         
            if ((activeCommandPin>=0) & (activeCommandPin <=1)){          //0 connect via bluetooth - 1 connect via network 
               if (activeCommandValue==1) {
                 response ="!C"+pinString+"="+et_firmwareCode+"$";
               }
              else if (activeCommandValue==0) {
                response =et_COMMAND_START_CHAR+"C"+pinString+"=0"+et_COMMAND_END_CHAR;
               }
              else      response =getErrorCommand(et_ERROR_VALUE);;
              
            } else   response =getErrorCommand(et_ERROR_COMMAND);        
          break;  
           }
           
        return response;
       
  }

//====================================================================================== getPinValue
//======================================================================================
int VirtuinoEthernet_WebServer::getPinValue(int pin){
  if (pin>=0 && pin<et_arduinoPinsSize) return arduinoPinsValue[pin];
  else return 0;
  }

//====================================================================================== vDigitalMemoryWrite
//======================================================================================
 void VirtuinoEthernet_WebServer::vDigitalMemoryWrite(int digitalMemoryIndex, int value){
  if ((digitalMemoryIndex>=0) && (digitalMemoryIndex<et_virtualDigitalMemorySize)){
    virtualDigitalMemory[digitalMemoryIndex]=value;
    
  }
  
}
//====================================================================================== vDigitalMemoryRead
//======================================================================================
 int VirtuinoEthernet_WebServer::vDigitalMemoryRead(int digitalMemoryIndex){
  if ((digitalMemoryIndex>=0) & digitalMemoryIndex<et_virtualDigitalMemorySize){
    return virtualDigitalMemory[digitalMemoryIndex];
  }
  else return 0;    // error
}

//====================================================================================== vMemoryWrite
//======================================================================================
// This void must not be called more than two times per second
void VirtuinoEthernet_WebServer::vMemoryWrite(int memoryIndex, float value){
  if ((memoryIndex>=0) & memoryIndex<et_virtualAnalogMemorySize){
      virtualFloatMemory[memoryIndex]=value;
  }
}

//====================================================================================== vMemoryRead
//======================================================================================
 float VirtuinoEthernet_WebServer::vMemoryRead(int memoryIndex){
  if ((memoryIndex>=0) & memoryIndex<et_virtualAnalogMemorySize){
    return virtualFloatMemory[memoryIndex];
  }
  else return 0;    // error
}


//====================================================================================== vDelay
//======================================================================================
void VirtuinoEthernet_WebServer::vDelay(long milliseconds){
  long timeStart=millis();
  while (millis()-timeStart<milliseconds) run();
}
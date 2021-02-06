//  ####  ESP8266/NodeMCU WiFiBot Control Demo ####
//  Copyright PlastiBots - www.plastibots.com
// 
// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.  Also, credit
// due to those who developed included libraries.  This software is provide for hobby purposes only.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF 
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE 
// FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION 
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
//  
//  Parses the URL command string directly from WiFi Bot Control
//  Requirements:  Config WiFi Bot Control Data Processing URL as http://[YourIP of ESP]/&  For example:  http://192.168.4.1/?   (yes make sure the ? is there)
//  The following comes through and will be parsed:
//  GET &/URLmode=1&URLxval=38&URLyval=-16&URLudlr=&URLcmdVal= HTTP/1.1
 
// **CREDITS ** 
//Super shout out here:  http://arduino.stackexchange.com/questions/13388/arduino-webserver-faster-alternative-to-indexof-for-parsing-get-requests
//http://arduino.stackexchange.com/questions/1013/how-do-i-split-an-incoming-string
//http://jhaskellsblog.blogspot.ca/2011/06/parsing-quick-guide-to-strtok-there-are.html
// see https://learn.sparkfun.com/tutorials/esp8266-thing-hookup-guide/example-sketch-ap-web-server
 
 
//Version History
// v14 - Using TB6612FNG motor driver - can refer to sample here https://learn.sparkfun.com/tutorials/tb6612fng-hookup-guide?_ga=1.189615467.1166851119.1488147174#board-overview
// v13 - integrating motor driving features of original WiFiBot Control.  Using Digispark Motor Sheid as example
// v12 - stable baseline version.  No integration of motor drivers but proven working within local network.
// v9  - Stable working baseline
 
 
 
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
 
const char* ssid = "Emancipator";
const char* password = "emmanuelidiags";
 
 
// how much serial data we expect before a newline
const unsigned int MAX_INPUT = 100;
// the maximum length of paramters we accept
const int MAX_PARAM = 75;
int URLmode=0, URLxval=0, URLyval=0, URLcmdVal=0;
char * udlr[2];

WiFiServer server(80);
 
//Setup Pins - motors etc.  
//Using Using TB6612FNG motor driver
//*** Note - this is for example purposes only - you will likely need to modify this to suit your own motor driver.

/*
int STBY = 15; //standby  GPIO Pin 15   D8 on NodeMCU Mini.
//Motor A
int PWMA = 5; //Speed control           D1 on NodeMCU Mini.
int AIN1 = 4; //Direction               D2 on NodeMCU Mini.
int AIN2 = 0; //Direction               D3 on NodeMCU Mini.
//Motor B
int PWMB = 14; //Speed control          D5 on NodeMCU Mini.
int BIN1 = 12; //Direction              D6 on NodeMCU Mini.
int BIN2 = 13; //Direction              D7 on NodeMCU Mini. 
*/
int leftSpeed, rightSpeed;
int gpi0_0 = 0;
int gpio_2 = 2;


void setup() 
{
  Serial.begin(115200);
/*
  pinMode(STBY, OUTPUT);
  pinMode(PWMA, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);

  pinMode(PWMB, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);  
  */
  pinMode(gpio_0, OUTPUT);
  pinMode(gpio_2, OUTPUT);
  
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  
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
 
  // Start the server
  server.begin();
  Serial.println("Server started");
}
 
//#############################################################################################################################
void loop() 
  {
    
  // listen for incoming clients
  WiFiClient client = server.available();
  if (!client) {
    //Serial.println("could not create client");
    yield();
    return;
  }
  
  // Wait until the client sends some data
  while (!client.available()) {
    delay(1);
    yield();
  }
  if (client) 
    {
    //Serial.println(F("Client connected"));
    // an http request ends with a blank line
    boolean done = false;
    while (client.connected() && !done) 
      {
      while (client.available () > 0 && !done)
        done = processIncomingByte (client.read ());
      }  // end of while client connected
 
    // send a standard http response header
    //Serial.println("Sending response");
    // send a standard http response header
    client.println("HTTP/1.0 200 OK");
    client.println("Content-Type: text/html");
    client.println("Connection: keep-alive");
    client.println();
  
    // give the web browser time to receive the data
    delay(10);
    // close the connection:
    client.stop();
    //Serial.println(F("Client disconnected"));
  }  // end of got a new client
 
   //reset the data so your robot does not drive off into the sunset
   //URLmode = -99;//, URLxval = 0, URLyval = 0, URLcmdVal = 0;
   
}  // end of loop
 
 
//#############################################################################################################################

void doCmd(){
  if xval
}
void driveByJoystick(int xval, int yval)
{
     if (xval != 0 && yval != 0)
        {
          driveMotors(xval,yval);
        } 
        else
        {
         //analogWrite(leftMtrSpdPin, 0);   //stop the motor
         //analogWrite(rightMtrSpdPin, 0);  //stop the motor
         stop();
        }
}
 
 
//#############################################################################################################################
void driveMotors(int xVal, int yVal)
{ 
  float xPct=1.0;
  int xAdj, yAdj;
  //xAdj = map(abs(xVal), 0, 100, 100, 255);
  //yAdj = map(abs(yVal), 0, 100, 100, 255);
  //For the ESP, values are 0 - 1023
  xAdj = map(abs(xVal), 0, 100, 0, 1023);
  yAdj = map(abs(yVal), 0, 100, 0, 1023);

  Serial.print("Xadj:"); Serial.println(xAdj);
  Serial.print("Yadj:"); Serial.println(yAdj);

 
  //approach - 
  //if Y is positive, both motors are set to move forward, else reverse. Left and Right from center will determine the speed of each motor.  At extremes will reverse each motor for fast turns
  //the value for X will determine the relative speed of each motor.


  //determine left / right
  if (xVal <= 0) //LEFT
  {
    if (xVal < -70 && (yVal <= 40 || yVal >= -40))  //fast turn
    {
      //move(1, 1023, 0); //motor 1, full speed, left  //was 255
      //Serial.println("fast turn left");
      leftSpeed = 200;  //something fast, but not too crazy
      rightSpeed = 1023;
    }
    else
    {
      //slower turn
      //Serial.println("Slower turn left");
      leftSpeed = (float)yAdj * ((float)(100 - abs(xVal)) / 100);
      rightSpeed = yAdj;
    }    
  }    
  else  //RIGHT
  {
    if (xVal > 70 && (yVal <= 40 || yVal >= -40))  //fast turn
    {
      //move(2, 1023, 0); //motor 2, full speed, left 
      //Serial.println("fast turn right");
      leftSpeed = 1023;
      rightSpeed = 200; //something fast, but not too crazy
    }
    else
    {
      //slower turn
      //Serial.println("slower turn right");
      leftSpeed = yAdj;
      rightSpeed = (float)yAdj * ((float)(100 - abs(xVal)) / 100);
    }
  }

  
  //now determine fwd or back as this will result in flipping values.
  if (yVal >= 0)  //going forward
  {
    move(2, leftSpeed, 1); //motor 1, full speed, left  //was note ESP PWM resolution is 0 > 1023
    move(1, rightSpeed, 1); //motor 2, full speed, left  //was 255 
  }
   else  //reversing
  {
   move(1, leftSpeed, 0); //motor 1, full speed, left  //was 255
   move(2, rightSpeed, 0); //motor 2, full speed, left  //was 255   
  }
  
  
  
   //Serial.print("yAdj: ");
   //Serial.print(yAdj);
   //Serial.print("  xVal: ");
   //Serial.print(xVal);
   //Serial.print("  leftspeed: ");
   //Serial.print(leftSpeed);
   //Serial.print("    rightspeed: ");
   //Serial.println(rightSpeed);
   //Serial.print("     100-lesvxal:");
   //Serial.print(100 - abs(xVal));
   //Serial.print("  100-lesvxal/100: ");
   //Serial.println((100 - abs(xVal))/100, DEC);
  
   //drive the motors  
   //move(1, leftSpeed, 0); //motor 1
   //move(2, rightSpeed, 0); //motor 2

  yield();
  leftSpeed=0, rightSpeed=0, xAdj=0, yAdj=0;
}
 
//#############################################################################################################################
void move(int motor, int speed, int direction){
//Move specific motor at speed and direction
//motor: 0 for B 1 for A
//speed: 0 is off, and 255 is full speed
//direction: 0 clockwise, 1 counter-clockwise

  digitalWrite(STBY, HIGH); //disable standby

  boolean inPin1 = LOW;
  boolean inPin2 = HIGH;

  if(direction == 1){
    inPin1 = HIGH;
    inPin2 = LOW;
  }

  if(motor == 1){
    digitalWrite(gpio_0, inPin1);
    digitalWrite(gpio_2, inPin2);
    
  }else{
    digitalWrite(gpio_2, inPin1);
    digitalWrite(gpio_0, inPin2);
  }
}

void stop(){
//enable standby  
  digitalWrite(gpio_0, LOW); 
  digitalWrite(gpio_2, LOW);
}

 
//#############################################################################################################################
void processGet (const char * data)
  {
  // find where the parameters start
  char * paramsPos = strchr (data, '?'); 
  //Serial.print("ParamsRaw:"); Serial.println(paramsPos);
  if (paramsPos == NULL)
  {
    return;  // no parameters
  }
  // find the trailing space
  const char * spacePos = strchr (paramsPos, ' ');
  //Serial.print("SpacePos:"); Serial.println(spacePos);
  if (spacePos == NULL)
  {
    return;  // no space found
  }
  // work out how long the parameters are
  int paramLength = spacePos - paramsPos ;
    // see if too long
  //Serial.print("ParamsLength:"); Serial.println(paramLength);
  if (paramLength >= MAX_PARAM)
  {
    //Serial.println("Pram length too long. Stopping");
    return;  // too long for us
  }
  // copy parameters into a buffer
  char param [MAX_PARAM];
  char paramVal[4][4]; //stores the char version of all values. //[rows] x [characters]
  strncpy (paramsPos,"&",1);   //replace the ? with & - a little cheat to make the below processing work for all values.
  //memcpy (param, paramsPos + 1, paramLength);  // skip the "?"
  memcpy (param, paramsPos, paramLength); 
  param [paramLength] = 0;  // null terminator
  
  Serial.print("ParamsURL>>  "); Serial.println(param);
  int count = 0;
  char *URLpair;
  const char s[2] = "&";  //going to be looking for these.. they split each parameter.
 
  //split the all URL parameters and their values apart. i.e URLMode=1 from URLxval=55 etc
  URLpair = strtok(param, s);
  while(URLpair != NULL)
  {
    //Serial.print("URLpair:  "); Serial.println(URLpair);
    //memset(paramVal[count], 0, sizeof(paramVal)); 
    memcpy(paramVal[count], strchr(URLpair,'=')+1 /* Offset */, 4 /* Length */); 
    paramVal[count][4] = '\0';
    //Serial.print("Values>"); Serial.println(paramVal[count]);
    URLpair = strtok(NULL, s);  //need to null the & at each iteration or it will just keep returning the first val it finds.
 
      //Assign to usable global variables for motor control and other functions
      //dont really want to do it this way, but something is corrupting xvals when it goes to negative. 
      //USE THESE VARIABLES TO DRIVE YOUR BOT AND REACT TO MODES AND COMMMANDS
      
   //see about making this faster - dont process or do something with data if value is null / blank.  Its char so ''.
   
   switch (count) {
        case 0:
          URLmode = atoi(paramVal[0]);
          break;
        case 1:
          URLxval = atoi(paramVal[1]);
          break;
        case 2:
          URLyval = atoi(paramVal[2]);
          break;
        case 3:
          udlr[0] = paramVal[3];
     udlr[1] = '\0'; //add terminating character
          break;
        case 4:
          URLcmdVal = atoi(paramVal[4]);
          break;
      }
    count++;
  }
 
  //DEBUGGING
  Serial.print("URLmode="); Serial.println(URLmode);
  Serial.print("URLxval="); Serial.println(URLxval);
  Serial.print("URLyval="); Serial.println(URLyval);
  //URLudlr is already ready. Values of U D L or R will be passed in paramVal[3] - you can poll this do do what you need.
  //Serial.print("udlr="); Serial.println(udlr);
  Serial.print("URLcmdVal="); Serial.println(URLcmdVal);
   
  //make sure to clean up the memory as it will corrupt after each iteration.
  //for (int i=0; i<5; i++)
  //{
  //  memset(paramVal[i], 0, sizeof(paramVal));  
  //}
 
switch (URLmode)
   {
    case 1: //driving using joystick and x y values
   doCmd();  //sample drive routine below.  Note this is basic, so you may need to refine it.
    break;
 
    case 2: //driving using U D L R from joystick. !! note, clicking the middle of the joystick sends URLmode=99. You can use that to stop the robot
      //You can drop yoru function call in there for simple joystick processing.
    break;
    
    case 3: //URL mode indicating command values being sent.  Process URLcmdVal as int values.
      //you can write your own funciton to process command values.    
    break;
   }
 
  
}  // end of processGet
 
//#############################################################################################################################
// here to process incoming serial data after a terminator received
void processData (const char * data)
  {
  //Serial.println (data);
  if (strlen (data) < 4)
    return;
 
  if (memcmp (data, "GET ", 4) == 0)
    processGet (&data [4]);
  }  // end of processData
 
//#############################################################################################################################
bool processIncomingByte (const byte inByte)
  {
  static char input_line [MAX_INPUT];
  static unsigned int input_pos = 0;
  switch (inByte)
    {
    case '\n':   // end of text
      input_line [input_pos] = 0;  // terminating null byte
      if (input_pos == 0)
        return true;   // got blank line
      // terminator reached! process input_line here ...
      processData (input_line);
      // reset buffer for next time
      input_pos = 0;  
      break;
 
    case '\r':   // discard carriage return
      break;
 
    default:
      // keep adding if not full ... allow for terminating null byte
      if (input_pos < (MAX_INPUT - 1))
        input_line [input_pos++] = inByte;
      break;
    }  // end of switch
  return false;    // don't have a blank line yet
  } // end of processIncomingByte  
 
 
 
//#####################################################################################################################################################
//#####################################################################################################################################################
//#############################################################################################################################
//#############################################################################################################################
//#############################################################################################################################


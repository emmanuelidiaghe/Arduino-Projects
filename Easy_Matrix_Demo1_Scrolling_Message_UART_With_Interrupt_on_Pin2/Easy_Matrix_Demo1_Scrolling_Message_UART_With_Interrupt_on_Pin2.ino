/*
 Project: Scrolling Message Display using Easy Matrix modules in cascade
 Description: This is demo project for Easy Matrix modules cascaded together to make a scrolling
 message display. It uses Adafruit's GFX and Mark Ruys' Max72xxPanel libraries both.
 
 The project receives display data and commands through UART. You can send a character stream through Serial Monitor
 tool from the Arduino IDE. You also need a 1K resistor that is used to connect Arduino Pin 0 (Rx) and Pin 2 (External Interrupt).
 This interrupts the Arduino every time a new command or character stream is received through UART so that it could respond to
 that.
 
 Here are the commands Arduino recognizes.
 1. Any character stream must be sent enclosed inside parenthesis. 
    Example: Suppose if you want to display a message "Hello! How are you doing?"
    You should send it like this: (Hello! How are you doing?)
    When the Arduino receives the first character as '(', it understands that the upcoming characters are display data until ')'
    is received. The message is displayed as scrolling from right to left.
    
 2. Display intensity can be incremented or decremented by sending /+ or /- respectively. Every time the Arduino receives '/', 
    it understands it is a command. It supports 16 brightness levels.
   
 3. Similarly, scrolling speed can be increased or decreased by sending /< or /> respectively. 
 
 4. /p is used to pause the scrolling message. Sending this command again resumes the scroll.
 
 5. /e is used to erase the buffer and blank the display
 
 Connections:
 Easy Matrix              Arduino
 VCC                      +5V
 DIN                      MOSI (Pin 11)
 LOAD                     pinCS defined in the program below
 CLK                      SCK (Pin 13)
 GND                      Gnd
 
 The UART communication happens at 9600 baud.
 
 Written by: Rajendra Bhatt
 Contact: admin@embedded-lab.com
 Easy Matrix product link: http://embedded-lab.com/blog/?p=9186
 
*/

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Max72xxPanel.h>

int pinCS = 10; // Attach CS to this pin, DIN to MOSI and CLK to SCK (cf http://arduino.cc/en/Reference/SPI )
int numberOfHorizontalDisplays = 4;
int numberOfVerticalDisplays = 1;

Max72xxPanel matrix = Max72xxPanel(pinCS, numberOfHorizontalDisplays, numberOfVerticalDisplays);
int scrollspeed = 200; // Default scrollspeed (milliseconds)

int spacer = 1;  // Space between two characters
int width = 5 + spacer; // The font width is 5 pixels
boolean inChar = false, NewData = false, pause = false;
boolean dataAvailable = false;
char inputString[512];
int count = 0, BTvalue = 5;


void setup() {

  matrix.setIntensity(10); // Use a value between 0 and 15 for brightness

// Adjust to your own needs
  matrix.setPosition(0, 7, 0); // The first display is at <0, 7>
 // matrix.setPosition(1, 6, 0); // The second display is at <1, 0>
 // matrix.setPosition(2, 5, 0); // The third display is at <2, 0>
 // matrix.setPosition(3, 4, 0); // And the last display is at <3, 0>
 // matrix.setPosition(4, 3, 0); // The first display is at <0, 0>
 // matrix.setPosition(5, 2, 0); // The second display is at <1, 0>
 // matrix.setPosition(6, 1, 0); // The third display is at <2, 0>
 // matrix.setPosition(7, 0, 0); // And the last display is at <3, 0>
 //  ...
 //  matrix.setRotation(0, 2);    // The first display is position upside down
 //  matrix.setRotation(3, 2);    // The same hold for the last display
 // matrix.fillScreen(0);
  matrix.write();
  Serial.begin(9600); // serial communication initialize
  
  // Using interrupt 0 on digital pin 2.
  pinMode(2, INPUT);
  digitalWrite(2, LOW);
  attachInterrupt(0, serialInterrupt, CHANGE);
  
}

void loop(){
 if(dataAvailable){
  display_data();
 } 
}

void display_data(){
  for ( int i = 0 ; i < width * count + matrix.width() - 1 - spacer; i++ ) {

    matrix.fillScreen(0);

    int letter = i / width;
    int x = (matrix.width() - 1) - i % width;
    int y = (matrix.height() - 8) / 2; // center the text vertically

    while ( x + width - spacer >= 0 && letter >= 0 ) {
      if ( letter < count ) {
        matrix.drawChar(x, y, inputString[letter], HIGH, LOW, 1);
      }

      letter--;
      x -= width;
    }
    matrix.write(); // Send bitmap to display
    if(!dataAvailable){
      i = width * count + matrix.width() - 1 - spacer; 
      matrix.fillScreen(LOW);
      matrix.write();
    }
    if(NewData){
      i = 0; 
      matrix.fillScreen(LOW);
      matrix.write();
      NewData = false;
    }
    while(pause == true){
      delay(0);
    } // Pause here if pause command is received
    
    delay(scrollspeed);
  }
}

// Volatile, since it is modified in an ISR.
volatile boolean inService = false;

void serialInterrupt(){
  
  if (inService) return;

  // You was not in service. Now you are.
  inService = true;
  
  // Reenable interrupts, to allow Serial to work. We do this only if inService is false.
  interrupts();
  while(!Serial.available());
  while (Serial.available()) {
     char ch = Serial.read();
     if(ch =='('){
       count = 0;
       inChar = true;
       
       while(inChar){
         if (Serial.available()) {
           ch = Serial.read();
           if(ch == ')'){
             inChar = false;
             dataAvailable = true;
            } else{
             inputString[count] = ch;
             count++;
           }
           if(count > 0) {
             NewData = true;
           }
         }
       }
     }  
     
     if(ch =='/'){   // Command mode
       inChar = true;
       while(inChar){
         if (Serial.available()) {
           ch = Serial.read();
           // '/+' is for brightness increment
           if(ch == '+'){
             if(BTvalue < 15) {
               BTvalue ++;
               matrix.setIntensity(BTvalue);
              }
             break;
           }  
           // '/-' is for brightness decrement           
           if(ch == '-'){
            if(BTvalue > 0){
             BTvalue --;
             matrix.setIntensity(BTvalue);
            }
            break;
           }    

           // '/>' is to slow down scroll
           if(ch == '>'){
             if(scrollspeed < 500) {
               scrollspeed = scrollspeed + 50;
             }
             break;
           }  
           // '/<' is to make scroll faster           
           if(ch == '<'){
            if(scrollspeed > 50){
             scrollspeed=scrollspeed-50;
            }
            break;
           }   

           // '/e' is to erase display           
           if(ch == 'e'){
            dataAvailable = false;
             break; 
           }
           
           // '/p' is to pause display           
           if(ch == 'p'){
             if(pause == false){
               pause = true;
             } 
             else {
               pause = false;
             } 
             break; 
           }
           
           else {
            break;  // Unrecognized command 
           }
           
           
         }
       }
     }  // if(ch == '!')

    
  }  // while(serial.available)
  inService = false;
}




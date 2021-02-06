
#include <TimerOne.h>
#include <StivSeg.h>

int count = 0;
int numDigits = 2;
int digitPins[2] = {10, 11};
//int segPins[8] = {2, 3, 4, 5, 6, 7, 8, 9}; //A-G and DP
int segPins[8] = {9, 8, 7, 6, 5, 4, 3, 2}; //A-G and DP; I soldered my segment in reverse
bool isComCathode = true;
StivSeg disp(3, digitPins, segPins, isComCathode);

void setup() {
  Timer1.initialize(1000000); //occurs every 1sec.
  Timer1.attachInterrupt(countSteps);
}

void loop() 
{
  disp.writeInt(count); //display count variable
}

void countSteps(void)
{
  if(count<100)
  {
    count++;
  }
  if(count == 100)
  {
    count = 0;
  }
}



/*
 * counter using TCNT1.cpp
 *
 * Created: 11-Apr-17 10:00:07 PM
 * Author : Emancipator
 */ 
#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>


int main(void)
{
	DDRA = 0xFF;  // set for output
	PORTA= 0x00;
	DDRC = 0xFF;  // set for output
	PORTC= 0x00; 
	
	int count1   = 0;
	int count2   = 0;
	int led1[9] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80}; //Array of LED shifts
	//int led2[9] = {0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};

	TCCR1B|=(1<<CS12); // using 256 pre-scaler
	
    while (1) 
    {
		if (TCNT1 == 7813)  //set counter to 1/8th second
		{
			TCNT1 = 0;
			PORTA = led1[count1];
			count1++;
			if (count1 == 8)
			{
				count1 = 0;
				PORTC  = led1[count2];
				count2++;
				if (count2 == 8)
				{
					count2 = 0;
				}
			}
		}
	}
}
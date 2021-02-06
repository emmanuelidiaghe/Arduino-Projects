/*
 * push button.cpp
 *
 * Created: 05-Apr-17 11:16:53 AM
 * Author : Emancipator
 */ 
#define F_CPU 12000000
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
    DDRA=0xFF;
    PORTA=0x00;
    DDRD=0x00;
    PORTD=0x40;
    
    int led_on[9]={0x01,0x03,0x07,0x0F,0x1F,0x3F,0x7F,0xFF};
    int a=0;
    int button_pressed=0;
    
      while(1)
    {     
        if ((PIND & (1<<PD6))==0)
        {
          _delay_ms(50);
          a=button_pressed + 1;
        }
        else {
        button_pressed=a;
           }   
       if(a==1)  
      {
        PORTA=led_on[0];
      }
      else if (a==2)
      {
        PORTA=led_on[1];
      }
      else if (a==3)
      {
        PORTA=led_on[2];
      }
      else if (a==4)
      {
        PORTA=led_on[3];
      } 
      else if (a==5)
      {
        PORTA=led_on[4];
      }
      else if (a==6)
      {
         PORTA=led_on[5];
      }
      else if (a==7)
      {
         PORTA=led_on[6];
      }
      else if (a==8)
      {
         PORTA=led_on[7];
      }
      else if (a==9)
      {
         PORTA=led_on[8];
      }
      else if (a==10)
      {
         button_pressed=0;
      }
      }
    }

#define F_CPU 8000000L  //ATtiny2313A @ 8MHz (internal oscillator)  
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "UART.h"

#define UART 1  //if you use UART serial, define UART
#define TERMINATOR '$'
#define VALUE 255 //PWM value : 0~255

int count=0;  //count of overflow (timer 0)
int state=0; //state of PWM pin( toggle-state) 
int value=0;  ////PWM value

//Interrupt Sub-Routine for timer 0 overflow interrupt
//when count value is 156, toggle state (duration 5 seconds)
ISR(TIMER0_OVF_vect)
{
  count++;
  if(count==156)
  {
    count=0;
    state=!state;
  }
  
  if(state)
  {
    OCR1A=value;
    OCR1B=0;
  }
  else
  {
    OCR1A=0;
    OCR1B=value;
  }
}

int main(void)
{
// if you use UART Serial
#ifdef UART
  int index=0;
  int process_data=0;
  unsigned char data;
  unsigned char buffer[10]="";
  
  UART_Init(103); // baud rate: 9600
#endif
  
  value=VALUE;  //PWM value
  
  // I/O set : PB3(pin15),PB4(pin16) OUTPUT
  // PB3->Output Compare Register OCR1A, PB4->OCR1B
  // They use Timer&Counter 1(TCNT1)
  DDRB|=(1<<PORTB3)|(1<<PORTB4);
  
  // set PWM, phase Correct Mode, 8-bit (TOP : 0x00FF) 
  TCCR1A=(1<<WGM10);
  
  // Set OC1A/OC1B on Compare Match when up-counting(clear when down-counting)
  TCCR1A|=(1<<COM1B1)|(1<<COM1A1);
  
  // clock select -> clk_io/8  (8 is prescaler) : PWM frequency 1.953kHz
  TCCR1B=(1<<CS11);
  
  // clock select -> clk_io/1024 (1024 is prescaler) for OVF interrupt
  TCCR0B|=(1<<CS02)|(1<<CS00);
  
  // enable timer 0 overflow interrupt 
  TIMSK|=(1<<TOIE0);
  // enable global interrupt (set I-bit in SREG(Status-Register)) 
  sei();
  
    while (1) 
    {
    
//if you use UART Serial
#ifdef UART
    data=UART_Receive();
    if(data==TERMINATOR)
    {
      buffer[index]='\0';
      process_data=1;
      /*serial debugging message */
      //UART_printString(buffer);
    }
    else
    {
      buffer[index++]=data;
    }
    
    if(process_data==1)
    {
      value=UART_Read(buffer);
      
      /*serial debugging message */
      UART_printString((unsigned char *)"conversion OK\n\0");
      UART_printString((unsigned char *)"PWM value :\0");
      UART_print8bitNumber(value);
      UART_Transmit('\n');
      UART_Transmit('\n');
      index=0;
      process_data=0;
    }
#endif  //end of communication loop
    }
    return 0;
}

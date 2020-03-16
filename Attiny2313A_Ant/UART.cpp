/*
 * UART.cpp
 *
 * Created: 2019-04-24 오후 6:53:56
 *  Author: orphe
 */ 


#include "UART.h"

void UART_Init(uint8_t baud)
{
	UBRRH=(unsigned char)(baud>>8);
	UBRRL=(unsigned char)baud;
	UCSRA|=(1<<U2X);
	UCSRB=(1<<RXEN)|(1<<TXEN);
	
	UCSRC=(3<<UCSZ0);
}

void UART_Transmit(unsigned char data)
{
	while(!(UCSRA&(1<<UDRE))) ;
	
	UDR=data;
}

unsigned char UART_Receive()
{
	while(!(UCSRA&(1<<RXC))) ;
	
	return UDR;
}

void UART_printString(unsigned char *str)
{
	int i;
	for(i=0;str[i];i++)
	{
		UART_Transmit(str[i]);
	}
}

void UART_print8bitNumber(uint8_t no)
{
	char numString[4]="0";
	int i,index=0;
	
	if(no>0)
	{
		for(i=0; no!=0; i++)
		{
			numString[i]=no%10+'0';
			no=no/10;
		}
		numString[i]='\0';
		index=i-1;
	}
	
	for(i=index;i>=0;i--)
	{
		UART_Transmit(numString[i]);
	}
}

void UART_print16bitNumber(uint16_t no)
{
	char numString[6]="0";
	int i,index=0;
	
	if(no>0)
	{
		for(i=0;no!=0;i++)
		{
			numString[i]=no%10+'0';
			no=no/10;
		}
		numString[i]='\0';
		index=i-1;
	}
	for(i=index;i>=0;i--)
	{
		UART_Transmit(numString[i]);
	}
}

void UART_print32bitNumber(uint32_t no)
{
	char numString[11]="0";
	int i,index=0;
	
	if(no>0)
	{
		for(i=0;no!=0;i++)
		{
			numString[i]=no%10+'0';
			no=no/10;
		}
		numString[i]='\0';
		index=i-1;
	}
	for(i=index;i>=0;i--)
	{
		UART_Transmit(numString[i]);
	}
}

int UART_Read(unsigned char *str)
{
	int index,ch=0;
	unsigned char k;
	for(index=0;k=str[index];index++)
	{
		ch=ch*10+(int)(k-'0');
	}
	
	/*serial debugging message */
	//UART_printString(message_read);
	
	return ch;
}
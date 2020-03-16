/*
 * UART.h
 *
 * Created: 2019-04-24 오후 2:25:04
 *  Author: orphe
 */ 
#ifndef _UART_H_
#define _UART_H_
#include <avr/io.h>

void UART_Init(uint8_t baud);
void UART_Transmit(unsigned char data);
unsigned char UART_Receive();
void UART_printString(unsigned char *str);
void UART_print8bitNumber(uint8_t no);
void UART_print16bitNumber(uint16_t no);
void UART_print32bitNumber(uint32_t no);
int UART_Read(unsigned char *str);


#endif /* UART_H_ */
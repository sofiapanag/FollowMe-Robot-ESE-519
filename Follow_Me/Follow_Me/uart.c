/*
 * uart.c
 *
 * Created: 10/8/2021 11:04:24 AM
 *  Author: Dennis
 */ 
#define F_CPU 16000000UL

// libraries
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <avr/io.h>
#include <assert.h>
#include "uart.h"

#define BUFFER_SIZE 1024

void initialize_UART(int baud_rate)
{
	int baud_prescaler = (F_CPU / (baud_rate * 16UL)) - 1; // see table 19.1 on datasheet
	// setting baud rate 9600 - UBRRn = 103
	UBRR0H = (unsigned char)(baud_prescaler>>8);
	UBRR0L = (unsigned char)baud_prescaler;
	// rx and tx
	UCSR0B = (1 << RXEN0) | (1 << TXEN0); // TX
	// 8 data bits
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
	// 2 stop bits
	UCSR0C |= (1 << USBS0);
}

void send_UART(unsigned char data)
{
	while(!(UCSR0A & (1 << UDRE0)));
	UDR0 = data;
}

void putstring_UART(char *strptr)
{
	while (*strptr)
	{
		send_UART(*strptr);
		strptr++;
	}
}


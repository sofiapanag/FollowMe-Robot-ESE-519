/*
 * Follow_Me.c
 *
 * Created: 11/27/2021 4:35:27 PM
 * Author : Dennis
 */ 
#define F_CPU 16000000UL
#include "util/delay.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "uart.h"

volatile unsigned long previous = 0;
volatile unsigned long current = 0; 
volatile unsigned long duration = 0;
volatile unsigned long distance = 0;
int IRleft, IRright, DirRight, DirLeft;

void intialize_PWM()
{
	cli();                  // disable global interrupts
    DDRD |= (1 << DDD3);      // set pin D3 as output (OC2B)
	
    // set timer prescaler to 1024
    TCCR2B |= (1 << CS20);
    TCCR2B |= (1 << CS21);
    TCCR2B |= (1 << CS22);
	
    // set timer mode to Fast PWM counting up to TOP
    TCCR2A |= (1 << WGM20);
    TCCR2A |= (1 << WGM21);
    TCCR2B &= ~(1 << WGM22);
		
    // clear OC2B on compare match, set OC2B at Bottom
    TCCR2A &= ~(1 << COM2B0);
    TCCR2A |= (1 << COM2B1);
	
    OCR2A = 195;    // freq of 20Hz for the motors, OCR0A=16MHz/(4*1024*20)
    OCR2B = 0x7F;    // duty cycle (speed)
	
	sei();                  // enable global interrupts
}


void initialize() {
    cli();                  // disable global interrupts
    //INPUT OF ARDUINO: ULTRASONIC
    // initialize ultrasonic ports
    DDRD |= (1<<DDD7);      // set pin D7 as output
    DDRB &= ~(1<<DDB0);     // set pin B0 as input (input capture timer 1)
    PORTB |= (1<<PORTB0);   // enable internal pull up resistor

    // initialize ultrasonic timer 1
    TCCR1B &= ~(1<<CS10);   // set timer prescaler to 8
    TCCR1B |= (1<<CS11);
    TCCR1B &= ~(1<<CS12);
    // input capture configurations
    TIMSK1 |= (1<<ICIE1);   // enable input capture interrupt
    TCCR1B |= (1<<ICES1);   // detecting rising edge
    TIFR1 |= (1<<ICF1);     // clear input capture flag

    //OUTPUT OF ARDUINO: LEFT MOTORS
    DDRD |= (1 << DDD5);      // set pin D5 as output (OC0B)
    // set timer prescaler to 1024
    TCCR0B |= (1 << CS00);
    TCCR0B &= ~(1 << CS01);
    TCCR0B |= (1 << CS02);
    
	// set timer mode to Fast PWM counting up to TOP
    TCCR0A |= (1 << WGM00);
    TCCR0A &= ~(1 << WGM01);
    TCCR0B &= ~(1 << WGM02);
	
    // clear OC0B on compare match, set OC0B at Bottom
    TCCR0A &= ~(1 << COM0B0);
    TCCR0A |= (1 << COM0B1);
    OCR0A = 195;    // freq of 20Hz for the motors, OCR0A=16MHz/(4*1024*20)
    OCR0B = 0x7F;    // duty cycle (speed)
	
    sei();                  // enable global interrupts
}

ISR(TIMER1_CAPT_vect) {     // ultrasonic
	TIFR1 |= (1<<ICF1);     // clear input capture flag
	current = ICR1;
	if (!(TCCR1B & (1<<ICES1))) {   // if detecting falling edge
		if (current > previous) {
			duration = current - previous;
		}
		if (current < previous) {
			duration = 65536 + current - previous;
		}
		distance = duration/(2*58); // in cm
	}
	previous = current;
	TCCR1B ^= (1<<ICES1);   // toggle detecting edge
}



void GoForward () {
    DirLeft = 1;
    DirRight = 1;
    OCR0B = 120;
    OCR2B = 120;
}

void GoBackward() {
    DirLeft = 0;
    DirRight = 0;
    OCR0B = 120;
    OCR2B = 120;
}

void GoRight() {
    DirLeft = 1;    // motors 1,2
    DirRight = 0;   // motors 3,4
    OCR0B = 200;
    OCR2B = 100;
}

void GoLeft() {
    DirLeft = 0;
    DirRight = 1;
    OCR0B = 100;
    OCR2B = 200;
}

void Stop() {
    DirLeft = 1;
    DirRight = 1;
    OCR0B = 0;
    OCR2B = 0;
}

int main (void) {
    const int  baud_rate = 9600;
	initialize_UART(baud_rate);
	intialize_PWM();
	initialize();
	
    while (1) {
        // ultrasonic trigger every 60ms
        PORTD |= (1<<PORTD7);
        _delay_ms(0.1);
        PORTD &= ~(1<<PORTD7);
        _delay_ms(60);
		
		char buffer[25] = {'\0'};
		sprintf(buffer,"distance %ld\n", distance);
		putstring_UART(buffer);
		
        if ((IRright == 1) && (IRleft == 1) && (distance > 10) && (distance < 30)) {
            GoForward();
        }
        else if ((IRright == 1) && (IRleft == 0)) {
            GoLeft();
        }
        else if ((IRright == 0) && (IRleft == 1)) {
            GoRight();
        }
        else if (((IRright == 1) && (IRleft == 1)) || ((distance > 5) && (distance < 10))){
            Stop();
        }
        else if (distance < 5) {
            GoBackward();
        }
    }
}






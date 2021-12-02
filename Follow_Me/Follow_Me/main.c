/*
 * Follow_Me.c
 *
 * Created: 11/27/2021 4:35:27 PM
 * Author : Dennis
 */
#define F_CPU 16000000UL
#include "util/delay.h"

#include "uart.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

volatile unsigned long previous = 0;
volatile unsigned long current = 0;
volatile unsigned long duration = 0;
volatile unsigned long distance = 0;
volatile unsigned long overflow = 0;
int IRleft, IRright, DirRight, DirLeft;


/************************************************************************/
/* initialize Timer 1 PWM                                                */
/************************************************************************/
void intialize_Timer1_PWM() {
  cli();               // disable global interrupts
  DDRD |= (1 << DDD3); // set pin D3 as output (OC2B)

  // set timer prescaler to 1024
  TCCR2B |= (1 << CS20);
  TCCR2B |= (1 << CS21);
  TCCR2B |= (1 << CS22);

  // set phase correct
  TCCR2A |= (1 << WGM20);
  TCCR2A &= ~(1 << WGM21);
  TCCR2B |= (1 << WGM22);

  // clear OC2B on compare match, set OC2B at Bottom
  TCCR2A &= ~(1 << COM2B0);
  TCCR2A |= (1 << COM2B1);

  OCR2A = 255;  // freq of 20Hz for the motors, OCR0A=16MHz/(2*1024*30)
  OCR2B = OCR2A/2; // 50 % duty cycle
  sei(); // enable global interrupts
}

/************************************************************************/
/* initialization of Timer 0 PWM										*/
/************************************************************************/
void intialize_Timer0_PWM() {
	cli();
	//OUTPUT OF ARDUINO: LEFT MOTORS
	DDRD |= (1 << DDD5); // set pin D5 as output (OC0B)
	
	// set timer prescaler to 1024
	TCCR0B |= (1 << CS00);
	TCCR0B &= ~(1 << CS01);
	TCCR0B |= (1 << CS02);

	// phase correct PWM
	TCCR0A |= (1 << WGM00);
	TCCR0A &= ~(1 << WGM01);
	TCCR0B |= (1 << WGM02);

	// clear OC0B on compare match, set OC0B at Bottom
	TCCR0A &= ~(1 << COM0B0);
	TCCR0A |= (1 << COM0B1);
	OCR0A = 255;  // freq of 20Hz for the motors, OCR0A=16MHz/(2*1024*20)
	OCR0B = OCR0A/2; // 50 % duty cycle
	sei();
}

/************************************************************************/
/*   //INPUT OF ARDUINO: ULTRASONIC										*/                                                                    
/************************************************************************/

void initialize() {
  cli(); // disable global interrupts

  // initialize ultrasonic ports
  DDRD |= (1 << DDD7);    // set pin D7 as output
  DDRB &= ~(1 << DDB0);   // set pin B0 as input (input capture timer 1)
  PORTB |= (1 << PORTB0); // enable internal pull up resistor

  // initialize ultrasonic timer 1
  TCCR1B &= ~(1 << CS10); // set timer prescaler to 8
  TCCR1B |= (1 << CS11);
  TCCR1B &= ~(1 << CS12);
  
  // input capture configurations
  TIMSK1 |= (1 << ICIE1); // enable input capture interrupt
  TCCR1B |= (1 << ICES1); // detecting rising edge
  TIFR1 |= (1 << ICF1);   // clear input capture flag
  TIFR1 |= (1 << TOV1);  // overflow flag   
 
  sei(); // enable global interrupts
}

/************************************************************************/
/* Ultrasonic ISR.														*/
/* TODO: see how to modify overflow variable 									*/
/************************************************************************/

ISR(TIMER1_CAPT_vect) { // ultrasonic
  
  if ((TCCR1B & (1 << ICES1))) { // rising edge -- setup dot and dash
	  overflow = 0;
	  TIMSK1 |= (1 << TOIE1); // overflow interrupt is -- TOV1 flag
  }
  
  TIFR1 |= (1 << ICF1); // clear input capture flag
  current = ICR1;
  
  if (!(TCCR1B & (1 << ICES1))) { // if detecting falling edge
    if (current > previous) {
      duration = current - previous;
    } 
	if (current < previous) {
      duration = 65536 + current - previous;
    } 
	
    distance = duration / (2 * 58); // in cm
  }
  previous = current;
  TCCR1B ^= (1 << ICES1); // toggle detecting edge
}

/**
 * TODO: ISR overflow
 **/
ISR(TIMER1_OVF_vect) 
{
	overflow++;
}

/*
volatile unsigned long calculate_duration(long unsigned edge1, long unsigned edge2, long overflow) {
	volatile unsigned long ticks = 0;
	if (edge1 < edge2)
		ticks = (overflow * (unsigned long)BIT_16_VAL) + (edge2 - edge1);
	else if (edge1 == edge2)
		ticks = (overflow * (unsigned long)BIT_16_VAL);
	else
		ticks = (overflow - 1) * (unsigned long)BIT_16_VAL + ((unsigned long)BIT_16_VAL - edge1) + edge2;

	return (ticks * (unsigned long)CONVERT_TO_MICROSECONDS) / factor;
}
*/
/* setting up interrupt
	TIFR1 |= (1 << ICF1);   // clear input capture flag
	TIFR1 |= (1 << TOV1);	// clear overflow flag
	TCCR1B |= (1 << ICES1);	// trigger on rising edge
	TIMSK1 |= (1 << ICIE1); // enable input capture interrupt
*/
/*
1) integrate overflow in calculations
2) reset overflow in falling edge
if ((TCCR1B & (1 << ICES1))) { // rising edge -- setup dot and dash
    overflow = 0;
    edge1 = ICR1;
    TIMSK1 |= (1 << TOIE1); // overflow interrupt is -- TOV1 flag
    
} 
 * */

void GoForward() {
  DirLeft = 1;
  DirRight = 1;
  OCR0B = 120;
  OCR2B = 120;
}

void GoBackward() {
  DirLeft = 0;
  DirRight = 0;
  // changed this to 50% duty cycle for testing. 
  OCR0B = OCR0A/2;
  OCR2B = OCR2A/2;
}

void GoRight() {
  DirLeft = 1;  // motors 1,2
  DirRight = 0; // motors 3,4
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

int main(void) {
  const int baud_rate = 9600;
  initialize_UART(baud_rate);
  intialize_Timer0_PWM();
  intialize_Timer1_PWM();
  initialize();

  while (1) {
    // ultrasonic trigger every 60ms
    PORTD |= (1 << PORTD7);
    _delay_ms(0.1);
    PORTD &= ~(1 << PORTD7);
    _delay_ms(60);

    char buffer[25] = {'\0'};
    sprintf(buffer, "distance %ld, overflow %ld\n", distance, overflow);
    putstring_UART(buffer);

    if ((IRright == 1) && (IRleft == 1) && (distance > 10 && distance < 30)) {
      GoForward();
    } else if ((IRright == 1) && (IRleft == 0)) {
      GoLeft();
    } else if ((IRright == 0) && (IRleft == 1)) {
      GoRight();
    } else if (((IRright == 1) && (IRleft == 1)) && ((distance > 5) && (distance < 10))) {
      Stop();
    } else if (distance < 5) {
      GoBackward();
    }
  }
}

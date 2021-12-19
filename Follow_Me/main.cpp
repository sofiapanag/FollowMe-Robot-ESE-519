/*
 * Follow_Me.c
 *
 * Created: 11/27/2021 4:35:27 PM
 * Author : Dennis
 */
#define F_CPU 16000000UL
#include "util/delay.h"

#include "myuart.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

volatile unsigned long previous = 0;
volatile unsigned long current = 0;
volatile unsigned long duration = 0;
volatile unsigned long distance = 0;
volatile unsigned long overflow = 0;
char buffer[25] = {'\0'};

/************************************************************************/
/* initialization of Timer 0 PWM										*/
/************************************************************************/
void intialize_Timer0_PWM() {
    cli();
    //OUTPUT OF ARDUINO: SERVO
    DDRD |= (1 << DDD5); // set pin D5 as output (OC0B)

    // set timer prescaler to 1024
    TCCR0B |= (1 << CS00);
    TCCR0B &= ~(1 << CS01);
    TCCR0B |= (1 << CS02);

    // phase correct PWM counting up to TOP
    TCCR0A |= (1 << WGM00);
    TCCR0A &= ~(1 << WGM01);
    TCCR0B &= ~(1 << WGM02);

    // clear OC0B on compare match, set OC0B at Bottom
    TCCR0A &= ~(1 << COM0B0);
    TCCR0A |= (1 << COM0B1);
    OCR0B = 0;
    sei();

    cli();
}


/************************************************************************/
/* initialization of Timer 1 PWM										*/
/************************************************************************/
void intialize_Timer1_PWM() {
    cli();
    //INPUT OF ARDUINO: ULTRASONIC
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

    sei();
}

/************************************************************************/
/* initialize Timer 2 PWM                                                */
/************************************************************************/
void intialize_Timer2_PWM() {
  cli();               // disable global interrupts
    //OUTPUT OF ARDUINO: MOTORS
  DDRD |= (1 << DDD3); // set pin D3 as output (OC2B) RIGHT
  DDRB |= (1 << DDB3); // set pin D3 as output (OC2A) LEFT

  // set timer prescaler to 1024
  TCCR2B |= (1 << CS20);
  TCCR2B |= (1 << CS21);
  TCCR2B |= (1 << CS22);

  // set phase correct counting up to TOP
  TCCR2A |= (1 << WGM20);
  TCCR2A &= ~(1 << WGM21);
  TCCR2B &= ~(1 << WGM22);

  // clear OC2B on compare match, set OC2B at Bottom
  TCCR2A &= ~(1 << COM2B0);
  TCCR2A |= (1 << COM2B1);

  // clear OC2A on compare match, set OC2A at Bottom
  TCCR2A &= ~(1 << COM2A0);
  TCCR2A |= (1 << COM2A1);

  OCR2A = 0;  // duty cycle of left wheels (speed)
  OCR2B = 0; // duty cycle of right wheels (speed)
  sei(); // enable global interrupts
}

/************************************************************************/
/*   //INPUT OF ARDUINO: ULTRASONIC										*/                                                                    
/************************************************************************/

void initialize() {
  cli(); // disable global interrupts

  DDRD &= ~(1 << DDD6);    // set pin D6 as input (Start/Stop)

  // IR Sensors
  DDRB &= ~(1 << DDB4);    // set pin B4 as input (left IR sensor)
  DDRB &= ~(1 << DDB5);    // set pin B5 as input (right IR sensor)

  // motors direction
  DDRD |= (1 << DDD2);    // set pin D2 as output (right)
  DDRD |= (1 << DDD4);    // set pin D4 as output (left)
 
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
    PORTD |= (1 << PORTD4);   // direction of left wheels is forward
    PORTD &= ~(1 << PORTD2);   // direction of right wheels is forward
    OCR2A = 120;
    OCR2B = 120;
}

void GoBackward() {
    PORTD &= ~(1 << PORTD4);   // direction of left wheels is backward
    PORTD |= (1 << PORTD2);   // direction of right wheels is backward
    OCR2A = 120;
    OCR2B = 120;
}

void GoRight() {
    PORTD |= (1 << PORTD4);   // direction of left wheels is forward
    PORTD &= ~(1 << PORTD2);   // direction of right wheels is forward
    OCR2A = 220;
    OCR2B = 70;
}

void GoLeft() {
    PORTD |= (1 << PORTD4);   // direction of left wheels is forward
    PORTD &= ~(1 << PORTD2);   // direction of right wheels is forward
    OCR2A = 70;
    OCR2B = 220;
}

void Stop() {
    PORTD |= (1 << PORTD4);   // direction of left wheels is forward
    PORTD &= ~(1 << PORTD2);   // direction of right wheels is forward
    OCR2A = 0;
    OCR2B = 0;
}

int main(void) {
    int count = 0;
    UART_initialize();
    //intialize_Timer0_PWM(); // servo motor
    intialize_Timer1_PWM(); // ultrasonic
    intialize_Timer2_PWM(); // motors speed
    initialize();

    while (1) {
            // ultrasonic trigger every 60ms
            PORTD |= (1 << PORTD7);
            _delay_ms(0.1);
            PORTD &= ~(1 << PORTD7);
            _delay_ms(60);

            sprintf(buffer, "distance %ld, overflow %ld\n", distance, overflow);
            UART_putstring(buffer);

        if ((PIND & (1 << PIND6))) {
            _delay_ms(80);  // debouncing
            if ((PIND & (1 << PIND6))) {
                count++;
            }
        }
       if (count % 2) { // button press

           /*// servo code
           if ((PINB & (1 << PINB4)) && (PINB & (1 << PINB5))) { //if nothing is detected
               for (float i = 0; i < 20; i++) {
                   OCR0B++;     // duty cycle servo
                   _delay_ms(100);
               }
               _delay_ms(100);
               for (float i = 0; i < 20; i++) {
                   OCR0B--;     // duty cycle for servo
                   _delay_ms(100);
               }
               _delay_ms(100);
               OCR0B = 0;
           }
           else {
               OCR0B = 0;
           } */

            if ((!(PINB & (1 << PINB4))) && (!(PINB & (1 << PINB5))) && ((distance > 8) && (distance < 30))) {
                GoForward();
            }
            else if ((!(PINB & (1 << PINB5))) && (PINB & (1 << PINB4))) { // only right
                GoRight();
            }
            else if ((PINB & (1 << PINB5)) && (!(PINB & (1 << PINB4)))) { // only left
                GoLeft();
            }
            else if (distance < 5) {
                GoBackward();
            }
           else {
               Stop();
           }
        }
       else {
           OCR2A = 0;
           OCR2B = 0;
       }

    }
}

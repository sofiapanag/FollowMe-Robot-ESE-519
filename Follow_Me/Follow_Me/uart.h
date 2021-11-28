/*
 * uart.h
 *
 * Created: 10/8/2021 11:05:00 AM
 *  Author: Dennis
 */ 


#ifndef UART_H_
#define UART_H_

void initialize_UART(int baud_rate);
void putstring_UART(char *str);


#endif /* UART_H_ */
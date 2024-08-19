/*
 * AL304_display.h
 *
 * Created: 13.07.2024 11:47:06
 *  Author: drspa
 */ 


#ifndef AL304_DISPLAY_H_
#define AL304_DISPLAY_H_

#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>

#define DIG0 PORTA1
#define DIG1 PORTA2
#define DIG2 PORTA3
#define DIG3 PORTA7
#define BLUE_LED PORTB0
#define LOCK PORTB1

#define DUR_4SEC 31249
#define DUR_500MSEC 3905

void PCI0nterruptInint(void);
void AL304_init(void);
void AL304ShowNumber(int n0, int n1, int n2, int n3, int dur, uint8_t dot_en);
void AL304ShowTime(uint8_t min0, uint8_t min1, uint8_t hor0, uint8_t hor1);


#endif /* AL304_DISPLAY_H_ */
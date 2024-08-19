/*
 * counter.h
 *
 * Created: 13.07.2024 20:53:44
 *  Author: drspa
 */ 

#ifndef COUNTER_H_
#define COUNTER_H_

#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

typedef struct{
	uint8_t cnt;
	uint8_t ceil;
} Counter;

extern Counter cnt_min0;
extern Counter cnt_min1;
extern Counter cnt_hor0;
extern Counter cnt_hor1;

void INT0InterruptInit(void);
void INT0InterruptDeinit(void);
void PCI1nterruptInint(void);
void PCI1nteruuptDenint(void);
void counter_init(void);
void CounterTick(void);

#endif /* COUNTER_H_ */
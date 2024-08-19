/*
 * counter.c
 *
 * Created: 13.07.2024 20:54:06
 *  Author: drspa
 */ 

#include "counter.h"

Counter cnt_min0;
Counter cnt_min1;
Counter cnt_hor0;
Counter cnt_hor1;

uint8_t tick_flag;
uint8_t input_reg;

void INT0InterruptInit(void)
{
    MCUCR |= (1 << ISC01);
    GIMSK |= (1 << INT0);
}

void INT0InterruptDeinit(void)
{
    GIMSK &= ~(1 << INT0);
    MCUCR &= ~(1 << ISC01);
}

void PCI1nterruptInint(void)
{
    GIMSK |= (1 << PCIE1);
    PCMSK1 |= (1 << PCINT10);
}

void PCI1nteruuptDenint(void)
{
    GIMSK &= ~(1 << PCIE1);
    PCMSK1 &= ~(1 << PCINT10);
}

void counter_init(void)
{
    tick_flag = 0;
    
	cnt_min0.cnt = 0;
	cnt_min0.ceil = 10;
	
	cnt_min1.cnt = 0;
	cnt_min1.ceil = 6;
	
	cnt_hor0.cnt = 0;
	cnt_hor0.ceil = 10;
	
	cnt_hor1.cnt = 0;
	cnt_hor1.ceil = 3;
}

void CounterTick(void)
{
    if(!tick_flag) return;
    
    cnt_min0.cnt ++;
    
    cnt_min1.cnt += cnt_min0.cnt / cnt_min0.ceil;
    cnt_min0.cnt %= cnt_min0.ceil;
    
    cnt_hor0.cnt += cnt_min1.cnt / cnt_min1.ceil;
    cnt_hor1.cnt == 2 ? (cnt_hor0.ceil = 4) : (cnt_hor0.ceil = 10);
    cnt_min1.cnt %= cnt_min1.ceil;
    
    cnt_hor1.cnt += cnt_hor0.cnt / cnt_hor0.ceil;
    cnt_hor0.cnt %= cnt_hor0.ceil;
    
    cnt_hor1.cnt %= cnt_hor1.ceil;
    
    tick_flag = 0;
}

// wake-up from clock tick interrupt
ISR(PCINT1_vect)
{
    GIMSK &= ~(1 << PCIE1);
    PCMSK1 &= ~(1 << PCINT10);
}

// tick count interrupt
ISR(EXT_INT0_vect)
{
    tick_flag = 1;
    GIFR |= (1 << INTF0);
    GIMSK &= ~(1 << INT0);
    MCUCR &= ~(1 << ISC01);
}

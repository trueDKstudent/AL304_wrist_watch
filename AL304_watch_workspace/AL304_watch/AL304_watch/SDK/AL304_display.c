/*
 * AL304_display.c
 *
 * Created: 13.07.2024 11:47:25
 *  Author: drspa
 */ 

#include <AL304_display.h>
#include <avr/pgmspace.h>
#include "tinySPI.h"

const uint8_t Num_table[] PROGMEM = {
	0b11000000,	//0
	0b11111001,	//1
	0b10000011,	//2
	0b10100001,	//3
	0b10111000,	//4
	0b10100100,	//5
	0b10000100,	//6
	0b11101001,	//7
	0b10000000,	//8
	0b10100000,	//9
	0b01111111,	//.
    0b11111111  //space
};

uint8_t display_flag; //indicates the end of delay time
uint8_t display_en; //indicates if button was pressed

void PCI0nterruptInint(void)
{
    GIMSK |= (1 << PCIE0);
    PCMSK0 |= (1 << PCINT6);
}

void TimerSetDuration(int duration)
{
    OCR1A = duration;
}

static void TimerInit(void)
{
    TCNT1 = 0;
    TimerSetDuration(DUR_4SEC);
    TIMSK1 |= (1 << OCIE1A);
    TCCR1B |= (1 << WGM12);
}

void TimerStart(void)
{
    TCCR1B |= (1 << CS12) | (1 << CS10); //Start Timer1
}

void TimerStop(void)
{
    TCCR1B &= ~((1 << CS12) | (1 << CS10)); //Stop Timer1
}

static void AL304_clear_register(void)
{
    PORTB &= ~(1 << LOCK);
    tinySPI_transfer(0x00);
    PORTB |= (1 << LOCK);
}

void AL304_init(void)
{
    display_flag = 0;
    display_en = 0;
	tinySPI_begin();
	tinySPI_setDataMode(SPI_MODE0);
    TimerInit();
	
	DDRA |= (1 << DIG0) | (1 << DIG1) | (1 << DIG2) | (1 << DIG3);
	DDRB |= (1 << BLUE_LED) | (1 << LOCK);
    
    AL304_clear_register();
}

static void AL304_send_number(uint8_t num, uint8_t dig)
{
	PORTA &= ~((1 << DIG0) | (1 << DIG1) | (1 << DIG2) | (1 << DIG3));
	PORTA |= (1 << dig);
	
	PORTB &= ~(1 << LOCK);
	tinySPI_transfer(pgm_read_byte(&Num_table[num]));
	PORTB |= (1 << LOCK);
}

void AL304ShowNumber(int n0, int n1, int n2, int n3, int dur, uint8_t dot_en)
{
    TimerSetDuration(dur);
    TimerStart();
    while (!display_flag) {
        PORTB |= (1 << BLUE_LED); // Turn-on backlight
        AL304_send_number(n0, DIG0);
        _delay_ms(2);
        AL304_send_number(n1, DIG1);
        _delay_ms(2);
        AL304_send_number(n2, DIG2);
        _delay_ms(2);
        AL304_send_number(n3, DIG3);
        _delay_ms(2);
        
        if(dot_en) {
        AL304_send_number(10, DIG2); //point
        _delay_ms(2);
        }
        
        AL304_clear_register();
        
        PORTB &= ~(1 << BLUE_LED); // Turn-off backlight
        PORTA &= ~((1 << DIG0) | (1 << DIG1) | (1 << DIG2) | (1 << DIG3));
    }
    display_flag = 0;
    TimerStop();
}

void AL304ShowTime(uint8_t min0, uint8_t min1, uint8_t hor0, uint8_t hor1)
{
    if(!display_en) return;
    
    AL304ShowNumber(min0, min1, hor0, hor1, DUR_4SEC, 1);
    
    display_en = 0;
}

// delay timer
ISR(TIM1_COMPA_vect)
{
    display_flag = 1;
}

// wake-up from button press interrupt
ISR(PCINT0_vect)
{
    display_en = 1;
    GIMSK &= ~(1 << PCIE0);
    PCMSK0 &= ~(1 << PCINT6);
}

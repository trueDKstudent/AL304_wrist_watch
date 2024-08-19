/*
 * ADC.h
 *
 * Created: 05.08.2024 13:56:58
 *  Author: drspa
 */ 


#ifndef ADC_H_
#define ADC_H_
#include <avr/io.h>

enum{
    MUXCH0,
    MUXCH1,
    MUXCH2,
    MUXCH3,
    MUXCH4,
    MUXCH5,
    MUXCH6,
    MUXCH7
};

typedef enum{
    BUT_SLEEP,
    BUT_SW_CHRG,
    BUT_SET_TIME
} Button_states;

void ADCSetMuxChannel(uint8_t adc_channel);
void ADCinit(void);
void ADCDeinit(void);
void ADCStart(void);
int ADCGetValue(void);
Button_states ADCGetButtonState(void);


#endif /* ADC_H_ */
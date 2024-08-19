/*
 * ADC.c
 *
 * Created: 05.08.2024 13:56:46
 *  Author: drspa
 */ 

#include "ADC.h"

void ADCSetMuxChannel(uint8_t adc_channel)
{
    ADMUX &= 0xC0; //clear mux channel selection bits
    ADMUX |= adc_channel;
}

void ADCinit(void)
{
    ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
    DIDR0 |= (1 << ADC6D);
    ADCSRA |= (1 << ADEN) | (1 << ADATE);
}

void ADCDeinit(void)
{
    ADMUX &= 0xC0; //clear mux channel selection bits
    ADCSRA &= ~((1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0));
    DIDR0 &= ~(1 << ADC6D);
    ADCSRA &= ~((1 << ADEN) | (1 << ADATE));
}

void ADCStart(void)
{
    ADCSRA |= (1 << ADSC);
}

int ADCGetValue(void)
{
    int val_l, val_h;
    while(!(ADCSRA && (1 << ADIF))) {}
    ADCSRA |= (1<<ADIF);
    val_l = ADCL;
    val_h = ADCH;
    val_l |= val_h << 8;
    return val_l;
}

Button_states ADCGetButtonState(void)
{
    Button_states state = BUT_SLEEP;
    int val1, val2;
    
    val1 = ADCGetValue();
    val2 = ADCGetValue();
    
    val1 = (val1 + val2) / 2;
    val2 = val1 - 512;
    
    if(val2 < 50 &&
       val2 > -50){
        state = BUT_SET_TIME;
    }else if(val2 < 50 &&
             val2 < -50){
        state = BUT_SW_CHRG;
    }else if(val2 > 50 &&
             val2 > -50){
        state = BUT_SLEEP;
    }
       
    return state;
}

uint8_t ButtonSettingsEn(void)
{
    uint8_t en_flag;
    int val1, val2;
    
    val1 = ADCGetValue();
    val2 = ADCGetValue();
    
    val1 = (val1 + val2) / 2;
    
     if((val1 - 512) < 50 &&
        (val1 - 512) > -50){
         en_flag = 1;
     }else{
         en_flag = 0;
     }
     
     return en_flag;
}

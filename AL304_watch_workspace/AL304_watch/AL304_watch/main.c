/*
 * AL304_watch.c
 *
 * Created: 12.07.2024 20:19:41
 * Author : drspa
 */ 
#include <avr/io.h>
#include <avr/sleep.h>
#include <AL304_display.h>
#include <counter.h>
#include <ADC.h>

#define CHARGE_MAX 651
#define CHARGE_MIN 512

void TaskShowTime(void)
{
    AL304ShowTime(cnt_min0.cnt,
                  cnt_min1.cnt,
                  cnt_hor0.cnt,
                  cnt_hor1.cnt);
    CounterTick();
}

void TaskShowCharge(void)
{
    int chrg_val;
    int cd0, cd1, cd2;
    
    ADCDeinit();
    ADCinit();
    ADCStart();
    
    _delay_ms(2); //wait 25 ADC clock cycles
                  //in order to initialize analog circuitry
    
    chrg_val = ADCGetValue();
    
    if(chrg_val < CHARGE_MIN) chrg_val = CHARGE_MIN;
    else if(chrg_val > CHARGE_MAX) chrg_val = CHARGE_MAX;
    
    chrg_val = 100* (chrg_val - CHARGE_MIN)/
                    (CHARGE_MAX - CHARGE_MIN);
    
    cd0 = chrg_val % 10;
    chrg_val /= 10;
    cd1 = chrg_val % 10;
    if(cd1 == 0) cd1 = 11;
    
    chrg_val /= 10;
    cd2 = chrg_val %10;
    if(cd2 == 0) cd2 = 11;
    
    AL304ShowNumber(cd0, cd1, cd2, 11, DUR_4SEC, 0);
    CounterTick();
}

void TaskSetTime(void)
{
    int hor = 0;
    int min = 0;
    uint8_t set_cnt = 0;
    uint8_t dot_blink = 0;
    
    while(set_cnt < 2) {
        switch(set_cnt){
            case 0:
            if(ADCGetButtonState() == BUT_SW_CHRG) min++;
            min %= 60;
            AL304ShowNumber(min%10, min/10, 11, 11, DUR_500MSEC, dot_blink);
            break;
            case 1:
            if(ADCGetButtonState() == BUT_SW_CHRG) hor++;
            hor %= 24;
            AL304ShowNumber(11, 11, hor%10, hor/10, DUR_500MSEC, dot_blink);
            break;
        }
        dot_blink ^= 1;
        if(ADCGetButtonState() == BUT_SET_TIME) set_cnt++;
    }
    
    cnt_min0.cnt = min % 10;
    cnt_min1.cnt = min / 10;
    cnt_hor0.cnt = hor % 10;
    cnt_hor1.cnt = hor / 10;
}

int main(void)
{
    Button_states button_state;
	AL304_init();
    PCI0nterruptInint();
    PCI1nterruptInint();
	counter_init();
	
    sei();
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    
    while (1) 
    {
        sleep_mode();
        INT0InterruptInit();
        
        ADCSetMuxChannel(MUXCH6);
        ADCinit();
        ADCStart();
        
        _delay_ms(2); //wait 25 ADC clock cycles 
                      //in order to initialize analog circuitry
        
		TaskShowTime();
        
        button_state = ADCGetButtonState();
        if(button_state == BUT_SW_CHRG){
            TaskShowCharge();
        }else if(button_state == BUT_SET_TIME){
            TaskSetTime();
        }else if(button_state == BUT_SLEEP){
            //do nothing
        }
        ADCDeinit();
        
        PCI1nterruptInint();
        PCI0nterruptInint();
    }
}

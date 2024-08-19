## Wrist watch on AL304 indicators

![watch](/Images/main_pic.jpg)

Hello everyone, in this article I’d like to show you my wrist watch on
AL304G LED indicators. When I first saw these indicators I immediately
imagined them hanging on the wires in air and thought that it would be 
a great idea to make watch on these indicators using hinged installation 
method.

Simplified circuit diagram for the AL304 watch is shown on figure 1.

![Fig. 1](/Images/Circuit.png)
**Figure 1.**

At first I did not have a clear idea on how schematic for this watch 
should look like. The very first thing which I did was soldering four
indicators together into one four-digit display. Later I added resistor 
assembly with serial to parallel shift register in order to conviniently 
controll the display since the MCU that I used did not have enought 
outputs to manipulate display on its own (figure 2). 

![Fig. 2](/Images/proto_view2.png)
**Figure 2.**

As an MCU I used ATtiny44A because it has a very small package and also
because it was the only spare MCU which I had at that moment and as it
turned out in the future it was a perfect choise. After I seteled down 
with display and MCU it was time to decide how will my watch measure time.
There were two options for me:
* First one is to use RTC chip like DS1337, let
it measure time and read it via I2C bus. But such soltion was not suitable 
in this case because TWI pins for I2C bus is already taken by SPI bus for 
shift register. Theoretically it is possible to multiplex TWI bus and switch
between SPI and I2C interfaces but it will complicate circuit and programming
(TWI pins are also used to programm MCU). 
* Second one is more old fashioned and quirky - it is clock generator which 
generates short voltage pulse each minute. Generators circuit consists of 2Hz
generator on CD4060 and frequency divider on CD4024 which divides input 2Hz 
frequncy by 120 and in result we get exactly "1 minute" frequency clock. 

"One minute" clock goes to MCU's EXTI0 pin. EXTI0 pin can be configured to
call corresponding interrup on falling edge of external signal which in this
case is clock signal. Reaction only on falling edge is helpful because MCU 
should count only one minute per one clock pulse. If level change interrupt is 
used (like PCINT1) interrupt routine will be called twice which means that MCU
will count two minutes pre one clock pulse! Instead PCINT1 interrupt is used to 
wake-up MCU from POWER_DOWN mode under which it consumes the least current.

Another interrupt PCINT0 is used to both wake-up MCU and show time, battery
charge and set the time if nessesary. Two latter functions can be chosen by 
pressing specific combinations of two buttons. Two buttons are distiguished
by MCU by voltage level that both buttons generates after it was pressed. 
Voltage levels are readen by MCU's ADC. If YELLOW button is presed once and
released watch will show only time. If YELLOW button is pressed and held for 
fout seconds watch will show battery charge. If GREEN button is pressed and held
immediately after YELLOW button was pressed a set time mode is activated.

Battery charge is also measured with MCU's ADC. When option to display battery
charge is selected ADC input is switched from channel ADC6 to ADC0 to which
resistive divider is connected. Divider is activated only when YELLOW button
is pressed otherwise battery voltage is cut from divider and it does not 
consume current. 

Entire watch is powered by LIR2032 Li-Ion battery with capacity of 40mA/h. 
Battery is protected from short circuit, overcharge and underdischarge and 
can be charged from type-C USB. In sleep mode watch consumes approximately 
10 uA.

Down below are more photos of finished watch. 

![Fig. 3](/Images/pic1.jpg)
![Fig. 4](/Images/pic2.jpg)
![Fig. 5](/Images/pic3.jpg)
![Fig. 6](/Images/pic4.jpg)
![Fig. 7](/Images/pic5.jpg)
![Fig. 8](/Images/pic6.jpg)
![Fig. 9](/Images/night_mode.jpg)

**Figure 3.**

It also can be seen that on 4-th digit segment "b" does not emmit light. 
Unfortunatelly it was burned durring soldering process when I was soldering 
main circuit to watch's frame. Maybe in the future I will replace that 
indicator but for now it is fine :)

Also despite this project is called "wrist watch" I still did not figure out
how to fix them on my wrist. Once I was suggested to pour entire watch in epoxy
resing but I do not what to risk so much. Instead it would be better to make
a clear case for this watch, so stay tuned for updates!

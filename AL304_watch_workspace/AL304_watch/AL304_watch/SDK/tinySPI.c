// Arduino tinySPI Library Copyright (C) 2018 by Jack Christensen and
// licensed under GNU GPL v3.0, https://www.gnu.org/licenses/gpl.html
//
// Arduino hardware SPI master library for
// ATtiny24/44/84, ATtiny25/45/85, ATtiny261/461/861, ATtiny2313/4313.
//
// https://github.com/JChristensen/tinySPI
// Jack Christensen 24Oct2013

#include <tinySPI.h>

void tinySPI_begin()
{
    USICR &= ~ ((1 << USISIE) | (1 << USIOIE) | (1 << USIWM1));
    USICR |= (1 << USIWM0) | (1 << USICS1) | (1 << USICLK);
    SPI_DDR_PORT |= (1 << USCK_DD_PIN);   // set the USCK pin as output
    SPI_DDR_PORT |= (1 << DO_DD_PIN);     // set the DO pin as output
    SPI_DDR_PORT &= ~(1 << DI_DD_PIN);    // set the DI pin as input
}

void tinySPI_setDataMode(uint8_t spiDataMode)
{
    if (spiDataMode == SPI_MODE1)
        USICR |= (1 << USICS0);
    else
        USICR &= ~(1 << USICS0);
}

uint8_t tinySPI_transfer(uint8_t spiData)
{
    USIDR = spiData;
    USISR = (1 << USIOIF);                // clear counter and counter overflow interrupt flag
    //ATOMIC_BLOCK(ATOMIC_RESTORESTATE)   // ensure a consistent clock period
    {
        while ( !(USISR & (1 << USIOIF)) ) USICR |= (1 << USITC);
    }
    return USIDR;
}

void tinySPI_end()
{
    USICR &= ~((1 << USIWM1) | (1 << USIWM0));
}


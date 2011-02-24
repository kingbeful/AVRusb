#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>  /* for sei() */
#include <util/delay.h>     /* for _delay_ms() */
#include <avr/eeprom.h>

#include <avr/pgmspace.h>
#include "spi.h"
#include "phy_A7105.h"
//#ifdef DEBUG
//#include <stdio.h>
//#endif
int main(void)
{
//    #ifdef DEBUG
//    printf ("Start the Main Function\n");
//    #endif
    wdt_disable();
    // Insert code
    RF_init();
//    SPI_MasterTransmit(data);
    while(1)
    ;

    return 0;
}

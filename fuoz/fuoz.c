/*
 * fuoz.c
 *	prowivka ras4itana na tranzistornij kommutator v moem slu4ae eto bil prosto tranzistor darlingtona BDX33C
 *  Atmega8 8MHz
 *	Attiny2313 8MHz
 *  Author: OFPR
 *  Main file
 */ 

#include "config.h"
#include <util/delay.h>



int main(void)
{
	fuoz_init();
	LED_DDR |= LED_PIN;	
	
    while(1)
    {
		LED_ON;
		_delay_ms(1000);
		LED_OFF;
		_delay_ms(1000);
		// sjuda mozhno hotj LCD prisoba4itj toljko neljzja zaprewatj prerivanija na dliteljnoe vremja          
    }
}








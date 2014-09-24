/*
* Vse nastrojki tut.
* TCI - Transistor controlled ignition (tranzistornoe zazhiganie)
* CDI - Capacitor discharge ignition (tiristornoe zazhiganie)
* Rabotaet toljko s  (__AVR_ATmega8__) i (__AVR_ATtiny2313__)
*
*
*
*
*
*/


#ifndef CONFIG_H
#define CONFIG_H


#include <avr/io.h>

#define F_CPU 8000000L


#define Type 0 //  0-TCI 1-CDI

/*
	Interrupt_MODE:
	0 - INT0 Rising
	1 - INT0 Falling
	2 - INT1 Rising
	3 - INT1 Falling
	4 - Comparator interrupt on rising (not used)
	5 - Comparator interrupt on falling (not used)
*/
#define Interrupt_MODE 0
#define Thermistor 0 // 1-Thermistor







//===============================================================================================================
#if Thermistor != 0
	#include "term.h"
#endif


#include "formirovatelj.h"

#endif

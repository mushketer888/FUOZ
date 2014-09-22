#include <avr/interrupt.h>
#include "config.h"

#define PIN_DDR  DDRB |= 1<<5
#define PIN_H PORTB |= 1<<5
#define PIN_L PORTB &= ~(1<<5)
const int Nakoplenie = 375; //3ms (pri 4ms iskra zhet bumagu no i potrebljaet energii sootvetstvenno, bolee 4ms katuwka nahoditsja v
							// nasiwenii mowjnostj iskri budet padatj)
const int Gorenie = 100;	//vremja gorenija benzina do stenki cilingra 800us (odin tik tajmera 8us) '650us


//=====================================================================================================================

unsigned int vrema = 0;
unsigned int tmp =0;
unsigned int zar = 0;
//unsigned int uskorenie = 0;
//unsigned int delta = 0;

void fuoz_init(void)
{
	//========init============
	#if (__AVR_ATtiny2313__) || (__AVR_ATmega8__)
		cli();
		TIFR |= 1<<OCF1A|1<<OCF1B;
		TIMSK |= 1<<OCIE1A|1<<OCIE1B|1<<TOIE1;//prerivanija tajmera ih tut visit azh 3 wtuki :D
		#if Interrupt_MODE == 0 //int0 rising
			MCUCR |= 1<<ISC00|1<<ISC01; 
			GIMSK = 1<<INT0;//vnewnee prerivanie
		#elif Interrupt_MODE == 1 //int0 falling
			MCUCR = (1<<ISC01) | ~(1<<ISC00);
			GIMSK = 1<<INT0;
		#elif Interrupt_MODE == 2 //int1 rising
			MCUCR |= 1<<ISC10|1<<ISC11;
			GIMSK = 1<<INT1;
		#elif Interrupt_MODE == 3 //int1 falling
			MCUCR = (1<<ISC11)|~(1<<ISC10);
			GIMSK = 1<<INT1;
		#endif
		PIN_DDR;
		sei();
	/*#elif (__AVR_ATmega328__)
		cli();
		TIFR |= 1<<OCF1A|1<<OCF1B;
		TIMSK |= 1<<OCIE1A|1<<OCIE1B|1<<TOIE1;
		#if Interrupt_MODE == 0 //int0 rising
			MCUCR |= 1<<ISC00|1<<ISC01;
			GIMSK = 1<<INT0;
		#elif Interrupt_MODE == 1 //int0 falling
			MCUCR = (1<<ISC01) | ~(1<<ISC00);
			GIMSK = 1<<INT0;
		#elif Interrupt_MODE == 2 //int1 rising
			MCUCR |= 1<<ISC10|1<<ISC11;
			GIMSK = 1<<INT1;
		#elif Interrupt_MODE == 3 //int1 falling
			MCUCR = (1<<ISC11)|~(1<<ISC10);
			GIMSK = 1<<INT1;
		#endif
		PIN_DDR;
		sei();*/
	#endif
	//=======================
}

//================================CDI=====================================================
#if Type == 1 


#if Interrupt_MODE == 0 || Interrupt_MODE == 1
ISR(INT0_vect)
#elif Interrupt_MODE == 2 || Interrupt_MODE == 3
ISR(INT1_vect)
#endif


{
	TCCR1B |= 1<<CS10|1<<CS11;//preddelitelj na 64 zapuskaem tajmer esli dat4ik dal signal
	vrema = TCNT1;
	TCNT1 = 0;
	tmp = (vrema /12);
	if (tmp > Gorenie)
	{
		tmp = tmp - Gorenie;
	}else{
		tmp = 1;
	}
	OCR1A = tmp;
	
	
};

ISR(TIMER1_COMPA_vect)
{
	PIN_H;	//ISKRA!
	OCR1B = tmp + 3;// + vremja srabativanija tiristora po datawitu 10us vzjal s zapasom 24us
	
};

ISR(TIMER1_COMPB_vect)
{
	PIN_L; // zapiraem tiristor 4tob daljwe kopitj energiju v kondensatore
};

ISR(TIMER1_OVF_vect)// ostanovka dvigatelja virubaem zazhiganie
{
	PIN_L;
	TCCR1B = 0x00;
};
#else //=================================================TCI================================================================

#if Interrupt_MODE == 0 || Interrupt_MODE == 1
ISR(INT0_vect)
#elif Interrupt_MODE == 2 || Interrupt_MODE == 3
ISR(INT1_vect)
#endif

{
	cli();
	TCCR1B |= 1<<CS10|1<<CS11;//preddelitelj na 64 zapuskaem tajmer esli dat4ik dal signal
	//delta = vrema;
	vrema = TCNT1;
		/*if (delta == 0)
		{
			delta = vrema;
		}
		if (vrema < delta && delta !=0)
		{
			uskorenie = delta - vrema;
			vrema = vrema - uskorenie;
		}
		else if(vrema > delta && delta !=0)
		{
			uskorenie = vrema - delta;
			vrema = vrema + uskorenie;
		}*/
		
	TCNT1 = 0;
	tmp = (vrema /12);

	if (tmp > Gorenie) // tmp verhnjaja mertvaja to4ka
	{
		tmp = tmp - Gorenie;
	}else{
		tmp = 1;
	}
	if (tmp > Nakoplenie)// esli zaderzhka iskri bolee 3ms
	{
		zar = tmp - Nakoplenie;//zaderzhka vklju4enija zarjadki katuwki zazhiganija 
	} 
	else
	{
		zar = vrema + tmp - Nakoplenie;// (obwee vremja za oborot + vremja do iskri "kotoraja budet posle preivanija INTx" -
									   // - vremja nakoplenija energii) vremja zarjadki 3ms
	}
	OCR1A = tmp;
	sei();
	
	
};

ISR(TIMER1_COMPA_vect)
{
	PIN_L;	// ISKRA!
	
	if(vrema <= Nakoplenie)// eto na slu4aj esli oboroti tam budut sliwkom boljwie ili dva cilindra no v etom slu4ae iskra budet v oboih cilindah odnovremenno
	{
		zar = 2; //srazu vgonjaem katuwku v nakoplenie posle iskroobrazovanija
	}
	OCR1B = zar;
	
};

ISR(TIMER1_COMPB_vect)
{
	PIN_H; // nakoplenie energii v katuwke
};

ISR(TIMER1_OVF_vect) // ostanovka dvigatelja
{
	PIN_L;
	TCCR1B = 0x00;//esli dvigatelj zagluwen neza4em tratit energiju akuma na razogrev katuwki zazhiganija 
};				  //maximaljnoe vremja nahozhdenija katuwki pod tokom primerno 0,5 sekundi
#endif

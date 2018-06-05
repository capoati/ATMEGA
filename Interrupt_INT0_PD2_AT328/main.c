/*****************************************************************

	MCU: 					ATmega328
	Autore: 				Emanuele Aimone
	Start Date:				23 / 11 / 2016
	Last Update: 			05 / 06 / 2018

	Description:			When there is a change on PD2 the Interrupt0 (INT0) occur.

	
	License:				This firmware is provided in a way
							free and without any warranty
							The firmware can be modified and
							unlimited copied.
							The author does not assume any
							liability for damage brought or
							caused by this firmware.
							
	Licenza:				Questo firmware viene fornito in modo 
							gratuito e senza alcuna garanzia
							Il firmware può essere modificato e 
							copiato senza limiti.
							L'autore non si assume nessuna 
							responsabilità per danni portati o
							causati da questo firmware.
							

	Link:					https://sites.google.com/site/qeewiki/books/avr-guide/external-interrupts-on-the-atmega328
	
	
*****************************************************************/

/*
FUSE ATMEGA328P
for 8MHz internal oscillator:

Low:		E2
High:		D9
Extended:	FE
Lock:		FF
*/

#include <avr/io.h> 
#include <avr/interrupt.h>
#include <util/delay.h>


//PARAMETERS:
#define 	F_CPU 			8000000UL 	//specifica alla libreria delay.h la frequenza di lavoro del'MCU
										//questo define deve essere definito prima di includere la libreria delay.h
						
#define 	DELAY_LED		100			//mS


//INPUTS:
#define 	STATUS_BUTTON1	 		PORTD2	//not used, here to remeber to connect a button on PD2

//OTPUTS:
#define 	PIN_LD1_SET_ON			PORTB |= 0x02 //PB1 is the LD1
#define 	PIN_LD1_SET_OFF			PORTB &= ~0x02
#define 	STATUS_LD1				bolean_pin_is_set(PINB,1)
						



 
//Interrupt Service Routine for INT0
ISR(INT0_vect)
{
	unsigned int i;
			
	// This for loop blink LEDs on Dataport 5 times
	for(i = 0; i<5; i++)
	{
		PIN_LD1_SET_ON;
		_delay_ms(DELAY_LED); 	// Wait
		PIN_LD1_SET_OFF;
		_delay_ms(DELAY_LED); 	// Wait
	}
}	


int main(void){
	
	

	DDRB = 2; 		//PB1 as output
	DDRD = 1<<PD2;	// Set PD2 as input (Using for interupt INT0)
	
	
	EICRA |= (1 << ISC00);    // set INT0 to trigger on ANY logic change
    EIMSK |= (1 << INT0);     // Turns on INT0
 
	
	sei();				//Enable Global Interrupt

	//infinite loop:
    while(1){

    }

}

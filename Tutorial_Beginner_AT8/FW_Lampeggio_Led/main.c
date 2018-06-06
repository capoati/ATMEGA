/*****************************************************************

	MCU: 				ATmega8
	Autore: 			Emanuele Aimone
	Ultimo Aggiornamento: 		31/10/2015
	
	Descrizione:			Fa lampeggiare un led collegato 
					al PIN23 (PC0)
	

	License:			This firmware is provided in a way
					free and without any warranty
					The firmware can be modified and
					unlimited copied.
					The author does not assume any
					liability for damage brought or
					caused by this firmware.


	Licenza:			Questo firmware viene fornito in modo 
					gratuito e senza alcuna garanzia
					Il firmware può essere modificato e 
					copiato senza limiti.
					L'autore non si assume nessuna 
					responsabilità per danni portati o
					causati da questo firmware.

							
*****************************************************************/


#include <avr/io.h> 
#include <avr/delay.h>

#define DELAY_LED	500 //mS

int main(void){
	
	//configura PC0 (PIN23) come Output 
	//e tutti gli altri PCx vengono configurati come Input
    DDRC = 0x01; 
	
    // loop infinito
    while(1)
    {
		PORTC |= 0x01;    //Led ON
		_delay_ms(DELAY_LED);
		PORTC &= 0xFE;    //Led OFF
		_delay_ms(DELAY_LED);
    }
}

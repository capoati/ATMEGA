/*****************************************************************

	MCU: 						ATmega328
	Autore: 					Emanuele Aimone
	Ultimo Aggiornamento: 		20/09/2018
	
	Descrizione:				Fa lampeggiare un led collegato 
								al PIN23 (PC0) attraverso il Timer0
	
	
	License:					This firmware is provided in a way
								free and without any warranty
								The firmware can be modified and
								unlimited copied.
								The author does not assume any
								liability for damage brought or
								caused by this firmware.


	Licenza:					Questo firmware viene fornito in modo 
								gratuito e senza alcuna garanzia
								Il firmware può essere modificato e 
								copiato senza limiti.
								L'autore non si assume nessuna 
								responsabilità per danni portati o
								causati da questo firmware.

							
*****************************************************************/


#include <avr/io.h> 
#include <avr/interrupt.h>

#define DELAY_LED	100 //mS

#define 	PIN_LED_SET_ON			PORTC |= 0x01 //PC0 is the rele fo the stop machine
#define 	PIN_LED_SET_OFF			PORTC &= ~0x01
#define 	STATUS_LED				bolean_pin_is_set(PINC,0)

//Variabili globali:
unsigned char decontLed; //usato per cambiare lo stato al Led 
						 //dopo il tempo definito con DELAY_LED

						 
//Funzioni:
void timer0_init(void); //inizializza i registri del Timer0
unsigned char bolean_pin_is_set(unsigned char port, unsigned char pin);


//Funzione eseguita quando si scatena l'interrupt del Timer0.
//L'interrupt viene generato quando TCNT0 va in overflows.
ISR(TIMER0_OVF_vect){  // timer0 overflow interrupt
	TCNT0 += 130; 	//precarica il registro TCNT0 in modo di avere un
					//interrupt generato ogni 1mS
	
    if(decontLed>0){
		decontLed--; //fino a quando non è a 0 (passati 500mS) 
					 //decrementa la variabile
	}else{
		decontLed = DELAY_LED; 	//la variabile viene ricaricata per 
								//fornire un altra attesa di 500mS 
								//prima di cambiare di stato il Led
		if(STATUS_LED == 0x00){ //controlla lo stato di PC0
			PIN_LED_SET_ON;    // Led ON
		}else{
			PIN_LED_SET_OFF;    // Led OFF
		}
	}
	
}

//il nostro codice principale, da dove il microcontrollore parte:
int main(void){
	
	//configura PC0 (PIN23) come Output 
	//e tutti gli altri PCx vengono configurati come Input
    DDRC = 0x01; 
	
	cli(); //Disabilita gli interrupts globali
	
	timer0_init(); //inizializza i registri del Timer0
	
	sei(); //Abilita gli interrupts globali
	
    //Ciclo infinito:
    while(1)
    {
		/* qui si può far girare altro codice che quindi 
		   di tanto in tanto viene messo in pausa per eseguire 
		   la funzione dell'interrupt del Timer0 */
    }
}

//inizializza i registri del Timer0
void timer0_init(void){
	//Enable Timer0 overflow Interrupt
	TIMSK0|=(1<<TOIE0);
	//Set Initial Timer value
	TCNT0=130;
	//Set prescaller 64 and start timer
	TCCR0B|=(1<<CS01)|(1<<CS00);
}

unsigned char bolean_pin_is_set(unsigned char port, unsigned char pin){
	unsigned char var_return = 0;
	if(bit_is_set(port,pin)>0){
		var_return = 1;
	}
	
	return var_return;
}

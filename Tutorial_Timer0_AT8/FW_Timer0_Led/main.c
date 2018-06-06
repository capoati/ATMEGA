/*****************************************************************

	MCU: 				ATmega8
	Autore: 			Emanuele Aimone
	Ultimo Aggiornamento: 		19/11/2015
	
	Descrizione:			Fa lampeggiare un led collegato 
					al PIN23 (PC0) attraverso il Timer0
	
	
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
#include <avr/interrupt.h>

#define DELAY_LED	500 //mS

//Variabili globali:
unsigned char decontLed; //usato per cambiare lo stato al Led 
						 //dopo il tempo definito con DELAY_LED

						 
//Funzioni:
void timer0_init(void); //inizializza i registri del Timer0


//Funzione eseguita quando si scatena l'interrupt del Timer0.
//L'interrupt viene generato quando TCNT0 va in overflows.
ISR(TIMER0_OVF_vect){  // timer0 overflow interrupt
	TCNT0 += 6; //precarica il registro TCNT0 in modo di avere un
				//interrupt generato in 1mS
	
    if(decontLed>0){
		decontLed--; //fino a quando non è a 0 (passati 500mS) 
					 //decrementa la variabile
	}else{
		decontLed = DELAY_LED; 	//la variabile viene ricaricata per 
								//fornire un altra attesa di 500mS 
								//prima di cambiare di stato il Led
		if((PORTC & 0x01) == 0x00){ //controlla lo stato di PC0
			PORTC |= 0x01;    // Led ON
		}else{
			PORTC &= 0xFE;    // Led OFF
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
	//Vedere paghina 70 del datasheet per il diagramma a blocchi
	//del Timer0
	
	//Registro TIMSK: Pagina 72 del datasheet
	//Il bit chiamato TOIE0 viene impostato a 1 per abilitare 
	//l'interrupt del Timer0
    TIMSK |= (1 << TOIE0);

    //Registro TCCR0: Pagina 71 del datasheet
	//imposta il clock con prescaler a 64
	//Qui per essere sicuri che l'interrupt abbia cadenza 1mS
	//dobbiamo calcolare in base alla frequenza dell'oscillatore
	//la divisione che imponiamo con il prescaler
    TCCR0 |= (1 << CS01) | (1 << CS00);
  
	//Registro TCNT0: Pagina 72 del datasheet
    //precarica il contatore usato dal Timer0
    TCNT0 = 6;
}

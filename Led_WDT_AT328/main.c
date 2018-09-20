/*****************************************************************

	MCU: 						ATmega328
	Autore: 					Emanuele Aimone
	Ultimo Aggiornamento: 		20/09/2018
	
	Descrizione:				Fa lampeggiare un led collegato 
								al PIN23 (PC0) in questo modo:
									- quando si riavvia fa 5 lampeggi da 1 secondo
									- quando è in run fa 50 lampeggi da 100mS
									- poi si impianta in un loop infinito dove
										interviene il WDT e quindi si riavvia
	
	
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



#define F_CPU 8000000UL //specifica alla libreria delay.h la frequenza di lavoro del'MCU
						//questo define deve essere definito prima di includere la libreria delay.h
						
						
#include <avr/io.h> 
#include <avr/delay.h>
#include <avr/wdt.h>


#define 	DELAY1_LED	1000 //mS
#define 	DELAY2_LED	100 //mS

#define 	PIN_LED_SET_ON			PORTC |= 0x01 				//on PC0 is connected the Led
#define 	PIN_LED_SET_OFF			PORTC &= ~0x01
#define 	PIN_LED_TOGGLE			PORTC ^= (1 << PC0);   		// toggles the state of the bit
#define 	STATUS_LED				bolean_pin_is_set(PINC,0)


//Variabili globali:


						 
//Funzioni:
unsigned char bolean_pin_is_set(unsigned char port, unsigned char pin);
void wdt_init(void); //init the WDT to occur after 500mS



//il nostro codice principale, da dove il microcontrollore parte:
int main(void){
	
	//configura PC0 (PIN23) come Output 
	//e tutti gli altri PCx vengono configurati come Input
    DDRC = 0x01; 

	wdt_init(); //init the WDT to occur after 500mS
	
	wdt_reset(); //it reset the WDT

	for(int i=0;i<10;i++){
		PIN_LED_TOGGLE; //change the status of the Led
		for(int j=0;j<DELAY1_LED;j++){
			wdt_reset(); //it reset the WDT
			_delay_ms(1);
		}
	}
	
	for(int i=0;i<100;i++){
		PIN_LED_TOGGLE; //change the status of the Led
		for(int j=0;j<DELAY2_LED;j++){
			wdt_reset(); //it reset the WDT
			_delay_ms(1);
		}
	}
	
    //Ciclo infinito:
    while(1)
    {
		/* qui si può far girare altro codice ma bisogna richiamare entro 500mS la funzione wdt_reset()  */
    }
}



unsigned char bolean_pin_is_set(unsigned char port, unsigned char pin){
	unsigned char var_return = 0;
	if(bit_is_set(port,pin)>0){
		var_return = 1;
	}
	
	return var_return;
}


//init the WDT to occur after 500mS
void wdt_init(void) {
	//http://www.nongnu.org/avr-libc/user-manual/group__avr__watchdog.html
	wdt_enable(WDTO_500MS);
}

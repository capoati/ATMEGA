/*****************************************************************

	MCU: 					ATmega328
	Autore: 				Emanuele Aimone
	Ultimo Aggiornamento: 	23/11/2016
	
	Descrizione:			Quando la tensione scende al di sotto della soglia 
							impostata con un partitore messo su  INT0 (PD2),
							l'interrupt si scatena e va a salvare i dati sulla 
							EEPROM.
							Se alimentato a 5V il partitore deve essere calcolato
							per fornire 3V.
							Se alimentato a 3.3V il partitore deve essere calcolato
							per fornire 2.5V.
							Con un condensatore da 100uF sul MCU e uno da 10uF
							si ha una lunga curva di scarica per l'MCU che quindi
							ha tempo di salvare i dati.
							
	
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
							http://www.avr-tutorials.com/interrupts/avr-external-interrupt-c-programming
	
	
*****************************************************************/

/*
FUSE ATMEGA328P
for 8MHz internal oscillator:

Low:		E2
High:		C9
Extended:	FF
Lock:		FF
*/



#define 	F_CPU 			8000000UL 	//specifica alla libreria delay.h la frequenza di lavoro del'MCU
										//questo define deve essere definito prima di includere la libreria delay.h
						
#define 	DELAY_LED		50 			//mS


#define 	STATUS_BUTTON1	 		bolean_pin_is_set(PINC,0) //PORTC0	

#define 	PIN_LD1_SET_ON			PORTB |= 0x02 //PB1 is the LD1
#define 	PIN_LD1_SET_OFF			PORTB &= ~0x02
#define 	STATUS_LD1				bolean_pin_is_set(PINB,1)

#define 	PIN_LED2_SET_ON			PORTC |= 0x02 //PC1 is the LED2
#define 	PIN_LED2_SET_OFF		PORTC &= ~0x02
#define 	STATUS_LED2				bolean_pin_is_set(PINC,1)

						

#include <avr/io.h> 
#include <avr/interrupt.h>
#include <avr/eeprom.h>
//#include <avr/delay.h>
#include <util/delay.h>
#include <avr/wdt.h>

 
unsigned char bolean_pin_is_set(unsigned char port, unsigned char pin){
	unsigned char var_return = 0;
	if(bit_is_set(port,pin)>0){
		var_return = 1;
	}
	
	return var_return;
}

//Interrupt Service Routine for INT0
ISR(INT0_vect)
{
	unsigned int i;
 
	if(STATUS_LD1){
		eeprom_write_byte ((unsigned char *)(0x00), 0x01);
	}else{
		eeprom_write_byte ((unsigned char *)(0x00), 0x00);
	}
			
	// This for loop blink LEDs on Dataport 5 times
	for(i = 0; i<5; i++)
	{
		PIN_LED2_SET_ON;
		for(i=0;i<DELAY_LED*10;i++){wdt_reset(); _delay_us(100);} 	// Wait
		PIN_LED2_SET_OFF;
		for(i=0;i<DELAY_LED*10;i++){wdt_reset(); _delay_us(100);} 	// Wait
	}
}	


int main(void){
	
	unsigned int i;
	
	wdt_reset();
		
	PORTC = 0x01; //pull-up resistor on PC0
    //PORTD = 1<<PD2;		// Enable PD2 pull-up resistor
	
    DDRC = 2; 	//configura PC0 e PC1 come Output e tutti gli altri PCx vengono configurati come Input
	DDRB = 2; 	//PB1 as output
	DDRD = 1<<PD2;		// Set PD2 as input (Using for interupt INT0)
	
	
	EICRA |= (1 << ISC00);    // set INT0 to trigger on ANY logic change
    EIMSK |= (1 << INT0);     // Turns on INT0
 
 
	if(eeprom_read_byte((unsigned char *)(0x00)) > 0x00){
			PIN_LD1_SET_ON;
	}

	
	sei();				//Enable Global Interrupt

    while(1)
    {
		wdt_reset();
		
		if(!STATUS_BUTTON1){ //reading the button status 
			
			if(STATUS_LD1){
				PIN_LD1_SET_OFF;
			}else{
				PIN_LD1_SET_ON;
			}
			while(!STATUS_BUTTON1) wdt_reset();
			
			for(i=0;i<1000;i++){ wdt_reset(); _delay_us(100); } //small delay for debounce
		}

    }

}

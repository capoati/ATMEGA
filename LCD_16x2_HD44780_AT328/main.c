/*****************************************************************

	MCU: 					ATmega328p
	Autore: 				Emanuele Aimone
	Ultimo Aggiornamento: 	06/05/2017
	
	Descrizione:			Semplice esempio per LCD HD44780 
	
							Per modificare i collegamenti editare 
							la libreria lcd.c altrimenti eseguire 
							i seguenti collegamenti su un display
							LCD HD44780 16x2 (colonne x righe)
							
							LCD_D4 -> PC0
							LCD_D5 -> PC1
							LCD_D6 -> PC2
							LCD_D7 -> PC3
							LCD_RS -> PC4
							LCD_EN -> PC5
							LCD_WD -> PB0
							
							
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
							
*****************************************************************/

/*  FUSES
	
	Low Fuse 		= 	0xE2
	High Fuse 		= 	0xD9
	Extended Fuse 	=	0xFD
	Lock Fuse 		= 	0xFF
	Calibration 	= 	0xB9B9B9B9
*/


//OUTPUTS
#define 	PIN_DB4uP_SET_ON			PORTC |= 0x01 //PC0 
#define 	PIN_DB4uP_SET_OFF			PORTC &= ~0x01
#define 	STATUS_DB4uP				bolean_pin_is_set(PINC,0)

#define 	PIN_DB5uP_SET_ON			PORTC |= 0x02 //PC1 
#define 	PIN_DB5uP_SET_OFF			PORTC &= ~0x02
#define 	STATUS_DB5uP				bolean_pin_is_set(PINC,1)

#define 	PIN_DB6uP_SET_ON			PORTC |= 0x04 //PC2 
#define 	PIN_DB6uP_SET_OFF			PORTC &= ~0x04
#define 	STATUS_DB6uP				bolean_pin_is_set(PINC,2)

#define 	PIN_DB7uP_SET_ON			PORTC |= 0x08 //PC3 
#define 	PIN_DB7uP_SET_OFF			PORTC &= ~0x08
#define 	STATUS_DB7uP				bolean_pin_is_set(PINC,3)

#define 	PIN_RSuP_SET_ON				PORTC |= 0x10 //PC4 
#define 	PIN_RSuP_SET_OFF			PORTC &= ~0x10
#define 	STATUS_RSuP					bolean_pin_is_set(PINC,4)

#define 	PIN_ENuP_SET_ON				PORTC |= 0x20 //PC5 
#define 	PIN_ENuP_SET_OFF			PORTC &= ~0x20
#define 	STATUS_ENuP					bolean_pin_is_set(PINC,5)

#define 	PIN_WDuP_SET_ON				PORTB |= 0x01 //PB0 
#define 	PIN_WDuP_SET_OFF			PORTB &= ~0x01
#define 	STATUS_WDuP					bolean_pin_is_set(PINB,0)


#define F_CPU 8000000UL //specifica alla libreria delay.h la frequenza di lavoro del'MCU
						//questo define deve essere definito prima di includere la libreria delay.h

#include <avr/io.h> 
//#include <avr/interrupt.h>
//#include <avr/eeprom.h>
//#include <avr/delay.h>
#include <util/delay.h>
//#include <avr/wdt.h>
#include <string.h>
//#include <stdlib.h>
//#include <stdbool.h>


//Variabili globali:


//Include funzioni:		 
#include "lcd.c"



//il nostro codice principale, da dove il microcontrollore parte:
int main(void){

	PORTB = 0; //no pull-up resistor
	PORTD = 0; //no pull-up resistor
	PORTC = 0; //no pull-up resistor on PORTC
	
	DDRB = 1; //only PB0 as output
	DDRD = 0; //all inputs
    DDRC = 1+2+4+8+16+32; //PC0,PC1,PC2,PC3,PC4,PC5 are output 


	LCD_INIT();	//inizializza il display
	
	LCD_CLEAR(); //cancella il display e porta il cursore all'inizio
	
	LCD_PUTS("Hello World!");
	
	// per implementare altre scritte sul display vedere descrizioni sulla libreria lcd.c
	
    //Ciclo infinito:
    do{
		
    }while(1);
}

/*****************************************************************

	MCU: 					ATmega328
	Autore: 				Emanuele Aimone
	Ultimo Aggiornamento: 	05/06/2018
	
	Descrizione:			Fa variare il duty cycle del PWM (PB1)
							attraverso il valore imposto
							sull'entrata analogica (ADC0)
	
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

/*  FUSES per quarzo a 16MHz
	
	Low Fuse 	= 	0xDF
	High Fuse 	= 	0xD9
	Lock Fuse 	= 	0xFF
	Calibration = 	0xB9B9B9B9
*/

#include <avr/io.h> 
#include <avr/interrupt.h>

						 
//Funzioni:
void PWM_init(void); //inizializza i registri per il PWM
void ADC_init(void); //inizializza i registri per l'ADC
unsigned int adc_read(unsigned char ch); //legge il valore dell'ADC

//il nostro codice principale, da dove il microcontrollore parte:
int main(void){
	
	//configura PB1 (PIN15) come Output 
	//e tutti gli altri vengono configurati come Input di default
    DDRB = 0x02; 
	
	cli(); //Disabilita gli interrupts globali
	
	PWM_init(); //inizializza i registri per il PWM
	ADC_init(); //inizializza i registri per l'ADC
	
	sei(); //Abilita gli interrupts globali
	
    //Ciclo infinito:
    while(1)
    {
		//legge dal canale 0 (PC0 sul pin 23)
		//e assegna il valore dell'ADC al registro
		//OCR1A per variare il duty cycle del PWM
		//generato sul pin 15 (PB1)
		OCR1A = adc_read(0); 
    }
}

//inizializza i registri per il PWM
void PWM_init(void){
	// imposta il PWM per il 50% duty cycle a 10bit
	OCR1A = 0x01FF; 
	
	//imposta la modalità non invertente
	TCCR1A |= (1 << COM1A1); 
	
	//imposta modalità di correzione fase a 10bit
	TCCR1A |= (1 << WGM11) | (1 << WGM10);
	
	//imposta il prescaler a 8 e avvia il PWM
	TCCR1B |= (1 << CS11);
	
}

//inizializza i registri per l'ADC
void ADC_init(void){
	// AREF = AVcc  : il riferimento dell'ADC è uguale 
	//a quello dell'alimentazione del microcontrollore
    ADMUX = (1<<REFS0);
	
	// ADC Abilitato e il prescaler è di 128
    // 16000000/128 = 125000
    ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
}


unsigned int adc_read(unsigned char ch){
  // ch seleziona il canale dell'ADC 0~7
  // ANDing with ’7' will always keep the value
  // of ‘ch’ between 0 and 7
  ch &= 0b00000111;  // fa la AND
  
  // prima di fare l'or porta a 0 i primi 3 bit di ADMUX
  ADMUX = (ADMUX & 0xF8)|ch;
 
  // avvia la conversione dell'ADC
  // impostando a ’1' il bit ADSC
  ADCSRA |= (1<<ADSC);
 
  // aspetta che la conversione finisca
  // ADSC diventa ’0' quando la conversione
  // finisce.
  while(ADCSRA & (1<<ADSC));
 
  return (ADC);
}
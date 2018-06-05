/*****************************************************************
	MCU: 					ATmega328p
	Programmer:				Emanuele Aimone
	Version:				1.0
	Start Date:				02 / 06 / 2018
	Last Update: 			05 / 06 / 2018
	
	Description:			RGB PWM on PB1,PB2,PB3
								PB1 = Red
								PB2 = Green
								PB3 = Blue
	
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
	Extended Fuse 	=	0xFE
	Lock Fuse 		= 	0xFF
	
*/

#define F_CPU 8000000UL //specifica alla libreria delay.h la frequenza di lavoro del'MCU
						//questo define deve essere definito prima di includere la libreria delay.h

#include <avr/io.h> 


//PARAMETERS 
#define 	LED_RED_DUTY_CYCLE				100	//uChar (0-255)
#define 	LED_GREEN_DUTY_CYCLE			50	//uChar (0-255)
#define 	LED_BLUE_DUTY_CYCLE				200 //uChar (0-255)



//Funzioni:
void init_RGB_LED_PWM(void); //inizializza i registri del PWM su PB1,PB2,PB3 per RGB LED
								//PB1 = Red
								//PB2 = Green
								//PB3 = Blue
								//ma vi è la possibilità di attivare il PWM su PD3,PD5,PD6
								
void set_duty_cycle_RGB_LED_PWM(unsigned char dt_RED, unsigned char dt_GREEN, unsigned char dt_BLUE); //it set the duty cycle for the PWM on PB1,2,3




//il nostro codice principale, da dove il microcontrollore parte:
int main(void){

	PORTB = 0x00; //all pull-up resistors are disabled
	PORTD = 0x00; //all pull-up resistors are disabled
	PORTC = 0x00; //all pull-up resistors are disabled
	
	DDRB = 2+4+8; //PB1,PB2,PB3 as output
	DDRD = 0; //all inputs
    DDRC = 0; //all inputs


	init_RGB_LED_PWM(); //inizializza i registri del PWM su PB1,PB2,PB3 per RGB LED
	
	set_duty_cycle_RGB_LED_PWM(LED_RED_DUTY_CYCLE, LED_GREEN_DUTY_CYCLE, LED_BLUE_DUTY_CYCLE); //it set the duty cycle for the PWM on PB1,2,3

	
    //Ciclo infinito:
    do{

    }while(1);
}


void init_RGB_LED_PWM(void){ //inizializza i registri del PWM su PB1,PB2,PB3 per RGB LED
								//PB1 = Red
								//PB2 = Green
								//PB3 = Blue
	//PWM1&2 initialisation
	TCCR1A = 0b10100001;// set PWM mode for PB1 and PB2
	TCCR1B = 0b00000010; //set clock:8 prescaler

	//PWM3 initialisation
	TCCR2A = 0b10000001; // set PWM mode for PB3
	TCCR2B = 0b00000010; //set clock:8 prescaler
	
	
	
	
								//PD3 = Red
								//PD5 = Green
								//PD6 = Blue
	//PWM1&2 initialisation
	/*TCCR0A = 0b10100001;// set PWM mode for PD5 and PD6
	TCCR0B = 0b00000010; //set clock:8 prescaler

	//PWM3 initialisation
	TCCR2A = 0b00100001; // set PWM mode for PD3
	TCCR2B = 0b00000010; //set clock:8 prescaler							
	*/							

}


//it set the duty cycle for the PWM on PB1,2,3
void set_duty_cycle_RGB_LED_PWM(unsigned char dt_RED, unsigned char dt_GREEN, unsigned char dt_BLUE){
								//PB1 = Red
								//PB2 = Green
								//PB3 = Blue
	OCR1A = dt_RED; 
	OCR1B = dt_GREEN;
	OCR2A = dt_BLUE;
	
								//PD3 = Red
								//PD5 = Green
								//PD6 = Blue
	/*OCR0A = dt_BLUE; 
	OCR0B = dt_GREEN;
	OCR2B = dt_RED ;*/
}
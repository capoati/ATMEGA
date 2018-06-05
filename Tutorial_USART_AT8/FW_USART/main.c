/*****************************************************************

	MCU: 					ATmega8
	Autore: 				Emanuele Aimone
	Ultimo Aggiornamento: 	15/01/2016
	
	Descrizione:			Con l'ausilio di un convertitore USB-Seriale
							è possibile stabilire una comunicazione
							seriale tra MCU e PC.
							Un led verrà fatto lampeggiare per ogni dato
							ricevuto o trasmesso.
	
	Licenza:				Questo firmware viene fornito in modo 
							gratuito e senza alcuna garanzia
							Il firmware può essere modificato e 
							copiato senza limiti.
							L'autore non si assume nessuna 
							responsabilità per danni portati o
							causati da questo firmware.
							
*****************************************************************/


/* FUSES: per 8Mhz
	Low 		= 	C4
	High 		= 	D9
	Lock		= 	FF
	Calibration	=	BABABABB

*/


#include <avr/io.h> 
#include <avr/interrupt.h>

//Importante: bisogna cambiare la frequenza F_CPU definita qui sotto 
//in modo che sia uguale a quella usata sul microcontrollore
//Se i fuse sono impostarti per una frequenza interna da 8MHz allora
//F_CPU deve essere 8000000UL
#define F_CPU 8000000UL  // 8 MHz

//Qui sotto basta cambiare USART_BAUDRATE con il baud rate voluto
#define USART_BAUDRATE 38400   
#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)

//Il led è sempre acceso, viene spento per il tempo definito qui sotto
//ogni volta che un dato viaggia su seriale
#define TIME_LED_OFF		50 //mS

//Variabili globali:
volatile unsigned char ultimo_byte_ricevuto; //questo byte conterrà l'utimo dato
								    //ricevuto via seriale
									//verrà modificato dentro la funzione
									//di interrupt della USART e letto
									//nella nostra funzione main()
volatile unsigned char cont_byte_ricevuti;	//variabile che tiene traccia di 
									//quanti byte sono stati ricevuti. 
									//Usata anche nella funzione main 
									//per sapere quando un byte è stato 
									//ricevuto.
									
volatile unsigned char decontLed; //usato per cambiare lo stato al Led 
						 //dopo il tempo definito con TIME_LED_OFF

volatile unsigned int decontSend;
			 
//Funzioni:
void USART_Init(void);	//inizializza i registri per la USART
void USART_SendByte(unsigned char data); //carica il byte nel relativo 
									 //registro di invio e invia il dato
void timer0_init(void); //inizializza i registri del Timer0


//Funzione eseguita quando si scatena l'interrupt della USART.
ISR(USART_RXC_vect){
	ultimo_byte_ricevuto = UDR; //il dato ricevuto è contenuto dentro 
							   //il registro UDR
	cont_byte_ricevuti++;	//incremento il contatore
	
	PORTC &= 0xFE;    // Led OFF
	decontLed = TIME_LED_OFF; //precarica la variabile in modo che il
							  //led venga acceso dopo 50mS
}

//Funzione eseguita quando si scatena l'interrupt del Timer0.
//L'interrupt viene generato quando TCNT0 va in overflows.
ISR(TIMER0_OVF_vect){  // timer0 overflow interrupt
	TCNT0 += 6; //precarica il registro TCNT0 in modo di avere un
				//interrupt generato in 1mS
	
    if(decontLed>1){
		decontLed--; //fino a quando non è a 1 (passati 50mS) 
					 //decrementa la variabile
	}else{
		if(decontLed == 1){
			decontLed--; //adesso va a 0 e quindi non passerà di qui
						 //fino a quando decontLed non verrà ricaricato
		
			PORTC |= 0x01;    // Led ON
		}
	}
	
	if(decontSend) decontSend--;
	
}

//il nostro codice principale, da dove il microcontrollore parte:
int main(void){
	//variabile vista solo localmente dalla funzione main()
	//usata per capire quando è stato ricevuto un'altro byte 
	//via seriale
	unsigned char ultimo_cont_byte_ricevuti; 
	
	//configura PC0 (PIN23) come Output 
	//e tutti gli altri PCx vengono configurati come Input
    DDRC = 0x01; 
	
	//DDRC = 0x03;  //Porta PC0 (pin 23) e porta PC1 (pin 24) impostati come output
	
	//Imposta il pin TX della seriale come Output
	DDRD = 0x02; 
	
	cli(); //Disabilita gli interrupts globali
	
	USART_Init(); //inizializza i registri per la USART
	timer0_init(); //inizializza i registri del Timer0
	
	sei(); //Abilita gli interrupts globali
	
	//accendiamo il led che ci notificherà (spegnendosi per 50mS)
	//se passano dati via seriale
	PORTC |= 0x01;    // Led ON
	
	//azzero il contatore di byte ricevuti
	cont_byte_ricevuti = 0;
	
	//inzializzo la variabile per capire poi se ricevo un byte
	ultimo_cont_byte_ricevuti = cont_byte_ricevuti;
	
	//invia un messaggio al PC
	USART_SendByte('C');USART_SendByte('i');USART_SendByte('a');USART_SendByte('o');USART_SendByte('!');
	
	decontSend=0;
	
    //Ciclo infinito:
    do{
		//se il contatore locale e quello incrementato dalla funzione
		//interrupt della USART sono diversi allora è ovvio che un byte
		//è stato ricevuto
		if(ultimo_cont_byte_ricevuti != cont_byte_ricevuti){
			//ora li rimetto uguali per non passare di qui all'infinito
			ultimo_cont_byte_ricevuti = cont_byte_ricevuti; 
			
			//invio indietro al PC il byte ricevuto
			USART_SendByte(ultimo_byte_ricevuto);
			
			//deccomentare per provare ad accendere e spegnere 
			//un led se si preme il tasto 'a' o 'A' da tastiera
			/*if(ultimo_byte_ricevuto == 'a' || ultimo_byte_ricevuto == 'A'){
				if((PORTC & 0x02) == 0){
					PORTC |= 0x02;    // Led ON
				}else{
					PORTC &= 0xFD;    // Led OFF
				}
			}*/
			
		}
		
		

    }while(1);
}

//inizializza i registri per la USART
void USART_Init(void){
   //Imposta il baud rate
   UBRRL = BAUD_PRESCALE; //carica i primi 8bit nel registro UBRR
   UBRRH = (BAUD_PRESCALE >> 8); //adesso carica i sucessivi 8bit nel registro UBRR
	 
  //per settare il numero di bit per trasmissione, la parità e gli stop bit 
  //della seriale bisogna modificare
  //il registro UCSRC che però di default ha i valori che servono a noi:
  //8bit dati, no parità, 1 stop bit

  // Abilita la ricezione, trasmissione e l'interrupt della USART
  UCSRB = ((1<<TXEN)|(1<<RXEN) | (1<<RXCIE));
}

//carica il byte nel relativo registro di invio e invia il dato
void USART_SendByte(unsigned char data){

  //Se l'ultimo dato caricato non è ancora stato trasmesso allora
  //aspetta che venga inviato
  while( !(UCSRA &(1<<UDRE)) );

  //Carica e trasmette il dato
  UDR = data;
}

// Funzione non usata, ma qui per completezza
// Aspetta fino a quando un dato non viene ricevuto e poi restituisce il dato
unsigned char USART_ReceiveByte(){
  while((UCSRA &(1<<RXC)) == 0);
  return UDR;
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


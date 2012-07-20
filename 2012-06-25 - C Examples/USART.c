#include <p18f4550.h>

#pragma config PLLDIV=2, CPUDIV=OSC1_PLL2, USBDIV=2, IESO=ON, WDT=OFF
#pragma config BOR=OFF, PWRT=ON, LVP=OFF, FOSC=HSPLL_HS, FCMEN=OFF, VREGEN=OFF
#pragma config MCLRE=ON, STVREN=ON, LPT1OSC=ON, PBADEN=OFF

//######### Function Declarations ################
void high_isr(void);
void low_isr(void);
void setup(void);

//######### Variables ################
volatile unsigned char txbuf;
volatile unsigned char rxbuf;

//######### Interrupts ################
#pragma code high_isr_entry=8
void high_isr_entry(void){
	_asm goto high_isr _endasm
}

#pragma code low_isr_entry=0x18
void low_isr_entry(void){
	_asm goto low_isr _endasm
}
#pragma code

#pragma interrupt high_isr
void high_isr(void){
	if(PIR1bits.RCIF = 1){
		//Receive byte
		rxbuf = RCREG;
	}
	
	if(PIR1bits.TXIF = 1){
		//Trasnmit Byte
		TXREG = rxbuf;
	}	
}

#pragma interruptlow low_isr
void low_isr(void){
	
}

//######### Functions ################

void setup(){
	TXSTA = 0b00100000;
	RCSTA = 0b10010000;
	BAUDCON = 0b00001000;
	SPBRGH = 2437 >> 8;
	SPBRG = 2437;
	
	PIE1bits.RCIE = 1;
	PIE1bits.TXIE = 1;
	INTCONbits.PEIE = 1;
	INTCONbits.GIE = 1;
}


void main(void) { 
	setup();
	while(1)	{
	}
}



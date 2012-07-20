#include <p18f4550.h>
#include <stdlib.h>

#pragma config PLLDIV=2, CPUDIV=OSC1_PLL2, USBDIV=2, IESO=ON, WDT=OFF
#pragma config BOR=OFF, PWRT=ON, LVP=OFF, FOSC=HSPLL_HS, FCMEN=OFF, VREGEN=OFF
#pragma config MCLRE=ON, STVREN=ON, LPT1OSC=ON, PBADEN=OFF

//######### Function Declarations ################
void high_isr(void);
void low_isr(void);
void setup();

//######### Variables ################
volatile unsigned char sentFlag = 0;

//######### Interrupts ################
#pragma code high_isr_entry=8
void high_isr_entry(void){
	_asm goto high_isr _endasm
}

#pragma code low_isr_entry=0x18
void low_isr_entry(void){
	_asm goto low_isr _endasm
}

#pragma interrupt high_isr
void high_isr(void){
	static unsigned char step = 0;
	if(PIR1bits.TXIF == 1){
		if(step == 0){
			TXREG = ADRESH;
			step = 1;
		}else if(step == 1){
			TXREG = ADRESL;
			step = 2;
		}else if(step == 2){
			TXREG = 10;								//New Line
			step = 0;
			PIE1bits.TXIE = 0;
			ADCON0bits.GO_DONE = 1;
			sentFlag = 0;
		}else{
			step = 0;
		}
	}
	if(PIR1bits.ADIF == 1){
		PIR1bits.ADIF = 0;
		PIE1bits.TXIE = 1;
	}
		
}

#pragma interruptlow low_isr
void low_isr(void){

}

//######### Functions ################
void setup(){
	TRISA = 1;

	TXSTA = 0b00100100;
	RCSTA = 0b10000000;
	BAUDCON = 0b00000000;
	SPBRG = 78;

	ADCON2 = 0b10010010;
	ADCON1 = 0b00001110;
	ADCON0 = 0b00000001;
	ADCON0bits.GO_DONE = 1;
	
	PIE1bits.TXIE = 0;
	PIE1bits.ADIE = 1;
	INTCONbits.PEIE = 1;
	INTCONbits.GIE = 1;
}

unsigned int binToASCII(

void main(void) { 
	setup();
	while(1)	{
		
	}
}



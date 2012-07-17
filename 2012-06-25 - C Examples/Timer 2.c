#include <p18f4550.h>

#pragma config PLLDIV=2, CPUDIV=OSC1_PLL2, USBDIV=2, IESO=ON, WDT=OFF
#pragma config BOR=OFF, PWRT=ON, LVP=OFF, FOSC=HSPLL_HS, FCMEN=OFF, VREGEN=OFF
#pragma config MCLRE=ON, STVREN=ON, LPT1OSC=ON, PBADEN=OFF

//######### Function Declarations ################
void high_isr(void);
void low_isr(void);

//######### Variables ################


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
	if(PIR1bits.TMR2IF == 1){
		PIR1bits.TMR2IF = 0;
		PORTD ^= 1;
	}
}

#pragma interruptlow low_isr
void low_isr(void){
}

//######### Functions ################
void setup(){
	TRISB = 0x03;						// set RB0, RB1 to input
	
	T2CON = 0b00000100;
	PIE1bits.TMR2IE = 1;
	PIR1bits.TMR2IF = 0;
	INTCONbits.PEIE = 1;				// Enable Perrifal Interrups
	INTCONbits.GIE = 1;				// Enable Interrups Globally
}


void main(void) { 
	setup();
	while(1)	{

	}
}



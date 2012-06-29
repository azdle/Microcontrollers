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
	static char counter = 0;
	if(INTCONbits.INT0IF == 1){
		INTCONbits.INT0IF = 0;
		counter++;
		if(counter <= 5){
			counter = 0;
			PORTD ^= 1;
		}
	}
}

#pragma interruptlow low_isr
void low_isr(void){
}

//######### Functions ################
void setup(){
	TRISB = 0x03;						// set RB0, RB1 to input
	RCONbits.IPEN = 1;					// Interrupt Routing 1-> low to low, 0-> low to high
	INTCONbits.GIEL = 1;				// Enable Low Priority Interrupts Globally
	INTCON3bits.INT1IP = 0;				// Makes INT1 a Low Priority Interrupt
	INTCON2bits.INTEDG1 = 0;			//Set Rising/Falling Edge Sensitivty
	INTCON3bits.INT1IF = 0;				// Set INT1 Flag to 0
	INTCON3bits.INT1IE = 1;				// Enable INT1 Interrupt
	INTCONbits.INT0IF = 0;				// Set Int0 Flag to 0
	INTCONbits.INT0IE = 1;				// Enable INT0 Interrupt
	INTCONbits.GIE = 1;					// Enable Interrups Globally
}


void main(void) { 
	setup();
	while(1)	{
	}
}



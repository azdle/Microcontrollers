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
	static int counter = 0;
	if(PIR1bits.TMR2IF == 1){
		counter++;
		if(counter >= 200){
			PORTDbits.RD0 != PORTDbits.RD0;
		}
	}
}

#pragma interruptlow low_isr
void low_isr(void){
}

//######### Functions ################
void setup(){
	TRISD = 0x00;						// set PORTD to output

	//TRM2: /15 /16 PS2=250 count=200 => 1sec
	PR2 = 250;
	T2CONbits.T2CKPS1 = 1;				// Prescale to 16
	T2CONbits.T2OUTPS3 = 1;				// Postscale of 15
	T2CONbits.T2OUTPS2 = 1;				// -
	T2CONbits.T2OUTPS1 = 1;				// -
	T2CONbits.T2OUTPS0 = 0;				// -
	T2CONbits.TMR2ON = 1;				// Turn Timer 2 On
	
	PIE1bits.TMR2IE = 1;				// Enable TMR2 Interrupt
	INTCONbits.GIE = 1;					// Enable Interrups Globally
}


void main(void) { 
	setup();
	while(1)	{
		if(INTCONbits.TMR0IF == 1){
			INTCONbits.TMR0IF = 0;
			TMR0H = 0x0B;						// Set timer 0 to 3036
			TMR0L = 0xDC;						//  - Total of 62500 counts to go.
			PORTD ^= 1;
		}
	}
}


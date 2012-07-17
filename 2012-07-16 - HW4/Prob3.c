#include <p18f4550.h>

#pragma config PLLDIV=2, CPUDIV=OSC1_PLL2, USBDIV=2, IESO=ON, WDT=OFF
#pragma config BOR=OFF, PWRT=ON, LVP=OFF, FOSC=HSPLL_HS, FCMEN=OFF, VREGEN=OFF
#pragma config MCLRE=ON, STVREN=ON, LPT1OSC=ON, PBADEN=OFF

//######### Function Declarations ################
void high_isr(void);
void low_isr(void);

//######### Variables ################
unsigned char pulse_counter = 0;
unsigned int pulse_buffer = 0; 				// Using int because 200*5 > 256

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
	if(INTCONbits.TMR0IF == 1){
		TMR0H = 0x24;						// Set timer 0 to 3036
		TMR0L = 0x45;						//  - Total of 62500 counts to go.
		INTCONbits.TMR0IF = 0;
		pulse_buffer = pulse_counter;
		pulse_counter = 0;
	}
	if(INTCONbits.INT0IF == 1){
		INTCONbits.INT0IF = 0;
		pulse_counter++;
	}
}

#pragma interruptlow low_isr
void low_isr(void){
}

//######### Functions ################
void setup(){
	TRISD = 0x00;						// set RB0, RB1 to input
	TMR0H = 0x24;						// Set Timer0 to 3036
	TMR0L = 0x45;						//  - Total of 62500 counts to go.
	T0CONbits.T0PS2 = 1;				// Set Timer0 Prescaler to 1:64
	T0CONbits.T0PS1 = 1;				//  -
	T0CONbits.T0PS0 = 1;				//  -
	T0CONbits.PSA = 0;					// Use Prescaler
	T0CONbits.T0CS = 0;					// Use Interanal OSC for Clock Source
	T0CONbits.T08BIT = 0;				// Use Timer 0 as a 16 bit timer.
	T0CONbits.TMR0ON = 1;				// Enable Timer 0
	INTCONbits.TMR0IE = 1;				// Enable Timer0 Interrupt
	INTCONbits.TMR0IF = 0;				// Set Timer 0 Flag to 0, Just to Be Sure
	INTCONbits.INT0IF = 0;				// Zero Int0 Flag, Just to Be Sure
	INTCONbits.INT0IE = 1;				// Enable Int0
	INTCONbits.GIE = 1;					// Enable Interrups Globally
}


void main(void) { 
	setup();
	while(1)	{
		if(pulse_buffer != 0){
			//Multipy first because dividing first reduces data precesion
			PORTD = (pulse_buffer * 5) / 6; 
			pulse_buffer = 0;
		}
	}
}


#include <p18f4550.h>

#pragma config PLLDIV=2, CPUDIV=OSC1_PLL2, USBDIV=2, IESO=ON, WDT=OFF
#pragma config BOR=OFF, PWRT=ON, LVP=OFF, FOSC=HSPLL_HS, FCMEN=OFF, VREGEN=OFF
#pragma config MCLRE=ON, STVREN=ON, LPT1OSC=ON, PBADEN=OFF

//######### Function Declarations ################
void high_isr(void);
void low_isr(void);
void display(unsigned char);
void displayNext(unsigned char);
void setup();

//######### Variables ################
unsigned char counter = 0;

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
		if(INTCONbits.TMR0IF == 1){	
			counter++;
			TMR0H = -63921 >> 8;			//Reset Counter
			TMR0L = -63921;				// -
			INTCONbits.TMR0IF = 0;		//Reset Flag
			PORTD ^= 1;					//Twiddle RD0
			if(counter >= 55){
				T0CONbits.TMR0ON = 0;	//Turn off Timer After 3 pulses
				PORTD = 0xFF;
			}
		}
}

#pragma interruptlow low_isr
void low_isr(void){

}

//######### Functions ################
void setup(){
	TRISB = 0x03;						// set RB0, RB1 to input
	TRISC = 0x00;						//Set All to Output
	TRISD = 0x00;						//Set All to Output
	PORTD = 0x00;						//Turn off All Pins

	INTCON2bits.TMR0IP = 1;				//High Priority = 1; Low = 0
	T0CONbits.T0CS = 0;					//Clock from interal = 0, Pin Count = 1
	T0CONbits.PSA = 0;					//1 = Use; 0 = Don't Use Prescaler
	T0CONbits.T0PS2 = 1;				//Set Prescaler (Pg: 129)
	T0CONbits.T0PS1 = 1;				// -
	T0CONbits.T0PS0 = 1;				// -
	T0CONbits.T08BIT = 0;				//Use Timer 0 as 16 Bit Timer
	INTCONbits.TMR0IE = 1;				//Enable Timer 0 Interrupt
	TMR0H = -63921 >> 8;					//Preset Timer 0 Counter
	TMR0L = -63921;						// -
	T0CONbits.TMR0ON = 1;				//Turn on Timer 0

	INTCONbits.GIE = 1;					// Enable Interrups Globally
}

void main(void) { 
	setup();
	while(1)	{

	}
}

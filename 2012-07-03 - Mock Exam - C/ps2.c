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
unsigned char i;

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
			TMR0H = -12000 >> 8;
			TMR0L = -12000;
			INTCONbits.TMR0IF = 0;
			PORTD ^= 1;
		}
}

#pragma interruptlow low_isr
void low_isr(void){

}

//######### Functions ################
void setup(){
	TRISB = 0x03;						// set RB0, RB1 to input
	TRISC = 0x00;
	TRISD = 0x00;

	INTCON2bits.TMR0IP = 1;				//High Priority
	T0CONbits.T0CS = 0;					//Clock from interal clock = 0, Pin Count = 1
	T0CONbits.PSA = 1;					//Don't Use Prescaler
	T0CONbits.T08BIT = 0;				//Use Timer 0 as 16 Bit
	INTCONbits.TMR0IE = 1;				//Enable Interrupt
	TMR0H = -12000 >> 8;
	TMR0L = -12000;
	T0CONbits.TMR0ON = 1;				//Turn on Timer 0

	INTCONbits.GIE = 1;					// Enable Interrups Globally

	//Setup Port:
	PORTD = 0x00;
}

void main(void) { 
	setup();
	while(1)	{
	}
}



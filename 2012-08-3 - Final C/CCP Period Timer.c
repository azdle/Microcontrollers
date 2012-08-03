#include <p18f4550.h>

#pragma config PLLDIV=2, CPUDIV=OSC1_PLL2, USBDIV=2, IESO=ON, WDT=OFF
#pragma config BOR=OFF, PWRT=ON, LVP=OFF, FOSC=HSPLL_HS, FCMEN=OFF, VREGEN=OFF
#pragma config MCLRE=ON, STVREN=ON, LPT1OSC=ON, PBADEN=OFF

//######### Function Declarations ################
void high_isr(void);
void low_isr(void);
void setup(void);

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
	static unsigned int periodStart = 0;
	static unsigned long int period = 0;
	static unsigned char overflowCounter = 0;
	if(PIR1bits.CCP1IF == 1){
		PIR1bits.CCP1IF = 0;
		period = CCPR1 - periodStart + (((long int)overflowCounter) << 16);
		periodStart = CCPR1;
		overflowCounter = 0;
	}
	if(PIR2bits.TMR3IF == 1){
		PIR1bits.TMR2IF = 0;
		overflowCounter++;
	}
}

#pragma interruptlow low_isr
void low_isr(void){
}

//######### Functions ################
void setup(){
	TRISCbits.TRISC2 = 1;
	
	//TMR3
	T3CONbits.RD16 = 1;
	T3CONbits.T3CCP2 = 1;
	T3CONbits.TMR3ON = 1;

	//CCP1
	CCP1CONbits.CCP1M3 = 0;
	CCP1CONbits.CCP1M2 = 1;
	CCP1CONbits.CCP1M1 = 0;
	CCP1CONbits.CCP1M0 = 1;

	//Interrupts
	PIE1bits.CCP1IE = 1;
	INTCONbits.PEIE = 1;
	INTCONbits.GIE = 1;
	
}


void main(void) { 
	setup();
	while(1)	{

	}
}



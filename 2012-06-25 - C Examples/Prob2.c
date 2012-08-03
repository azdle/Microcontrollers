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
}

#pragma interruptlow low_isr
void low_isr(void){
}

//######### Functions ################
void setup(){
	TRISCbits.RC1 = 0;

	CCP2CONbits.CCP2M3 = 1;
	CCP2CONbits.CCP2M2 = 1;

	PR2 = 240;
	CCPR2L = 32;
	CCP2CONbits.DC2B1 = 0;
	CCP2CONbits.DC2B0 = 1;

	T2CONbits.T2CKPS1 = 0;
	T2CONbits.T2CKPS0 = 1;
	T2CONbits.TMR2ON = 1;

	T1CONbits.RD16 = 1;
	T1CONbits.T1CKPS1 = 1;
	T1CONbits.T1CKPS0 = 1;
	T1CONbits.T1OSCEN = 1;
	T1CONbits.TMR1CS = 0;
	T1CONbits.TMR1ON = 1;
	
}


void main(void) { 
	int i;
	setup();
	for(i = 0; i<=1373; i++){
		while(PIR1bits.TMR1IF == 0);
		PIR1bits.TMR1IF = 0;
	}
	CCPR2L = 215;
	CCP2CONbits.DC2B1 = 1;
	CCP2CONbits.DC2B0 = 0;
	while(1)	{
	}
}



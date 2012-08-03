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
	static unsigned int rxbuff = 0;
	if(PIR1bits.RCIF == 1){
		//Receive byte
		if(rxbuff == 0){
			rxbuff = RCREG;
		}else{
			CCPR1L = rxbuff;
			rxbuff = RCREG;
			CCP1CONbits.DC1B1 = (rxbuff & 0b00000010) >> 1;
			CCP1CONbits.DC1B0 = (rxbuff & 0b00000001);
				
			rxbuff = 0;
		}
	}
}

#pragma interruptlow low_isr
void low_isr(void){
}

//######### Functions ################
void setup(){
	TRISCbits.RC2 = 0;

	CCP1CONbits.DC1B1 = 0;
	CCP1CONbits.DC1B0 = 0;
	CCP1CONbits.CCP1M3 = 1;
	CCP1CONbits.CCP1M2 = 1;

	PR2 = 249;
	CCPR1L = 100;

	T2CONbits.T2CKPS1 = 0;
	T2CONbits.T2CKPS0 = 1;
	T2CONbits.TMR2ON = 1;

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
	char i;
	setup();
	while(1)	{
	}
}



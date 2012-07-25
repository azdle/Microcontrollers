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
	if(SSPSTATbits.BF == 0){
		RXBUFFER = SSPBUF;
		SSPBUFF = TXBUFF;
		SPIFLAG = 0;
	}
}

#pragma interruptlow low_isr
void low_isr(void){
}

//######### Functions ################
void setup(){
	SSPSTATbits.SMP = 0;
	SSPCONbits.SSPEN = 1;
	SSPCONbits.CKP = 0;
	SSPCONbits.SSPM3 = 0;
	SSPCONbits.SSPM2 = 0;
	SSPCONbits.SSPM1 = 0;
	SSPCONbits.SSPM0 = 0;

	PIE1bits.SSPIE = 1;
	INTCONbits.PEIE = 1;
	INTCONbits.GIE = 1;
}

unsigned char rwspi(unsigned char spiOut){
	SSPBUF = spiOut;
	while(SSPSTATbits.BF == 0);
	return SSPBUF;
}


void main(void) { 
	setup();
	rwspi(0x80)
	rwspi(0x10)
	rwspi(0x02)
	TEMPH = rwspi(0x10)
	TEMPL = rwspi(0x0)
	while(1)	{

	}
}



#include <p18f4550.h>

#pragma config PLLDIV=2, CPUDIV=OSC1_PLL2, USBDIV=2, IESO=ON, WDT=OFF
#pragma config BOR=OFF, PWRT=ON, LVP=OFF, FOSC=HSPLL_HS, FCMEN=OFF, VREGEN=OFF
#pragma config MCLRE=ON, STVREN=ON, LPT1OSC=ON, PBADEN=OFF

//######### Function Declarations ################
void high_isr(void);
void low_isr(void);
void setup(void);

//######### Variables ################
unsigned char TempH;
unsigned char TempL;
volatile unsigned char TempBuffWrite;
#pragma udata TempBuff = 0x200  //Use ram bank 0x200-0x2FF
volatile unsigned char TempBuff[256];
#pragma udata

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
	if(PIR1bits.SSPIF == 1){
		PIR1bits.SSPIF = 0;
		TempBuff[TempBuffWrite++] = SSPBUF;
		SSPBUF = 0;
	}
	if(PIR1bits.CCP1IF == 1){
		PIR1bits.CCP1IF = 0;
		SSPREG = 0;
	}
}

#pragma interruptlow low_isr
void low_isr(void){
}

//######### Functions ################
void setup(){
	SSPSTATbits.CKE = 1;
	
	SSPCON1bits.CKP = 0;
	SSPCON1bits.SSPM3 = 0;
	SSPCON1bits.SSPM2 = 0;
	SSPCON1bits.SSPM1 = 0;
	SSPCON1bits.SSPM0 = 0;
	SSPCON1bits.SSPEN = 1;

	CCP1CONbits.CCP1M3 = 1;
	CCP1CONbits.CCP1M2 = 0;
	CCP1CONbits.CCP1M1 = 1;
	CCP1CONbits.CCP1M0 = 1;

	T1CONbits.RD16 = 0;
	T1CONbits.T1CKPS1 = 0;
	T1CONbits.T1CKPS0 = 0;
	T1CONbits.TMR1CS = 0;
	T1CONbits.TMR1ON = 1;

	PIE1bits.CCP1IE = 1;
	PIE1bits.TMR1IE = 1;
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
	rwspi(0x80);
	rwspi(0x10);
	rwspi(0x02);
	TempH = rwspi(0x10);
	TempL = rwspi(0x00);
	while(1)	{

	}
}



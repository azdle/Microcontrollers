#include <p18f4550.h>
#include <stdlib.h>

#pragma config PLLDIV=2, CPUDIV=OSC1_PLL2, USBDIV=2, IESO=ON, WDT=OFF
#pragma config BOR=OFF, PWRT=ON, LVP=OFF, FOSC=HSPLL_HS, FCMEN=OFF, VREGEN=OFF
#pragma config MCLRE=ON, STVREN=ON, LPT1OSC=ON, PBADEN=OFF

//######### Function Declarations ################
void high_isr(void);
void low_isr(void);
void setup();
void writeNewADCValue(unsigned int);
void readADCAverageASCII();

//######### Variables ################
volatile unsigned char sentFlag = 0;
volatile unsigned int ADCAverage = 0;
volatile char ASCIIString[7];

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
	static unsigned char input = 0;
	static unsigned char newDataFlag = 0;

	if(PIR1bits.RCIF == 1){
		input = RCREG;
		newDataFlag = 1;
	}
	if(PIR1bits.TXIF == 1){
		if (newDataFlag == 2){
			TXREG = 10; //NewLine
			newDataFlag = 0;
		}else if (newDataFlag == 1){
			TXREG = input;
			newDataFlag = 2;
		}
	}	
}

#pragma interruptlow low_isr
void low_isr(void){

}

//######### Functions ################
void setup(){
	TRISC = 0b11000000;

	TXSTA = 0b00100100;
	RCSTA = 0b10010000;
	BAUDCON = 0b00000000;
	SPBRG = 78;


	while(PIR1bits.TXIF == 0);
	TXREG = 10; // New Line
	while(PIR1bits.TXIF == 0);
	TXREG = 45; // -
	while(PIR1bits.TXIF == 0);
	TXREG = 62; // >
	
	PIE1bits.TXIE = 1;
	PIE1bits.RCIE = 1;
	INTCONbits.PEIE = 1;
	INTCONbits.GIE = 1;
}
	

void main(void) { 
	setup();

	while(1)	{
		
	}
}



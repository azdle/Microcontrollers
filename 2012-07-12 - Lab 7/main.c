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
volatile unsigned char ASCIIString[7];

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
	static unsigned char step = 0;
	if(PIR1bits.TXIF == 1){
		if(step == 7){
			TXREG = 10; //Line Return, TODO Change to Carrage Return 0x0D
			step = 0;
		}else{
			if (step == 0){
				readADCAverageASCII();
			}
			TXREG = ASCIIString[step++];
		}
	}
	if(PIR1bits.ADIF == 1){
		PIR1bits.ADIF = 0;
		writeNewADCValue(ADRES);
		ADCON0bits.GO_DONE = 1;
	}
		
}

#pragma interruptlow low_isr
void low_isr(void){

}

//######### Functions ################
void setup(){
	TRISA = 1;

	TXSTA = 0b00100100;
	RCSTA = 0b10000000;
	BAUDCON = 0b00000000;
	SPBRG = 78;

	ADCON2 = 0b10010010;
	ADCON1 = 0b00001110;
	ADCON0 = 0b00000001;
	ADCON0bits.GO_DONE = 1;
	
	PIE1bits.TXIE = 1;
	PIE1bits.ADIE = 1;
	INTCONbits.PEIE = 1;
	INTCONbits.GIE = 1;
}

void writeNewADCValue(unsigned int value){
	ADCAverage = (ADCAverage + value)/2;  //Can't do moving average, not enough ram.
}

void readADCAverageASCII(){
	float voltage;
	long lWhole, ulPart;

	voltage = ((float)(ADCAverage*5))/1024;
	//Convert number from float to fixed point for display.
	//The number is converted to two parts.
	lWhole=(long)(voltage);
	ulPart=(long)(voltage*100)-lWhole*100;
	
	sprintf(ASCIIString, (char *)"%li.%li\n",lWhole,ulPart);

}
	

void main(void) { 
	setup();
	while(1)	{
		
	}
}



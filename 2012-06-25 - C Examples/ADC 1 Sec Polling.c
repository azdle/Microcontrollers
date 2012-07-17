#include <p18f4550.h>
#include <delays.h>

#pragma config PLLDIV=2, CPUDIV=OSC1_PLL2, USBDIV=2, IESO=ON, WDT=OFF
#pragma config BOR=OFF, PWRT=ON, LVP=OFF, FOSC=HSPLL_HS, FCMEN=OFF, VREGEN=OFF
#pragma config MCLRE=ON, STVREN=ON, LPT1OSC=ON, PBADEN=OFF

//######### Function Declarations ################
void high_isr(void);
void low_isr(void);
void setup();

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
	ADCON2 = 0b10010010;
	ADCON1 = 0b00001110;
	ADCON0 = 0b00000001;
}


void main(void) { 
	unsigned int temp;
	setup();
	while(1)	{
		ADCON0bits.GO_DONE = 1;
		while(ADCON0bits.GO_DONE == 1);
		temp = ((int) ADRESH) << 8 | ADRESL;
		Delay10TCYx(0);
		Delay10TCYx(0);
		Delay10TCYx(0);
		Delay10TCYx(0);
		Delay10TCYx(0);
	}
}



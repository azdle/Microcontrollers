#include <p18f4550.h>
#include <delays.h>

#pragma config PLLDIV=2, CPUDIV=OSC1_PLL2, USBDIV=2, IESO=ON, WDT=OFF
#pragma config BOR=OFF, PWRT=ON, LVP=OFF, FOSC=HSPLL_HS, FCMEN=OFF, VREGEN=OFF
#pragma config MCLRE=ON, STVREN=ON, LPT1OSC=ON, PBADEN=OFF

//######### Function Declarations ################
void high_isr(void);
void low_isr(void);
void setup(void);
//void random(void);

//######### Variables ################
unsigned long int SEED_X = 521288629L;
unsigned long int SEED_Y = 362436069L;


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
	INTCON2bits.RBPU = 0;
	TRISB = 0xFF;
	TRISD = 0x00;
	PORTD = 0x00;
}

unsigned int random(){
	static unsigned int a = 18000, b = 30903;

	SEED_X = a*(SEED_X&65535) + (SEED_X>>16);
	SEED_Y = b*(SEED_Y&65535) + (SEED_Y>>16);
	return (SEED_X + SEED_Y)/2;
}



void main(void) { 
	setup();
	while(1)	{
		while(PORTBbits.RB7 == 1);
		Delay10KTCYx(2000);
		while(PORTBbits.RB7 == 0);
		Delay10KTCYx(2000);
		PORTD = random();
	}
}



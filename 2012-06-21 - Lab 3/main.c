#include <p18f4550.h>

#pragma config PLLDIV=2, CPUDIV=OSC1_PLL2, USBDIV=2, IESO=ON, WDT=OFF
#pragma config BOR=OFF, PWRT=ON, LVP=OFF, FOSC=HSPLL_HS, FCMEN=OFF, VREGEN=OFF
#pragma config MCLRE=ON, STVREN=ON, LPT1OSC=ON, PBADEN=OFF

void high_isr(void);
void low_isr(void);

#pragma code high_isr_entry=8
void high_isr_entry(void)
{
	_asm goto high_isr _endasm
}

#pragma code low_isr_entry=0x18
void low_isr_entry(void)
{
	_asm goto low_isr _endasm
}
#pragma code

#pragma interrupt high_isr
void high_isr(void)
{
}

#pragma interruptlow low_isr
void low_isr(void)
{
}

void setup(void)
{
    
}

void main(void) 
{ 
	setup();	
	while(1)
	{
	}
}



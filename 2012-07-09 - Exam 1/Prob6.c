#include <p18f4550.h>

#pragma config PLLDIV=2, CPUDIV=OSC1_PLL2, USBDIV=2, IESO=ON, WDT=OFF
#pragma config BOR=OFF, PWRT=ON, LVP=OFF, FOSC=HSPLL_HS, FCMEN=OFF, VREGEN=OFF
#pragma config MCLRE=ON, STVREN=ON, LPT1OSC=ON, PBADEN=OFF

//######### Function Declarations ################
void high_isr(void);
void low_isr(void);
void setup();

//######### Functions ################
void setup(){
	TRISD = 0x00;						//Set All to Output
	PORTD = 0x00;						//Turn off All Pins

	INTCON2bits.TMR0IP = 1;				//High Priority = 1; Low = 0
	T0CONbits.T0CS = 0;					//Clock from interal = 0, Pin Count = 1
	T0CONbits.PSA = 0;					//0 = Use; 1 = Don't Use Prescaler
	T0CONbits.T0PS2 = 1;				//Set Prescaler = 256 (Pg: 129)
	T0CONbits.T0PS1 = 1;				// -
	T0CONbits.T0PS0 = 1;				// -	
	T0CONbits.T08BIT = 0;				//Use Timer 0 as 16 Bit Timer
	INTCONbits.TMR0IE = 1;				//Enable Timer 0 Interrupt
	T0CONbits.TMR0ON = 1;				//Turn on Timer 0
}

void main(void) { 
	unsigned char countdown = 89;	//Round down, plenty of overhead anyway
	setup();
	while(1)	{
		while(countdown != 0){			//Block until complete
			if(INTCONbits.TMR0IF == 1){	
				INTCONbits.TMR0IF = 0;		//Reset Flag
				countdown--;
			}	
		}
	}
}

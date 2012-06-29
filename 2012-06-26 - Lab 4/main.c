#include <p18f4550.h>

#pragma config PLLDIV=2, CPUDIV=OSC1_PLL2, USBDIV=2, IESO=ON, WDT=OFF
#pragma config BOR=OFF, PWRT=ON, LVP=OFF, FOSC=HSPLL_HS, FCMEN=OFF, VREGEN=OFF
#pragma config MCLRE=ON, STVREN=ON, LPT1OSC=ON, PBADEN=OFF

//######### Function Declarations ################
void high_isr(void);
void low_isr(void);
void display(unsigned char);
void displayNext(unsigned char);
void setup();

//######### Variables ################
unsigned char key;
unsigned char currKey;
unsigned char lastKey;
unsigned char dispChars;
unsigned char charLookup[] = { //Inverse Logic
	0b00000011, 0b10011111, 0b00100101, //0,1,2
	0b00001101, 0b10011001,	0b01001001,	//3,4,5
	0b00000101, 0b00011111, 0b00000001,	//6,7,8
	0b00011001, 0b00010000, 0b00000000,	//9,A,B
	0b01100010, 0b00000010, 0b01100000,	//C,D,E
	0b01110000	//F -- All Letters Have DP Enabed
};

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
	static unsigned char dispnum = 1;

	dispnum ^= 0x01;
	if(INTCONbits.TMR0IF == 1){
		INTCONbits.TMR0IF = 0;
		display(dispnum);
	}
}

#pragma interruptlow low_isr
void low_isr(void){
}

//######### Functions ################
void setup(){
	TRISB = 0x03;						// set RB0, RB1 to input
	TRISC = 0x00;
	TRISD = 0x00;
	/*
	RCONbits.IPEN = 1;					// Interrupt Routing 1-> low to low, 0-> low to high(d)
	INTCONbits.GIEL = 1;				// Enable Low Priority Interrupts Globally
	INTCON3bits.INT1IP = 0;				// Makes INT1 a Low Priority Interrupt
	INTCON2bits.INTEDG1 = 0;			//Set Rising/Falling Edge Sensitivty
	INTCON3bits.INT1IF = 0;				// Set INT1 Flag to 0
	INTCON3bits.INT1IE = 1;				// Enable INT1 Interrupt
	* /
	INTCONbits.RBIE = 1					//Enable the PORTB<7:4> Interrupt
	INTCON2bits.RBPU = 0;				//Enable PortB Pull-Ups
	*/

	T0CONbits.T0CS = 0;					//Clock from interal clock
	T0CONbits.PSA = 1;					//No Prescaler
	//T0CONbits.T0PS2 = 0;				//Set Prescaler to 1:2 (Lowest setting)
	//T0CONbits.T0PS1 = 0;				//Set Prescaler to 1:2 (Lowest setting)
	//T0CONbits.T0PS0 = 0;				//Set Prescaler to 1:2 (Lowest setting)
	INTCON2bits.TMR0IP = 1;				//High Priority
	T0CONbits.TMR0ON = 1;				//Turn on Timer 0
	INTCONbits.TMR0IE = 1;				//Enable Interrupt

	INTCONbits.GIE = 1;					// Enable Interrups Globally

	//Turn Everything On:
	PORTC = 0x03;
	PORTD = 0x00;
}

void display(unsigned char display){
	//display = which digit should be written to
	//character = what number/letter to display
	if(display == 0){
		PORTCbits.RC0 = 1;
		PORTCbits.RC1 = 0;
		PORTD = charLookup[dispChars&0b00001111];
	}else if(display == 1){
		PORTCbits.RC0 = 0;
		PORTCbits.RC1 = 1;
		PORTD = charLookup[dispChars>>4];
	}else{
		PORTCbits.RC0 = 0;
		PORTCbits.RC1 = 0;
	}		

}

void displayNext(unsigned char character){
	dispChars = dispChars << 4;
	dispChars += character;
}


void main(void) { 
	setup();
		displayNext(0x02);
		displayNext(0x0F);
	while(1)	{
		//key = scanKeyPad();
		//Delay, maybe?
	}
}



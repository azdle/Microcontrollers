#include <p18f4550.h>
#include <delays.h>

#pragma config PLLDIV=2, CPUDIV=OSC1_PLL2, USBDIV=2, IESO=ON, WDT=OFF
#pragma config BOR=OFF, PWRT=ON, LVP=OFF, FOSC=HSPLL_HS, FCMEN=OFF, VREGEN=OFF
#pragma config MCLRE=ON, STVREN=ON, LPT1OSC=ON, PBADEN=OFF

//######### Function Declarations ################
void high_isr(void);
void low_isr(void);
void showDigit(unsigned char);
void displayNumber(unsigned char);
unsigned char binToBcd(unsigned char);
void setup();

//######### Variables ################
unsigned char keypressed = 0;
unsigned char key;
unsigned char currKey;
unsigned char lastKey;
unsigned char dispChars;
unsigned char garbage;
unsigned char charLookup[] = { //Inverse Logic
	0b00000011, 0b10011111, 0b00100101, //0,1,2
	0b00001101, 0b10011001,	0b01001001,	//3,4,5
	0b01000001, 0b00011111, 0b00000001,	//6,7,8
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
	static unsigned char dispnum = 0;
	static unsigned int counter = 0;
	static unsigned char currentNum = 0;

	if(PIR1bits.TMR2IF == 1){
		PIR1bits.TMR2IF = 0;
		counter++;
		if(counter > 999){
			if(currentNum >= 100){
				currentNum = 0;
			}
			displayNumber(currentNum++);
			counter = 0;
		}
		dispnum ^= 1;
		showDigit(dispnum);
	}
}

#pragma interruptlow low_isr
void low_isr(void){

}

//######### Functions ################
void setup(){
	TRISB = 0xF0;						// set RB<7:4> to input
	TRISC = 0x00;
	TRISD = 0x00;

	PR2 = 250;							// Overflow value for TRM2
	T2CON = 0b00010111;					// 1:3 Postscale, 1:16 Prescale, TMR2 On
	PIR1bits.TMR2IF = 0;
	PIE1bits.TMR2IE = 1;
	RCONbits.IPEN = 0;					// Send all to high priority
	INTCONbits.PEIE = 1;				// Peripheral Interrupt Enable
	INTCONbits.GIE = 1;					// Enable Interrups Globally

	//Turn Everything On:
	PORTC = 0x03;
	PORTB = 0b00000000;
	PORTD = 0x00;
}

void showDigit(unsigned char display){
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

void displayNumber(unsigned char number){
		dispChars = binToBcd(number);
}

unsigned char binToBcd(unsigned char binaryNum){
	unsigned char tens;
	while(binaryNum > 99){	//Truncate to 2 Digits
		binaryNum =- 100;
	}
	tens = binaryNum/10;
	return (tens << 4) + binaryNum-(tens*10);
}


void main(void) { 
	setup();
	while(1)	{
	}
}



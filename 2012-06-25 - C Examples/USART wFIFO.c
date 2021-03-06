#include <p18f4550.h>

#pragma config PLLDIV=2, CPUDIV=OSC1_PLL2, USBDIV=2, IESO=ON, WDT=OFF
#pragma config BOR=OFF, PWRT=ON, LVP=OFF, FOSC=HSPLL_HS, FCMEN=OFF, VREGEN=OFF
#pragma config MCLRE=ON, STVREN=ON, LPT1OSC=ON, PBADEN=OFF

//######### Function Declarations ################
void high_isr(void);
void low_isr(void);

//######### Variables ################
#define BUFSIZE 16      /* power of two should compile efficiently */

volatile unsigned char txfifo[BUFSIZE];
volatile unsigned txrd=0, txwr=0;       /* read and write positions */
volatile unsigned char rxfifo[BUFSIZE];
volatile unsigned rxrd=0, rxwr=0;       /* read and write positions */

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
	if(PIR1bits.RCIF = 1){
		//Receive byte
	}
	
	if(PIR1bits.TXIF = 1){
		//Trasnmit Byte
		if(bytesInTxBuffer > 0){
			TXREG = readTxBuffer();
		}
	}	
}

#pragma interruptlow low_isr
void low_isr(void){
	
}

//######### Functions ################

void TX(char byteToSend){
	/* write a byte into my FIFO */
	txfifo[txwr++] = byteToSend;
	if[txwr > BUFSIZE - 1)
		txwr = 0;
}

char readTxBuffer(){
	//Read a byte from fifo
	return txfifo[txrd++];
	if[txrd > BUFSIZE - 1)
		txrd = 0;
}

char bytesInTxBuffer(){
	return txwr - txrd;
}


char RX(char byteToRecv){
	/* write a byte into my FIFO */
	rxfifo[rxwr++] = byteToRecv;
	if[rxwr > BUFSIZE - 1)
		rxwr = 0;
}

char readTxBuffer(){
	//Read a byte from fifo
	char ret = rxfifo[rxwr++]
	if[rxwr > BUFSIZE - 1)
		rxwr = 0;
	return ret;
}

char bytesInTxBuffer(){
	return rxwr - rxrd;
}

void setup(){
	TXSTA = 0b00100100;
	RCSTA = 0b10100000;
	BAUDCON = 0b00001000;
	SPBRGH = 2437 >> 8;
	SPBRG = 2437;
	
	PIR1bits.RCIE = 1;
	PIR1bits.TXIE = 1;
	INTCONbits.GIE = 1;
}


void main(void) { 
	setup();
	while(1)	{
	
	}
}



			include p18F4550.inc	;Include Rigister Names and Whatnot
			
			radix hex		;hex, dec, oct  DEFAULT = hex

;setup:		clrf	TRISD	;Set all of Port D to Output
;			setf	TRISA	;Set all of Port A to Input

;counter		set		0x00	
;a:			set		0x01	;Give a file register a label to be used later
;numLow:		set		0x02	;Store Low Byte First
;numHigh:	set		0x03	;Store High Byte Second


			org 0x00
			bra	start

;################### Interrupt Routines #################
int1:		org	0x08		;High Priority Interrupt
			;check which interrupt happened
			bcf	INTCON,	INT0IF	;clear event flag
			;do interrupt stuff
			retfie
			
			org 0x18		;Low Priority Interrupt
			;do other interrupt stuff
			retfie

;################## Registers ###########################
			cblock 0	;extra working registers
			wr0, wr1, wr2, wr3, wr4, wr5
			wr6, wr7, wr8, wr9
			counter
			endc

M_:			set	0x10
N_:			set 0x11
negstat:	set 0x13

divr:		set	0x14
divq:		set	0x15
divn:		set	0x16
divcount:	set	0x17

start:
;################## Pin Setup ##########################
pinset:		clrf	TRISD			;output
			setf	TRISB 			;input
			movlw	0x0E			;disable A/D
			movwf	ADCON1			; ^
enint:		bcf		INTCON,	INT0IF	;ensure event flag is not set
			bsf		INTCON,	INT0IE	;enables INT0 interrupt
			bsf		INTCON,	GIE		;enables global interrupts

;################## Macros #############################
;################## BS Loop ############################
bsloop:		incf	counter
			movff	counter, PORTC
			bra		bsloop
;################## Setting Stack Values to Eachother ##
setsame:	

;################## Stack Macros #######################
pushr:		macro	arg
			movff	arg, POSTINC1
			endm

popr:		macro	arg
			movf	POSTDEC1,	f
			movff	INDF1,		arg
			endm


;################## Example Programs ###################
setup:		lfsr	FSR1,	0x300

;################## Accessing Relative to FSRx #########
relstack:	movlw	0x01
			pushr	WREG
			movlw	0x02
			pushr	WREG
			movlw	0xFE	; = -2
			clrf	PLUSW1	;clears stack memory containng 0x01


;################## Using the Stack to Swap PRODx Registers ##

prodswap:	lfsr	FSR1,	0x300
			movlw	0x34
			movwf	PRODL
			movlw	0x26
			movwf	PRODH
			
			pushr	PRODL
			pushr	PRODH
			popr	PRODL
			popr	PRODH


;################## Using the Stack #####################

stackex:	lfsr 	FSR1,	0x300
			movlw	0x37
			pushr	WREG
			movlw	0x56
			popr	WREG

			bra		gotoend

;################## Division ###################################

division:	movlw	0x00	;zero
			movwf	divr
			movlw	0x0A	;dividend - top
			movwf	divq
			movlw	0x02	;divisor - bottom
			movwf	divn
			movlw	0x08	;16-Bit Number
			movwf	divcount
			

divloop:	bcf		STATUS,	C
			rlcf	divq
			rlcf	divr
			movf	divn,	w
			subwf	divr,	w
			btfss	STATUS,	N
				bsf		divq,	0
			btfss	STATUS,	N
				movwf	divr

			decf	divcount
			bnz		divloop
			bra 	gotoend
			
;########################### Multiplying with Negative Numbers ###########################

negmul:		movlw	0x01												;incomplete
			movwf	M_
			movlw	0xF3
			movwf	N_

			mulwf	M_

			movf	M_ ,w
			btfsc	STATUS,	N
				subwf	PRODH, f
			movf	N_,	w
			btfsc	STATUS, N
				subwf	PRODH,	f

			bra		gotoend


signmul:	movlw	0xFF
			bnn		tst_m		;skip neg_n
neg_n:		negf	WREG
			incf	negstat
aft_neg_n:	movwf	N_
tst_m:		movlw	0x06
			bnn		do_mult		;skip neg_m

neg_m:		negf	M_
			incf	negstat
aft_neg_m:	movwf	M_

do_mult:	mulwf	N_													;incloplete ^

			bra		gotoend


;#################### 'Secure' Crypto ###############################
crypto:		lfsr	FSR0,		0x020
			lfsr	FSR1,		0x030
cryptloop:	movf	POSTINC1,	w
			xorwf	POSTINC0,	f
			bnz		cryptloop

			bra		crypto

;#################### Find String Length ###########################
strlen:		set		0x03		;0x00 is the address, not the value

findlen:	lfsr	FSR0,		0x200
flloop		incf	strlen
			movf	POSTINC0,	f
			bnz		flloop
			decf	strlen
			bra		gotoend


countdown:	set		0x04

indiradd:	clrf	WREG			;Indiecct Add
			lfsr	FSR0,		0x200
idaloop:		addwf	INDF0,		w
			movf	POSTINC0,	f	;This is slightly inefficient, not fully utilizating properites
			decf	countdown
			bnz		idaloop
			
			bnz 	gotoend

countdown:	set		0x04

indircpy:	lfsr	FSR0,	0x200	;Indirect Copy
			lfsr	FSR1,	0x304
idcloop:	movff	POSTINC0,	POSTDEC1 ;Move IND0 to IND1 and increiment both addresses
			decf	countdown
			bnz		idcloop

			bra		gotoend

;################### Indirect Addressing ############################
indirect:	lfsr	FSR0,	0x030 ;This is an address
			lfsr	1,		0x29F
			lfsr	FSR2,	0x405

			movf	INDF0,	w
			addwf	INDF1,	w
			movwf	INDF2

			bra		gotoend

;################## Bitwise Operations ##############################
;bytewise:	setf	a		;Set 'a' (0x01) to 0xFF
;			clrf	a		;Set 'a' (0x01) to 0x00
;
;			movf	WREG,	w	;Mov WREG to itself, then:
;			bn		gotoend		;Check Status Register Negaitave Flag (Checks of last operation that affects N flag)
;
;			mulwf	a		;Multiply WREG with a into PRODH and PRODL
;			mullw	0x02	;Multiply WREG with literal (2)
;
;multibyte:	two three byte numbers 0x10 - 0x12 and 13-15 into 20-22

;multip32:	multiply two 16 bit numbers into 32 bit number
			;PL*QL
			;PH*QL
			;PL*QH
			;PH*PH
			;Sum of all = final product


;bitwise:	bcf 	PORTA,	3	;Bit Clear: Clears Bit 3 of Port A
;			btg		PORTB,	0	;Bit Toggle: Toggles Bit 0 of Port B
;
;
;
;flip10:		movlw	LOOPCOUNT
;			movwf	counter
;loop:		decf	counter
;			btg		PORTD,	0
;			bnz		loop

;paloop:		btfss	PORTA,	0
;				bra		paloop
;			btg		PORTD,	0
;			bra		paloop



;########################## End Loop #######################
gotoend:	bra 	gotoend			;Get Caught, Don't Finish

msg:		db		"Hello, World!"	;message (Note: Place after end loop)
			end
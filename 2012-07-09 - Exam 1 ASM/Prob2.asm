		include p18F4550.inc

x		equ	-4
num		set	0x25
		org	0x26
		lfsr	FSR2, 0x510
		movlw	x
		movff	POSTINC2, num
		setf	PLUSW2
loop:	decf	num
		tstfsz	num
		Bra	loop
loopbk: goto	loopbk
		end	

include p18F4550.inc	;Include Rigister Names and Whatnot

x		set	0x00
y		set	0x01
z		set	0x02

case1:	movlw	0x02
		mulwf	x
		movf	PRODL, w ;assuming 2*x is an 8 bit number
		subwf	y, w
		bnn		case2
		movwf	z
		bra		endloop
case2:	bnz		case3
		clrf	z
		bra		endloop
case3:	clrf	z
		decf	z

endloop:bra		endloop

		end
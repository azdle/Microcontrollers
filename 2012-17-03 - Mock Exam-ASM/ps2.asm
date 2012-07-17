		include p18F4550.inc	;Include Rigister Names and Whatnot

start:	movf	0x00, w
		addwf	0x01, w
		movwf	0x02

		end
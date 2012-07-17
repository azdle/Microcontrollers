		include p18F4550.inc	;Include Rigister Names and Whatnot

start:	movf	0x00, w
		addwf	0x01, w
		movwf	0x02

		end

;Yes, placed numbers in to 0x00, 0x01
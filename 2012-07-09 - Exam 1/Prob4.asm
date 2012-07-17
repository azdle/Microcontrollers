	include p18F4550.inc

st:	lfsr	FSR1,	0x245
	
p4:	movf INDF1, f
	bz	el
	movlw	'k'
	subwf	POSTINC1, w
	bnz		p4
	incf	PRODL
	bra		p4

el:	bra		el
	end
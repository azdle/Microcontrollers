        list    p=18F4550
        include p18f4550.inc
        radix   decimal
        ;the following line is not necessary for MPLAB, where you can set
        ;the configuration bits in the "configuration" menu.
        config FOSC=HSPLL_HS, PLLDIV=2, CPUDIV=OSC1_PLL2, USBDIV=2, IESO=ON, WDT=OFF, PWRT=ON, BOR=OFF
		config LVP=OFF, MCLRE=ON, PBADEN=OFF, STVREN=ON
        config FCMEN = ON
        cblock  0
        a3, a2, a1, a0		;add32 value 1
		b3, b2, b1, b0		;add32 value 2
		c3, c2, c1, c0		;add32 return
		ph, pl				;mul16 value 1
		qh, ql				;mul16 value 2
		f3, f2, f1, f0		;mul16 return
		r3, r2, r1, r0		;result of seedx
		s3, s2, s1, s0		;result of seedy
		seedx3, seedx2, seedx1, seedx0
		seedy3, seedy2, seedy1, seedy0
        endc

setup:	
x1:		equ		0x46
x0:		equ		0x50
y1:		equ		0x78
y0:		equ		0xB7
		movlw	0x1F
		movwf	seedx3
		movlw	0x12
		movwf	seedx2
		movlw	0x3B
		movwf	seedx1
		movlw	0xB5
		movwf	seedx0
		movlw	0x15
		movwf	seedy3
		movlw	0x9A
		movwf	seedy2
		movlw	0x55
		movwf	seedy1
		movlw	0xE5
		movwf	seedy0


		setf	PORTD	;set all to 1
		clrf	TRISD	;set to outputs
		bcf		INTCON2,	RBPU ;Turn on Internal Pull-ups
		setf	TRISB	;set to inputs


main:	btfsc	PORTB,	7
			bra	main
		call	random
		call	delay
		bra		main

delay:	setf	PRODH 	;65.28 ms, Way more than enough.
dlpout:	setf	PRODL
dlpin:	decf PRODL		
		bnz		dlpin
		decf	PRODH		
		bnz		dlpout
		return


random:	movff	seedx0, ql
		movff	seedx1, qh
		movlw	x1
		movwf	ph
		movlw	x0
		movwf	pl
		call	mul16
		movff	f0, r0
		movff	f1, r1
		movff	f2, r2
		movff	f3, r3
		movf	seedx2, w		;'24bit' add
		addwf	r0,	f
		movf	seedx3, w
		addwfc	r1,	f
		btfsc	STATUS,	C
			incf		r2
		

		movff	seedy0,	ql
		movff	seedy1,	qh
		movlw	y1
		movwf	ph
		movlw	y0
		movwf	pl
		call	mul16
		movff	f0, s0
		movff	f1, s1
		movff	f2, s2
		movff	f3, s3
		movf	seedy2, w
		addwf	s0,	f
		movf	seedy3, w
		addwfc	s1,	f
		btfsc	STATUS,	C
			incf		s2

		movff	r0,	seedx0
		movff	r1,	seedx1
		movff	r2,	seedx2
		movff	r3,	seedx3
		movff	s0,	seedy0
		movff	s1,	seedy1
		movff	s2,	seedy2
		movff	s3,	seedy3

		movff	r0,	a0
		movff	r1,	a1
		movff	r2,	a2
		movff	r3,	a3
		movff	s0,	b0
		movff	s1,	b1
		movff	s2,	b2
		movff	s3,	b3

		call 	add32

		addlw	0x00	;Enusre Carry Bit is 0
		rrcf	c3,		f
		rrcf	c2,		f
		rrcf	c1,		f
		rrcf	c0,		f

		movff	c0,		PORTD

		return

add32:	
		movf 	a0,	w
		addwf	b0,	w
		movwf	c0
		movf 	a1,	w
		addwfc	b1,	w
		movwf	c1
		movf 	a2,	w
		addwfc	b2,	w
		movwf	c2
		movf 	a3,	w
		addwfc	b3,	w
		movwf	c3


mul16:	clrf	f2
		clrf	f3
		movf	pl,		w
		mulwf	ql
		movff	PRODL,	f0
		movff	PRODH,	f1
		movf	ph,		w
		mulwf	ql
		movf	PRODL,	w
		addwf	f1,		f
		btfsc	STATUS,	C
			incf		f2
		movf	PRODH,	w
		addwf	f2,		f
		btfsc	STATUS,	C
			incf		f3
		movf	pl,		w
		mulwf	qh
		movf	PRODL,	w
		addwf	f1,		f
		btfsc	STATUS,	C
			incf		f2
		movf	PRODH,	w
		addwf	f2,		f
		btfsc	STATUS,	C
			incf		f3
		movf	ph,		w
		mulwf	qh
		movf	PRODL,	w
		addwf	f2,		f
		btfsc	STATUS,	C
			incf		f3
		movf	PRODH,	w
		addwf	f3,		f
		
		
		return
		
		end
        list    p=18F4550
        include p18f4550.inc
        ;org    0
        ;bra   main
        ;these are not necessary, but I always put them in anyway.
        ;org    8
        ;retfie
        ;org    0x18
        ;retfie
        ;org    0x2C
x:		db		12,	22,	33,	0

main:   ;movlw	5
		;movwf	0x00
		;movlw	6
		;movwf	0x01
		;movlw	7
		;movwf	0x02		

		movf	x,	W
		addwf	x+1,	W
		addwf	x+2,	W
		movwf	x+3

lpbk:   bra		lpbk		; == while(1){}
        end
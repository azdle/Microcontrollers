        list    p=18F4550
        include p18f4550.inc
        radix   decimal
        org    0
        goto   main
        ;these are not necessary, but I always put them in anyway.
        org    0x08
        retfie
        org    0x18
        retfie
        org    0x30
main:   movlw	128
		movff	0, PRODL
		clrf	0
		incf	0,f
loop:	addwf	0,w
		bnz		loop
lpbk:	bra		lpbk
        end
        list    p=18F4550
        include p18f4550.inc
        radix   decimal
        ;the following line is not necessary for MPLAB, where you can set
        ;the configuration bits in the "configuration" menu.
        config FOSC=HSPLL_HS, PLLDIV=2, CPUDIV=OSC1_PLL2, USBDIV=2, IESO=ON, WDT=OFF, PWRT=ON, BOR=OFF
		config LVP=OFF, MCLRE=ON, PBADEN=OFF, STVREN=ON
        config FCMEN = ON
        cblock  0
        count2,count1,count0
        bits
        endc
        org    0
        goto   main
        ;these are not necessary, but I always put them in anyway.
        org    4
        retfie
        org    0x18
        retfie
        org    0x2C
main:   clrf   PORTC
        clrf   TRISC       ;set all bits of port C to output
        setf   PORTC       ;and set them all to 0
        clrf   count0
        clrf   count1
oloop:  clrf   count2      ;1 instruction cycle clock
loop:   dcfsnz count0,f    ;1 instruction cycle clock unless result is not 0 (then 2)
        bra    nlabel      ;2 instruction cycle clocks
        bra    loop        ;2 instruction cycle clocks (loop is 255*4 + 3 clocks)
nlabel: nop                ;1 instruction cycle clock to make this an even 1024 clocks
        incfsz count1,f    ;1 instruction cycle clocks unless result is 0 (then 2)
        bra    loop        ;2 instruction cycle clocks (loop is 255*1027 + 1026 clocks)
        incf   count2,f    ;1 instruction cycle clocks
        btfss  count2,4    ;3 instruction cycle clocks (why?) if bit 4 of count2 is 1, 1 else
        goto   loop        ;2 instruction cycle clocks (this instruction is skipped when count2 has
                           ;been incremented 16 times.  (loop takes 16*262915 clocks)
        incf   bits,f      ;1 instruction cycle clocks
        movf   bits,w      ;1 instruction cycle clocks
        movwf  PORTC       ;1 instruction cycle clocks -- so this will be executed every 4206646 clocks
        goto   oloop       ;2 instruction cycle clocks
        end


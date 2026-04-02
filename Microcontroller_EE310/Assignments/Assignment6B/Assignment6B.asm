;--------------------------------------------------------
; Title: Hexadecimal Counter using 7-Segment Display
;--------------------------------------------------------
; Purpose: This program increments, decrements, and resets
; a hexadecimal value (0–F) displayed on a 7-segment display.
; Dependencies: AssemblyConfig.inc, xc.inc
; Compiler: MPLAB X IDE v6.30
; Author: Rachel Markman
; OUTPUTS: RD0–RD7 → 7-segment display segments
; INPUTS:
;   RB0 → Increment button 
;   RB1 → Decrement button
;--------------------------------------------------------

#include "AssemblyConfig.inc"
#include <xc.inc>

innerLoop EQU 125
outerLoop EQU 100
thirdLoop EQU 12

;---------------------
; Definitions
;---------------------

#define BUTTONA     PORTB, 0
#define BUTTONB     PORTB, 1
#define DISPLAY     LATD

;---------------------
; Program Constants
;---------------------

REG10 EQU 0x0A
REG25 EQU 0x19
REG31 EQU 0x31
REG32 EQU 0x32
REG33 EQU 0x33

    PSECT absdata,abs,ovrld
    ORG 0x30

Start:
    ;-----------------------------
    ; Initialize PortB for buttons
    ;-----------------------------
    BANKSEL PORTB
    CLRF PORTB
    BANKSEL LATB
    CLRF LATB
    BANKSEL ANSELB
    CLRF ANSELB
    BANKSEL TRISB
    MOVLW 0x03
    MOVWF TRISB

    ;-----------------------------
    ; Initialize PortD for display
    ;-----------------------------
    BANKSEL PORTD
    CLRF PORTD
    BANKSEL LATD
    CLRF LATD
    BANKSEL ANSELD
    CLRF ANSELD
    BANKSEL TRISD
    CLRF TRISD

    ;-----------------------------
    ; Load hex values into RAM
    ;-----------------------------
    LFSR 0, REG10

    MOVLW 0x3F
    MOVWF REG10
    MOVLW 0x06
    MOVWF 0x0B
    MOVLW 0x5B
    MOVWF 0x0C
    MOVLW 0x4F
    MOVWF 0x0D
    MOVLW 0x66
    MOVWF 0x0E
    MOVLW 0x6D
    MOVWF 0x0F
    MOVLW 0x7D
    MOVWF 0x10
    MOVLW 0x07
    MOVWF 0x11
    MOVLW 0x7F
    MOVWF 0x12
    MOVLW 0x6F
    MOVWF 0x13
    MOVLW 0x77
    MOVWF 0x14
    MOVLW 0x7C
    MOVWF 0x15
    MOVLW 0x39
    MOVWF 0x16
    MOVLW 0x5E
    MOVWF 0x17
    MOVLW 0x79
    MOVWF 0x18
    MOVLW 0x71
    MOVWF REG25

    ;Start at 0
    LFSR 0, REG10
    MOVF INDF0, W
    MOVWF DISPLAY

;----------
; Main loop
;----------

MainLoop:
    BTFSS BUTTONA
    GOTO CheckB_WithA

    BTFSS BUTTONB
    GOTO decrease

    GOTO MainLoop

CheckB_WithA:
    BTFSS BUTTONB
    GOTO zero

    GOTO increase

;-----------------------
; Increment / Decrement
;-----------------------

increase:
    MOVF INDF0, W
    CPFSEQ REG25
    GOTO increment
    GOTO zero

decrease:
    MOVF INDF0, W
    CPFSEQ REG10
    GOTO decrement
    GOTO max

increment:
    INCF FSR0L, F
    MOVF INDF0, W
    MOVWF DISPLAY
    CALL loopDelay
    GOTO MainLoop

decrement:
    DECF FSR0L, F
    MOVF INDF0, W
    MOVWF DISPLAY
    CALL loopDelay
    GOTO MainLoop

max:
    LFSR 0, REG25
    MOVF INDF0, W
    MOVWF DISPLAY
    CALL loopDelay
    GOTO MainLoop

zero:
    LFSR 0, REG10
    MOVF INDF0, W
    MOVWF DISPLAY
    CALL loopDelay
    GOTO MainLoop

;-------
; Delay
;-------

loopDelay:
    MOVLW innerLoop
    MOVWF REG31
    MOVLW outerLoop
    MOVWF REG32
    MOVLW thirdLoop
    MOVWF REG33

loop1:
    DECF REG31, F
    BNZ loop1

    MOVLW innerLoop
    MOVWF REG31

    DECF REG32, F
    BNZ loop1

    MOVLW outerLoop
    MOVWF REG32

    DECF REG33, F
    BNZ loop1

    RETURN

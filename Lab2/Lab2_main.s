;************************************ LAB 02 *************************************************
; main.s
; Benjamin Meads, Nick Templeton
; 9/19/23
;
; Lab02: Write code that Starts LED in OFF state, one button press moves the LED to the ON state. 
;		 A second button press moves the LED to the dim/flashing state. A third button press moves
;		 the LED to the OFF state.
;
; Pseudocode
;	Debounce:
;		waiting:
;			cmp PC13, 0x0
;			be  waiting
;		Pressed:
;			cmp PC13, 0x0
;			be  waiting
;		Held:
;			cmp PC13, 0x1
;			be  held
;		Released:
;			cmp PC13, 0x1
;			be  held
;		add %rdx, 0x1		
;	main:
;		cmp %rdx, 0x1
;		bne "end"
;		ldr PA5, 0x1
;		cmp %rdx, 0x2
;		bne	L2
;		ldr %rdx, 0x0
;		ldr PA5, 0x0
;		rep ret
;	L2:
;		ldr r1, [r0, #GPIO_ODR]
;		orr r1, r1, #0x00000020
;		str r2, [r0, #GPIO_ODR]
;		
;		ldr r1, [r0, #GPIO_ODR]
;		and r1, r1 #0xfffffDf
;		str r3, [r0, #GPIO_ODR]
;**************************************************************************************************
	INCLUDE core_cm4_constants.s
	INCLUDE stm32l476xx_constants.s
	
	AREA    main, CODE, READONLY
	EXPORT	__main
	ENTRY			
				
__main	PROC
		
    ; Enable the clock to GPIO Port A	
	LDR r0, =RCC_BASE ;creates space for clock
	LDR r1, [r0, #RCC_AHB2ENR] ;enables the clock (read)
	ORR r1, r1, #RCC_AHB2ENR_GPIOAEN ;enables the clock for GPIO A(modify)
	ORR r1, r1, #RCC_AHB2ENR_GPIOCEN ;enables the clock for GPIO B (modify)
	STR r1, [r0, #RCC_AHB2ENR] ;enables the clock (write)
	; MODE: 00: Input mode, 01: General purpose output mode
    ;       10: Alternate function mode, 11: Analog mode (reset state)
	
	;Input value is 00
	;Output value is 01

	LDR r0, =GPIOA_BASE ;setting up memory space for GPIOA
		LDR r1, [r0, #GPIO_MODER] ;reading in GPIO Moder offset
		AND r1, r1, #0xFFFFF3FF ;setting PIN 5 to 00
		ORR r1, r1, #0x00000400 ;setting PIN 5 to 01
		STR r1, [r0, #GPIO_MODER] ;store the value in r1. Into memory location r0 + GPIO_MODR (offset)
	
	;This sets the button up, setting it to be an input
	LDR r5, =GPIOC_BASE ;setting up memory space for GPIOA
		LDR r1, [r5, #GPIO_MODER] ;reading in GPIO Moder offset
		AND r1, r1, #0xF3FFFFFF ;setting PIN 13 to 00
		STR r1, [r5, #GPIO_MODER] ;store the value in r1. Into memory location r0 + GPIO_MODR (offset)
	

Big_Loop

;/////////////////////////////////////////////////////////////////////////////////////////// Debouncer-Section ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	

waiting
	bl PC13
	cmp r2, #0x1
	beq waiting
pressed
	bl PC13
	cmp r2, #0x1
	beq waiting
held
	bl PC13
	cmp r2, #0x0
	beq held
release
	bl PC13
	cmp r2, #0x0
	beq release
	add r4, #0x1
;///////////////////////////////////////////////////////////////////////////////////////// Switch Statement Section/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
L1
	cmp r4, #0x1			;This is if the button was pressed One Time.					;It will jump to L3, L3 will turn the LED on, then begin the BIG LOOP again
	bne L4
	bl LED_on
L4
	cmp r4, #0x2			;This is if the button was pressed Two Times
	bne L5				;It will jump to L4, L4 will tun the LED on, and off, then begin the big loop again
	bl LED_Blink
L5
	cmp r4, #0x3			;This is if the button was pressed Three times
	bne L6
	bl LED_off				; This will jump to L5, L5 will turn the LED off, then begin the BIG LOOP
L6
	b Big_Loop
;//////////////////////////////////////////////////////////////////////////////////////////Function Calls////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
LED_on
	; Turn on the LED
		LDR r1, [r0, #GPIO_ODR] ;Load r1 with the contends of Output data register
		ORR r1, r1, #0x00000020 ;setting PIN 20 to HIGH														
		STR r1, [r0, #GPIO_ODR];store contents of r1 into memory location r0 + GPIO_ODR
		bx LR
LED_Blink
		LDR r1, [r0, #GPIO_ODR] ;Load r1 with the contends of Output data register
		ORR r1, r1, #0x00000020 ;setting PIN 20 to HIGH														
		STR r1, [r0, #GPIO_ODR];store contents of r1 into memory location r0 + GPIO_ODR

		LDR r1, [r0, #GPIO_ODR] ;Load r1 with the contends of Output data register
		AND r1, r1, #0xFFFFFFDF ;setting PIN 20 to LOW													
		STR r1, [r0, #GPIO_ODR];store contents of r1 into memory location r0 + GPIO_ODR
		bl PC13
		cmp r2, #0x1
		beq LED_Blink
		b L5
LED_off
		LDR r1, [r0, #GPIO_ODR] ;Load r1 with the contends of Output data register
		AND r1, r1, #0xFFFFFFDF ;setting PIN 20 to LOW													
		STR r1, [r0, #GPIO_ODR];store contents of r1 into memory location r0 + GPIO_ODR
		LDR r4, =0x0
		bx LR
PC13	
	LDR r2, [r5, #GPIO_IDR] ;Load r2 with the contents of the input data register
	LSR r2, #0xD
	AND r2, r2, #0x1 ;
	bx LR
		
stop 	B 		stop

	ENDP
					
	ALIGN			

	END
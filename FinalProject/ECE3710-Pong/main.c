#include "stm32l476xx.h"
#include "LCD.h"
#include "TIM.h"
#include "SPI.h"
#include "pin_setup.h"
#include "LED_Matrix.h"
#include "EXTI.h"

/******************************************************************************
Project Title: Pong

Summary:	

Inputs:	
	BTN1 = PC6, 
	BTN2 = PC7, 
	BTN3 = PC8, 
	BTN4 = PC9
Outputs: 
	LCD1 = PB0-5,
	LCD2 = PC0-5, 
	LED_Array = PA15(CS), PA7(MOSI), PA5(clk)
	
	

*******************************************************************************

Psuedocode:

Begin Main
	init LCD1 & LCD2
	init SPI for LED Matrix
	init P1 & P2 buttons
	init buzzer
	
	loop (could use interupts)
		ball movement function
		send data to led matrix via SPI
		
		player 1 paddle movement function
		send data to LCD1
		
		player 2 paddle movement function
		send data to LCD2
		
		if player 1 scored
			update LCD1
		if player 2 scored
			update LCD2
			
		if player 1 has 7 points
			clear LED Matrix
			display player 1 winner
			set off buzzer
	endloop
End

******************************************************************************/

int main(void)
{
	//init LCD1 & LCD2
	LCD_Init(GPIOC);
	LCD_Init(GPIOB);
	
	//init Buttons
	EXTI_Initialize();
	
	//init SPI for LED Matrix
	SPI_Init(SPI1);
	LED_Matrix_Init();
	
	
	while(1){
		Ball_Movement();
		delay(34464);
		delay(34464);
		delay(34464);
		//delay(34464);
	}
		
	//init P1 & P2 Buttons
//	pinMode(GPIOC, 6, INPUT);
//	pinMode(GPIOC, 7, INPUT);
//	pinMode(GPIOC, 8, INPUT);
//	pinMode(GPIOC, 9, INPUT);
	
	
}

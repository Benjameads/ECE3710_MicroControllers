#include "TIM.h"
#include "stm32l476xx.h"
#include "pin_setup.h"

/******************************************************************************
Function Title: delay

Summary:	This produces a sytem accurate ms delay.

Inputs:	NA
Outputs: NA

*******************************************************************************

Psuedocode:

Begin 
	enable  HSI clock
	enable clock for TIM7
	disable TIM7
	clear TIM7 status reg
	clear TIM7 counter
	Set prescaler to produce a 1Khz counter (16000-1 for HSI)
	Set ARR to passed in ms value -1
	Enable TIM7
	loop till TIM7 flag goes off
End

******************************************************************************/
void delay(uint16_t ns) {
	if ( ns == 0 ) //Sanity check
		return;
	
	RCC->CR |= RCC_CR_HSION;
	while((RCC->CR & RCC_CR_HSIRDY) == 0);	//Check if clk is ready
	
	// Enable timer 7 clock
	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM7EN;
	
	TIM7->CR1 &= ~TIM_CR1_CEN; // Disable timer 7
	TIM7->SR = 0; // Clear status register
	TIM7->CNT = 0; // Clear counter
	TIM7->PSC = 16-1; // 16 MHz/ 16 = 1 MHz
	TIM7->ARR = ns - 1; // ARR + 1 cycles
	TIM7->CR1 |= TIM_CR1_CEN; // Enable timer 7
	// Loop until UIF is set
	while ( (TIM7->SR & TIM_SR_UIF) == 0 );
}


#include "EXTI.h"
#include "stm32l476xx.h"
#include "LCD.h"
#include "pin_setup.h"
#include "LED_Matrix.h"
#include "LCD.h"

//extern volatile uint32_t p1_pos, p2_pos, lost;

#define UP		1
#define DOWN	0
void EXTI_Initialize(void) 
{
	//Enable SYSCFG clock
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
	
	//turn on clock for A port
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;	
	
	//Set up btn Moder
	pinMode(GPIOA, 0, INPUT);
	pinMode(GPIOA, 1, INPUT);
	pinMode(GPIOA, 4, INPUT);
	pinMode(GPIOA, 9, INPUT);
	
	//Set up btn PUPDR
	setPullUpDown(GPIOA, 0, PULLUP);
	setPullUpDown(GPIOA, 1, PULLUP);
	setPullUpDown(GPIOA, 4, PULLUP);
	setPullUpDown(GPIOA, 9, PULLUP);
	
	//This is setting PC 0,1,2 to our source of EXTI2
	SYSCFG->EXTICR[0] &= (~SYSCFG_EXTICR1_EXTI0 & ~SYSCFG_EXTICR1_EXTI1);
	SYSCFG->EXTICR[1] &= ~SYSCFG_EXTICR2_EXTI4;
	SYSCFG->EXTICR[1] &= ~SYSCFG_EXTICR3_EXTI9;
	SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI0_PA | SYSCFG_EXTICR1_EXTI1_PA;
	SYSCFG->EXTICR[1] |= SYSCFG_EXTICR2_EXTI4_PA;
	SYSCFG->EXTICR[2] |= SYSCFG_EXTICR3_EXTI9_PA;
	
	/*
	enabling rising edge and disabling falling edge for EXTI 0,1,2,3
	EXTI->RTSR1 |= EXTI_RTSR1_RT0 | EXTI_RTSR1_RT1 | EXTI_RTSR1_RT2 | EXTI_RTSR1_RT3;
	EXTI->FTSR1 &= ~EXTI_FTSR1_FT0 & ~EXTI_FTSR1_FT1 & ~EXTI_FTSR1_FT2 & ~EXTI_FTSR1_FT3;
	*/
	
	//enabling falling edge and disabling rising edge for EXTI 0,1,2,3
	EXTI->RTSR1 &= ~EXTI_RTSR1_RT0 & ~EXTI_RTSR1_RT1 & ~EXTI_RTSR1_RT4 & ~EXTI_RTSR1_RT9;
	EXTI->FTSR1 |= EXTI_FTSR1_FT0 | EXTI_FTSR1_FT1 | EXTI_FTSR1_FT4 | EXTI_FTSR1_FT9;
	
	//Enable the EXTI 0-3 Interupt for the source
	EXTI->IMR1 |= (EXTI_IMR1_IM0 | EXTI_IMR1_IM1 | EXTI_IMR1_IM4 | EXTI_IMR1_IM9);
	
	//setting priority
	NVIC_SetPriority(EXTI0_IRQn, 1);
	NVIC_SetPriority(EXTI1_IRQn, 2);
	NVIC_SetPriority(EXTI4_IRQn, 3);
	NVIC_SetPriority(EXTI9_5_IRQn, 4);
	
	//Enable the EXTI 0-2 Interupt for the dest
	NVIC_EnableIRQ(EXTI0_IRQn);
	NVIC_EnableIRQ(EXTI1_IRQn);
	NVIC_EnableIRQ(EXTI4_IRQn);
	NVIC_EnableIRQ(EXTI9_5_IRQn);
}

//Pl - UP
void EXTI0_IRQHandler (void) {
	if ((EXTI->PR1 & EXTI_PR1_PIF0) == EXTI_PR1_PIF0)
	{
		//insert logic to move up paddle on Player1 LCD
		//if(!lost)
			LCD1_paddle_control(UP);
		
//		if(lost){
//			for (int i=0; i < 17; i++){
//				paddle_1[i] = paddle_reset[i];
//				paddle_2[i] = paddle_reset[i];
//			}
//			p1_pos = 7;
//			p2_pos = 7;
//			p1_score = 0;
//			p2_score = 0;
//			LCD_Clear(GPIOB);
//			LCD_Clear(GPIOC);
//			LCD_DisplayString(1,paddle_1, GPIOB);
//			LCD_DisplayString(1,paddle_2, GPIOC);
//			LED_Matrix_Init();
//			 lost = 0;
//		}
		
		//Clear Interrut request
		EXTI->PR1 |= EXTI_PR1_PIF0;
	}
}

//Pl - DOWN
void EXTI1_IRQHandler(void) {
	if ((EXTI->PR1 & EXTI_PR1_PIF1) == EXTI_PR1_PIF1)
	{
		//insert logic to move down paddle on Player1 LCD
		//if(!lost)
			LCD1_paddle_control(DOWN);
		//Clear Interrut request
		EXTI->PR1 |= EXTI_PR1_PIF1;
	}	
}

//P2 - UP
void EXTI4_IRQHandler(void) {
	if ((EXTI->PR1 & EXTI_PR1_PIF4) == EXTI_PR1_PIF4)
	{
		//insert logic to move up paddle on Player1 LCD
		//if(!lost)
			LCD2_paddle_control(UP);
		//Clear Interrut request
		EXTI->PR1 |= EXTI_PR1_PIF4;
	}	
}

//P2 - DOWN
void EXTI9_5_IRQHandler(void) {
	if ((EXTI->PR1 & EXTI_PR1_PIF9) == EXTI_PR1_PIF9)
	{
		//insert logic to move down paddle on Player2 LCD
		//if(!lost)
			LCD2_paddle_control(DOWN);
		//Clear Interrut requests
		EXTI->PR1 |= EXTI_PR1_PIF9;
	}	
}

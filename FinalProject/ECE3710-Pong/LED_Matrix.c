#include "stm32l476xx.h"
#include "LED_Matrix.h"
#include "pin_setup.h"
#include "SPI.h"
#include "TIM.h"
#include "LCD.h"

/******************************************************************************
Function Title: LED_Matrix_Init

Summary:	This function sets up the LED Matrix.

Inputs:	NA
Outputs: SPI to LED Matrix(PA5, PA7, PB6)

*******************************************************************************

Psuedocode:

Begin 
	send scan limit command, Address 0x0B, Data 0x07
End

******************************************************************************/
#define LEFT	1
#define RIGHT	0
#define UP		1
#define DOWN	0
static volatile uint32_t LED[8];
static volatile uint8_t vert, horz, row;

void LED_Matrix_Init(void){
	//		com is code,	on 			,scan lmt,	no decode,	clear screen
	uint16_t com[15] = {0x0C01,	0x0B07,	0x0900,	0x0F00,	0x0100,	0x0200,	0x0300,	0x0400,	0x0500,	0x0600,	0x0700,	0x0800, 0x0000, 0x0000, 0x0000};
	volatile int i;
	
	//initialize 2-d array to value 0
	for(i = 0; i<8; i++){
		LED[i] = 0x00000000;
	}
	horz = RIGHT;
	vert = DOWN;
	LED[0] = 0x40000000;
	row = 0;
	
	//send scan limit command, Address 0x0B, Data 0x07, contains 3 no-op commands
	//SPI_Write(SPI1, com, 15);
	for(i=0; i<15; i++){
		digitalWrite(GPIOB, 6, 0);
		SPI_Write(SPI1, &(com[i]), 1);
		GPIOB->ODR |= 0x00000040U;
		//digitalWrite(GPIOB, 6, 1);
	}
}

void LED_Matrix_Print (volatile uint32_t SCREEN[8]){
	volatile uint32_t i, k=0;
	volatile int j;
	uint16_t write[32], addr = 0x0100;
	
	for(i=0; i<8; i++){
		for(j=3; j >= 0; j--){
			write[k] = (uint16_t)( (addr*(i+1)) | ((SCREEN[i]>>(j*8))&(0xFF)) );
			k++;
		}
	}
	
	for(i=0; i<8; i++){
		GPIOB->ODR &= 0xFFFFFFBFU;
		SPI_Write(SPI1, &(write[i*4]), 4);
		GPIOB->ODR |= 0x00000040U;
	}
}

/******************************************************************************
Function Title: Ball_Movement

Summary:	This function moves the ball across the LED Matrix

Inputs:	NA
Outputs: SPI to LED Matrix(PA5, PA7, PB6)

*******************************************************************************

Psuedocode:

Begin
	if on edge
		change direction
	
	if horz == left
		shift ball left
	if horz == right
		shift ball right
	if vert == up
		shift ball up
	if vert == down
		shift ball down
	
	send LED array to via SPI
End

******************************************************************************/
void Ball_Movement(void){
	volatile uint32_t i;
	unsigned char you_lose[17] = {'X', 'X', 'X', 'X', 'X', 'X', 'X', 'X', 'X', 'X', 'X', 'X', 'X', 'X', 'X', 'X', 0};
	
	//if on upper edge
	if(LED[0] != 0x00000000)
		vert = DOWN; //move right
	//if on lower edge
	if(LED[7] != 0x00000000)
		vert = UP;
	
	
	//if on right edge
	if(LED[row] == 0x00000001){
		horz = LEFT;
		if(row != (p2_pos-5)){
			p1_score++;
			for (i=15; i > (15-p1_score); i--){
				p1_points[i] = 0xFF;
			}
			LCD_DisplayString(0, p1_points, GPIOB);
			if(p1_score >= 7){
				p2_pos = 0;
				LCD_DisplayString(1, you_lose, GPIOC);
				lost = 1;
				while(lost);
			}
		}
	}
	
	//if on left edge
	else if(LED[row] == 0x80000000){
		horz = RIGHT;
		if(row != 11-p1_pos){
			p2_score++;
			for (i=0; i < p2_score; i++){
				p2_points[i] = 0xFF;
			}
			LCD_DisplayString(0, p2_points, GPIOC);
			if(p2_score >= 7){
				p1_pos = 0;
				LCD_DisplayString(1, you_lose, GPIOB);
				lost = 1;
				while(lost);
			}
		}
	}
	
	//if vert == up
	if(vert == UP){
		if(horz == RIGHT){
			//shift ball left and up
			LED[row-1] = (uint32_t)(LED[row]>>1);
		}
		if(horz == LEFT){
			//shift ball left and down
			LED[row-1] = (uint32_t)(LED[row]<<1);
		}
		LED[row] = 0x00000000;
		row--;
	}
	//if horz == right
	else if(vert == DOWN){
		if(horz == RIGHT){
			//shift ball right and up
			LED[row+1] = (uint32_t)(LED[row] >> 1);
		}
		if(horz == LEFT){
			//shift ball right and down
			LED[row+1] = (uint32_t)(LED[row] << 1);
		}
		LED[row] = 0x00000000;
		row++;
	}
	
	LED_Matrix_Print(LED);
}


	

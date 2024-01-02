#include "LCD.h"
#include "stm32l476xx.h"

volatile uint32_t p1_pos = 7, p2_pos = 7, lost = 0;

void delay_ms(unsigned int ms) {
	volatile uint32_t i, j;
	for (i=0; i < ms; i++){
		for(j=0; j<200; j++);
	}
}

void Pulse_E (GPIO_TypeDef * GPIOx){
	GPIOx->ODR |= 0x00000020;	//Turn on Enable
	delay_ms(1);
	GPIOx->ODR &= 0xFFFFFFDF;	// turn off Enable /*This was the big error, needed an & vs an | */
	delay_ms(1);
}

void LCD_WriteCom(unsigned char com, GPIO_TypeDef * GPIOx) {
	GPIOx->ODR &= 0xFFFFFFEF;	/*rs set to 0*/
	delay_ms(1);
	
	GPIOx->ODR &= 0xFFFFFFF0;	//clear previous nibble
	GPIOx->ODR |= com >> 4;		//send upper nibble
	Pulse_E(GPIOx);
	
	GPIOx->ODR &= 0xFFFFFFF0;	//clear previous nibble
	GPIOx->ODR |= com & 0x0F;	//send lower nibble
	Pulse_E(GPIOx);
}

void LCD_WriteData(unsigned char dat, GPIO_TypeDef * GPIOx) {
	GPIOx->ODR &= 0xFFFFFFEF; /*rs set to 0*/
	GPIOx->ODR |= 0x00000010; /*rs set to 1*/
	delay_ms(1);
	
	GPIOx->ODR &= 0xFFFFFFF0;	//clear previous nibble
	GPIOx->ODR |= dat >> 4;		//send upper nibble
	Pulse_E(GPIOx);
	
	GPIOx->ODR &= 0xFFFFFFF0;	//clear previous nibble
	GPIOx->ODR |= dat & 0x0F;	//send lower nibble
	Pulse_E(GPIOx);
}

void LCD_Init(GPIO_TypeDef * GPIOx){
	//PB0-3 = DB4-7
	//PB4 = RS
	//PB5 = E
	volatile uint32_t x;
	
	/*Enable the Internal High Speed oscillato (HSI)*/
	RCC->CR |= RCC_CR_HSION;
	while((RCC->CR & RCC_CR_HSIRDY) == 0);	//Check if clk is ready
	
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;	//turn on clock for B port
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;	//turn on clock for C port
	
	//set up MODER
	//Px0-5 set to output mode
	x = GPIOx->MODER;
	x &= 0xFFFFF000;
	x |= 0x00000555;
	GPIOx->MODER = x;
	
	//Part1 reset display
	GPIOx->ODR &= 0xFFFFFFF0;	//clear previous nibble
	GPIOx->ODR |= 0x00000003;	
	Pulse_E(GPIOx);
	delay_ms(4);
	
	//Part2 reset display
	GPIOx->ODR &= 0xFFFFFFF0;	//clear previous nibble
	GPIOx->ODR |= 0x00000003;	
	Pulse_E(GPIOx);
	
	//Part3 reset display
	GPIOx->ODR &= 0xFFFFFFF0;	//clear previous nibble
	GPIOx->ODR |= 0x00000003;	
	Pulse_E(GPIOx);
	
	//Part 4 reset display
	GPIOx->ODR &= 0xFFFFFFF0;	//clear previous nibble
	GPIOx->ODR |= 0x00000002;	
	Pulse_E(GPIOx);
	
	LCD_WriteCom(0x28, GPIOx);	/*4-bit mode, 2 Row, 5x8 Latice*/
	delay_ms(1);
	LCD_WriteCom(0x0C, GPIOx);	/*display on, cursor off, cursor blinking off*/
	delay_ms(1);
	LCD_WriteCom(0x14, GPIOx);	/*cursor moves left to right, no display scroling*/
	delay_ms(1);
	LCD_WriteCom(0x01, GPIOx);	/*clear display*/
	delay_ms(2);
	
	if(GPIOx == GPIOB){
		LCD_DisplayString(1,paddle_1, GPIOB);
	}
	if(GPIOx == GPIOC){
		LCD_DisplayString(1,paddle_2, GPIOC);
	}
}

void LCD_Clear(GPIO_TypeDef * GPIOx){
  LCD_WriteCom(0x01, GPIOx);
	delay_ms(2);
}

void LCD_DisplayString(unsigned int line, unsigned char *ptr, GPIO_TypeDef * GPIOx) {
	if(!line)
		LCD_WriteCom(0x80, GPIOx);	/*Set CGRAM to line 1*/
	else
		LCD_WriteCom(0x80+0x40, GPIOx);	/*Set CGRAM to line 2*/
	delay_ms(1);
	
	for(; *ptr != 0; ptr++){
		LCD_WriteData(*ptr, GPIOx);
		//delay_ms(1);
	}
}

void LCD1_paddle_control(int direction)
{
	
	#define UP		1
  #define DOWN	0
	
	//we assume the paddle will initialize to position 8 (middle of LCD)
	//so paddle_x[8] = '_'

	if (p1_pos <= 4 && direction == DOWN);
	else if (p1_pos >= 11 && direction == UP);
	else if (direction == UP)
	{
		paddle_1[p1_pos] = ' ';
		paddle_1[p1_pos+1] = '_';
		p1_pos++;
	}
	else if (direction == DOWN)
	{
		paddle_1[p1_pos] = ' ';
		paddle_1[p1_pos-1] = '_';
		p1_pos--;
	}	
	LCD_DisplayString(1,paddle_1, GPIOB);
}

void LCD2_paddle_control(int direction)
{
	#define UP		1
  #define DOWN	0
	
	//we assume the paddle will initialize to position 8 (middle of LCD)
	//so paddle_x[8] = '_'

	if (p2_pos <=5 && direction == UP);
	else if (p2_pos >= 12 && direction == DOWN);
	else if (direction == UP)
	{
		paddle_2[p2_pos] = ' ';
		paddle_2[p2_pos-1] = '_';
		p2_pos--;
	}
	else if (direction == DOWN)
	{
		paddle_2[p2_pos] = ' ';
		paddle_2[p2_pos+1] = '_';
		p2_pos++;
	}	
	LCD_DisplayString(1,paddle_2, GPIOC);
}

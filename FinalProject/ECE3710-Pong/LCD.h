#include "stm32l476xx.h"
#include <stdlib.h>
#ifndef __STM32L476R_NUCLEO_LCD_H
#define __STM32L476R_NUCLEO_LCD_H

void delay_ms(unsigned int ms);
void LCD_WriteCom(unsigned char com, GPIO_TypeDef * GPIOx);
void LCD_WriteData(unsigned char dat, GPIO_TypeDef * GPIOx);
void LCD_Init(GPIO_TypeDef * GPIOx);
void LCD_Clear(GPIO_TypeDef * GPIOx);
void LCD_DisplayString(unsigned int line, unsigned char *ptr, GPIO_TypeDef * GPIOx);
void Pulse_E (GPIO_TypeDef * GPIOx);
void LCD1_paddle_control(int direction);
void LCD2_paddle_control(int direction);
static unsigned char paddle_reset[17] = {' ', ' ', ' ', ' ', ' ', ' ', ' ', '_', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 0};
static unsigned char paddle_1[17] = {' ', ' ', ' ', ' ', ' ', ' ', ' ', '_', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 0};
static unsigned char paddle_2[17] = {' ', ' ', ' ', ' ', ' ', ' ', ' ', '_', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 0};
extern volatile uint32_t p1_pos, p2_pos, lost;
static uint32_t p1_score=0, p2_score=0;
static unsigned char p1_points [17] = {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 0};
static unsigned char p2_points [17] = {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 0};

#endif

#include "stm32l476xx.h"
#ifndef __STM32L476R_LED_MATRIX_H
#define __STM32L476R_LED_MATRIX_H

	void LED_Matrix_Init(void);
	void Ball_Movement(void);
	void LED_Matrix_Print (volatile uint32_t SCREEN[8]);
	
#endif

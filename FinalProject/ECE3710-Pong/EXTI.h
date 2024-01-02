#include "stm32l476xx.h"
#ifndef __STM32L476R_NUCLEO_EXTI_H
#define __STM32L476R_NUCLEO_EXTI_H

void EXTI_Initialize(void);
void EXTI0_IRQHandler(void);
void EXTI1_IRQHandler(void);
void EXTI4_IRQHandler(void);
void EXTI9_5_IRQHandler(void);

#endif

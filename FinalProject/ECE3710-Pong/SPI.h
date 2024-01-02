#include "stm32l476xx.h"
#ifndef __STM32L476R_SPI_H
#define __STM32L476R_SPI_H

	void SPI_Init(SPI_TypeDef * SPix);
	void SPI_Write(SPI_TypeDef * SPIx, uint16_t *txBuffer, int size);
	void SPI_Write_2(SPI_TypeDef * SPIx, uint16_t *txBuffer, int size);
	void SPI1_IRQHandler(void);
	
		
#endif


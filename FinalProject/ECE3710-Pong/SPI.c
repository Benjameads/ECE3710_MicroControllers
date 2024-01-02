#include "stm32l476xx.h"
#include "SPI.h"
#include "pin_setup.h"
#include "TIM.h"

void SPI_Init(SPI_TypeDef * SPIx){
	uint32_t x;
	
	/*Enable the Internal High Speed oscillato (HSI)*/
	RCC->CR |= RCC_CR_HSION;
	while((RCC->CR & RCC_CR_HSIRDY) == 0);	//Check if clk is ready
	
	// Enable SPI clock
	if(SPIx == SPI1){
		RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN | RCC_AHB2ENR_GPIOAEN;	//turn on clock for B port
	
		x = GPIOA->MODER;
		x &= 0x3FFF33FF;	//clear PA5 and PA7 
		x |= 0x80008800;	//set PA5 and PA7 to alternate function
		GPIOA->MODER = x;
		
		x = GPIOB->MODER;
		x &= 0xFFFFCFFF;	//clear PB6 
		x |= 0x00001000; //set PB6 output
		GPIOB->MODER = x;
		
//		x = GPIOB->OTYPER;
//		x &= 0xFFFFFFBF;	//clear PB6 otyper
//		x |= 0x00000040; //set PB6 open drain
//		GPIOB->MODER = x;
		
		//setting alternate function on PA5 and PA7 to be AF5 (0101) 
		//Sets those pins to be usable for SPI1
		x = GPIOA->AFR[0];
		x &= 0x0F0FFFFF;
		x |= 0x50500000;
		GPIOA->AFR[0] = x;
		
		//setting alternate function on PA15 to be AF5 (0101) 
		//Sets those pins to be usable for SPI1
		x = GPIOA->AFR[1];
		x &= 0x0FFFFFFF;
		x |= 0x50000000;
		GPIOA->AFR[1] = x;
		
		RCC->APB2ENR |= RCC_APB2ENR_SPI1EN; // Enable SPI1 Clock
		RCC->APB2RSTR |= RCC_APB2RSTR_SPI1RST; // Reset SPI1
		RCC->APB2RSTR &= ~RCC_APB2RSTR_SPI1RST; // Clear the reset of SPI1
	} else if(SPIx == SPI2){
		RCC->APB1ENR1 |= RCC_APB1ENR1_SPI2EN; // Enable SPI2 Clock
		RCC->APB1RSTR1 |= RCC_APB1RSTR1_SPI2RST; // Reset SPI2
		RCC->APB1RSTR1 &= ~RCC_APB1RSTR1_SPI2RST; // Clear the reset of SPI2
	} else if(SPIx == SPI3){
		RCC->APB1ENR1 |= RCC_APB1ENR1_SPI3EN; // Enable SPI3 Clock
		RCC->APB1RSTR1 |= RCC_APB1RSTR1_SPI3RST; // Reset SPI3
		RCC->APB1RSTR1 &= ~RCC_APB1RSTR1_SPI3RST; // Clear the reset of SPI3
	}
	
	SPIx->CR1 &= ~SPI_CR1_SPE; // Disable SPI
	
	// Baud rate control:
	// 000 = f_PCLK/2 001 = f_PCLK/4 010 = f_PCLK/8 011 = f_PCLK/16
	// 100 = f_PCLK/32 101 = f_PCLK/64 110 = f_PCLK/128 111 = f_PCLK/256
	// SPI baud rate is set to 5 MHz
	SPIx->CR1 &= ~SPI_CR1_BR; //Set SPI clock to 16MHz/2 = 8 MHz
	//SPIx->CR1 |= SPI_CR1_BR_0;
	
	// Clock phase
	// 0 = The first clock transition is the first data capture edge
	// 1 = The second clock transition is the first data capture edge
	SPIx->CR1 &= ~SPI_CR1_CPHA; //1st edge
	
	// Clock polarity
	// 0 = Set CK to 0 when idle
	// 1 = Set CK to 1 when idle
	SPIx->CR1 &= ~SPI_CR1_CPOL; // Polarity Low
	
	// Configure duplex or receive~only
	// 0 = Full duplex (transmit and receive), 1 = Receive~only
	//SPIx->CR1 &= ~SPI_CR1_RXONLY;
	
	// Bidirectional data mode enable: This bit enables half~duplex
	// communication using common single bidirectional data Line.
	// 0 = 2~Line unidirectional data mode selected
	// 1 = 1~Line bidirectional data mode selected
	SPIx->CR1 |= SPI_CR1_BIDIMODE;
	
	// Output enable in bidirectional mode
	// 0 = Output disabled (receive~only mode)
	// 1 = Output enabled (transmit~only mode)
	SPIx->CR1 |= SPI_CR1_BIDIOE;
	
	// Bit order
	// 0 = MSB transmitted/received first
	// 1 = LSB transmitted/received first
	SPIx->CR1 &= ~SPI_CR1_LSBFIRST; //Most significant bit first
	
	// CRC Polynomial
	SPIx->CRCPR = 7;
	
	// Hardware CRC calculation disabled
	SPIx->CR1 &= ~SPI_CR1_CRCEN;
	
	// NSSGPIO: The value of SSI is forced onto the NSS pin and the IO value
	// of the NSS pin is ignored.
	// 1 = Software slave management enabled
	// 0 = Hardware NSS management enabled
	//SPIx->CR1 |= SPI_CR1_SSM;
	SPIx->CR1 &= ~SPI_CR1_SSM;
	SPIx->CR2 &= ~SPI_CR2_SSOE;
	
	// Manage NSS (slave selection) by using Software
	SPIx->CR1 |= SPI_CR1_SSI;
	
	// Set as Master: 0 = slave, 1 = master
	SPIx->CR1 |= SPI_CR1_MSTR;
	
	// Data Frame Format
	SPIx->CR2 |= SPI_CR2_DS; //1111: 16-bit
	
	// Frame format: 0 = SPI Motorola mode, 1 SPI TI mode
	SPIx->CR2 &= ~SPI_CR2_FRF;
	
	// Enable NSS pulse management
	//SPIx->CR2 |= SPI_CR2_NSSP;
	SPIx->CR2 &= ~SPI_CR2_NSSP;
	
//	//enable tx dma
//	SPIx->CR2 |= SPI_CR2_TXDMAEN;
//	
//	DMA1_CSELR->CSELR &= ~DMA_CSELR_C3S;
//	DMA1_CSELR->CSELR |= 0x00000010;
	
	//SPI1->CR2 |= SPI_CR2_TXEIE;
	
	//NVIC_EnableIRQ(SPI1_IRQn);
	
	// Enable SPI
	SPIx->CR1 |= SPI_CR1_SPE;
}

void SPI_Write(SPI_TypeDef * SPIx, uint16_t *txBuffer, int size){
	int i = 0;
	for (i = 0; i < size; i++) {
		// Wait for TXE (Transmit buffer empty)
		while( (SPIx->SR & SPI_SR_TXE ) != SPI_SR_TXE );
		SPIx->DR = txBuffer[i];
		while( (SPIx->SR & SPI_SR_TXE ) != SPI_SR_TXE );
		//while( (SPIx->SR & SPI_SR_FTLVL) != (SPI_SR_FTLVL_0 | SPI_SR_FTLVL_1) );
		//GPIOA->ODR |= 0x00000040;
	}
	
	// Wait for BSY flag cleared
	while( (SPIx->SR & SPI_SR_BSY) == SPI_SR_BSY );
}

void SPI1_IRQHandler(void){
	//GPIOB->ODR |= 0x00000040;
	//SPI1->SR &= ~SPI_SR_TXE;
}

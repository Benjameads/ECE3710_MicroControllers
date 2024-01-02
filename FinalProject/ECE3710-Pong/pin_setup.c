#include "pin_setup.h"
#include "stm32l476xx.h"

void pinMode(GPIO_TypeDef *port, int pin, int mode) {
    //port->MODER = ...................
	uint32_t x = port->MODER;
	
	//if INPUT
	x &= ~(unsigned int)(0x3 << pin*2);
	
	if (mode == OUTPUT) 
		x |= OUTPUT << pin*2;
	
	port->MODER = x;
}

void setOutputType(GPIO_TypeDef *port, int pin, int type) {
    //port->OTYPER = ...................
	if (type == PUSHPULL){
		port->OTYPER = port->OTYPER & (~(1 << pin));
	}
	else if (type == OPENDRAIN) {
		port->OTYPER = port->OTYPER | (1 << pin);
	}
}

void setPullUpDown(GPIO_TypeDef *port, int pin, int pupd) {
    //port->PUPDR = ...................
	uint32_t x = port->PUPDR;
	
	//if NONE
	x &= (~(unsigned int)(0x3 << pin*2));
	
	if (pupd == PULLUP) 
		x |= (PULLUP << pin*2);
	if (pupd == PULLDOWN) 
		x |= ((unsigned int)PULLDOWN << pin*2);
	
	port->PUPDR = x;
}

void digitalWrite(GPIO_TypeDef *port, int pin, int value) {
    //port->ODR = ...................
	if (value == 0){
		port->ODR = port->ODR & (~(0x1U << pin));
	}
	else if (value == 1) {
		port->ODR = port->ODR | (0x1U << pin);
	}
}

int digitalRead(GPIO_TypeDef *port, int pin) {
    //return ...................	
	return ((port->IDR >> pin) & 0x1 );
}

//int main() {
//    pinMode(GPIOA, 5, OUTPUT);
//    digitalWrite(GPIOA, 5, 1);
//    setOutputType(GPIOA, 5, OPENDRAIN);
//    pinMode(GPIOB, 3, INPUT);
//    setPullUpDown(GPIOB, 3, PULLUP)
//    int value = digitalRead(GPIOB, 3);
//}

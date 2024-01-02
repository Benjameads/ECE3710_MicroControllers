#include "stm32l476xx.h"
#ifndef __STM32L476R_PIN_SETUP_H
#define __STM32L476R_PIN_SETUP_H

#define OUTPUT 1
#define INPUT 0

#define PUSHPULL 0
#define OPENDRAIN 1

#define NONE 0
#define PULLUP 1
#define PULLDOWN 2

void pinMode(GPIO_TypeDef *port, int pin, int mode);
void setOutputType(GPIO_TypeDef *port, int pin, int type);
void setPullUpDown(GPIO_TypeDef *port, int pin, int pupd);
void digitalWrite(GPIO_TypeDef *port, int pin, int value);
int digitalRead(GPIO_TypeDef *port, int pin);

#endif


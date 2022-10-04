//Test program, makes LED blink on and off

#include "stm32f4xx.h"

void main() {
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN; 	//Enable GPIO-D register
	GPIOD->MODER |= GPIO_MODER_MODER12_0;		//Set GPIOD pin 12 to GP output
	GPIOD->ODR = (1<<12); //Set pin12 high, to light LED
};
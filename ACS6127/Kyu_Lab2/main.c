//ACS6127: Digital Inputs and Debouncing - Preparatory Lab
//Written by James A Cusick (Aka Kyu)
//Last edited 07/10/2022CE

#include "stm32f4xx.h"

void main() {
	
	//GPIO config
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	GPIOD->MODER |= (1<<24);	//Set Pin12 to GPout mode
	
	while(1){
		if((GPIOA->IDR & 0x0001)==1){
			GPIOD->BSRR |= 1<<12;
		}else{
			GPIOD->BSRR |= 1<<(12+16);
		}
	}
}
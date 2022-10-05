//Test program, makes LED blink on and off

#include "stm32f4xx.h"

void main() {
	uint32_t ii = 0;
	uint32_t thres = 8000000;
	
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN; 	//Enable GPIO-D register
	GPIOD->MODER |= GPIO_MODER_MODER12_0;		//Set GPIOD pin 12 to GP output
	while(1){
		
		//Pin 12 high for 'thres' cycles
		while(ii<thres){
			GPIOD->ODR |= (1<<12); //Set pin12 high, to light LED
			ii++;
		}
		
		//Pin 12 low for 'thres' cycles
		while(ii>0){
			GPIOD->ODR &= !(1<<12); //Set pin12 low, disable LED
			ii--;
		}
	}		
}
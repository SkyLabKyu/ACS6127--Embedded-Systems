//ACS3127: Digital Input-Output and Timers – preparatory lab
//Written by James A Cusick (a.k.a Kyu)
//Last edited 05/10/2022CE

#include "stm32f4xx.h"

void main(){
	//Initialize port for LED
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
	GPIOD->MODER |= GPIO_MODER_MODER12_0;
	
	//Timer config
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	TIM2->CR1 &= ~0x0016; //Timer 2 control register, set to upcount mode
	TIM2->CR1 |= 1<<3;	// || set to stop counting when timer triggers
	TIM2->PSC = 8399;			// Set prescaler to 8299 (clock speed of 10khz)
	TIM2->ARR = 9999;			//Counter reset value, 1 second between pulses
	TIM2->EGR = 1;				//Enable timer
	
	while(1){
		GPIOD->BSRR |= (1<<12);
		TIM2->CR1 |= 1; //Enables counter
		
		while((TIM2->SR&0x0001)!=1){};
		TIM2->SR &= ~1;
			
		GPIOD->BSRR |= 1<<(12+16);
		TIM2->CR1 |= 1; //Enables counter
			
		while((TIM2->SR&0x0001)!=1){};
		TIM2->SR &= ~1;
	}
}
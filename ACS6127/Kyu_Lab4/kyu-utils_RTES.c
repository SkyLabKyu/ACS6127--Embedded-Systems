//LED initialize function

#include "stm32f4xx.h"

//Initializes GPIOD to configure LEDs as Outputs
void LED_init(){
	GPIO_InitTypeDef IO_Params;
	
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
	
	IO_Params.Pin = (1<<12) | (1<<13) | (1<<14) | (1<<15);
	IO_Params.Mode = GPIO_MODE_OUTPUT_PP;
	IO_Params.Speed = GPIO_SPEED_FAST;
	
	HAL_GPIO_Init(GPIOD, &IO_Params);
}

//Flashes LED
void LED_blink(uint8_t LED, uint8_t State){
	uint32_t i = 0;
	if(State>0){
		GPIOD->ODR |= 1<<LED;
	}else{
		GPIOD->ODR &= ~(1<<LED);	
	}
}
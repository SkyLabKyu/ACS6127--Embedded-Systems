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
	if(State>0){
		GPIOD->ODR |= 1<<LED;
	}else{
		GPIOD->ODR &= ~(1<<LED);	
	}
}

void PB_init(){
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	GPIOA->MODER |= 0x00000000;
}

uint8_t PB_check(){	
	if(GPIOA->IDR & 0x0001){
		return(1);
	}else{
		return(0);
	}
}

void SPI_Check(SPI_HandleTypeDef SPI_Params){
	uint8_t Init_Buff[1];
	uint16_t Frame = 1;
	uint32_t Timout = 1000;
	
	GPIOE->BSRR |= GPIO_PIN_3<<16;
	
	Init_Buff[0] = 0x0F | 0x80;
	HAL_SPI_Transmit(&SPI_Params, Init_Buff, Frame, Timout);
	
	Init_Buff[0] = 0x00;
	HAL_SPI_Receive(&SPI_Params, Init_Buff, Frame, Timout);
	
	GPIOE->BSRR |= GPIO_PIN_3;
	
	if(Init_Buff[0] == 0x3F){
		LED_blink(12, 1);
	}else{
		LED_blink(14, 1);
	}
}
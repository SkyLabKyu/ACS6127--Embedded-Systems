// Lab 3 - Dickin' around with SPI
//Code by James A Cusick (a.k.a "Kyu")
//Last edited 03/11/2022CE

#include "stm32f4xx.h"

int main() {
	SPI_HandleTypeDef SPI_Params;
	GPIO_InitTypeDef GPIO_Params;
	uint8_t SPI_Buffer[1];

	
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN; //Enable SPI clock enable
	
	//Configure SPI Parameters	
	SPI_Params.Instance = SPI1;
	SPI_Params.Init.Mode = SPI_MODE_MASTER;
	SPI_Params.Init.NSS = SPI_NSS_SOFT;
	SPI_Params.Init.CLKPolarity = SPI_POLARITY_HIGH;
	SPI_Params.Init.CLKPhase = SPI_PHASE_2EDGE;
	SPI_Params.Init.Direction = SPI_DIRECTION_2LINES;
	SPI_Params.Init.DataSize = SPI_DATASIZE_8BIT;
	SPI_Params.Init.FirstBit = SPI_FIRSTBIT_MSB;
	SPI_Params.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;
	
	//Initialize SPI control
	HAL_SPI_Init(&SPI_Params);
	
	//Configure GPIOA Parameters
	GPIO_Params.Pin = GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
	GPIO_Params.Mode = GPIO_MODE_AF_PP;
	GPIO_Params.Pull = GPIO_NOPULL;
	GPIO_Params.Speed = GPIO_SPEED_FAST;
	GPIO_Params.Alternate = GPIO_AF5_SPI1;
	
	//Initialize GPIOA
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	HAL_GPIO_Init(GPIOA, &GPIO_Params);
	
	//Configure GPIOE Parameters
	GPIO_Params.Pin = GPIO_PIN_3;
	GPIO_Params.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_Params.Pull = GPIO_PULLUP;
	GPIO_Params.Speed = GPIO_SPEED_FAST;
	
	//Initialize GPIOE
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;
	HAL_GPIO_Init(GPIOE, &GPIO_Params);
	
	GPIOE->BSRR |= GPIO_PIN_3;
	__HAL_SPI_ENABLE(&SPI_Params);
	
	//Initialize LED pins
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN; // Enable Port D clock
	GPIOD->MODER |= GPIO_MODER_MODER14_0; // Port D.14 output - red LED
	GPIOD->MODER |= GPIO_MODER_MODER12_0; // Port D.12 output - green LED
	
	while(1){
		SPI_Buffer[0] = 0x80 | 0x0F;
		GPIOE->BSRR |= GPIO_PIN_3<<16;
		HAL_SPI_Transmit(&SPI_Params, SPI_Buffer, 1, 1000);
		SPI_Buffer[0] = 0x00;
		HAL_SPI_Receive(&SPI_Params, SPI_Buffer, 1, 1000);
		GPIOE->BSRR |= GPIO_PIN_3;
		
		if((SPI_Buffer[0] & 0x3f) == 1){
			GPIOD->BSRR |= 1<<12;
		}else{
			GPIOD->BSRR |= 1<<14;
		}
	}
}

// Lab 3 - Dickin' around with SPI
//Code by James A Cusick (a.k.a "Kyu")
//Last edited 03/11/2022CE

#include "stm32f4xx.h"

int main() {
	SPI_HandleTypeDef SPI_Params;
	GPIO_InitTypeDef GPIOA_Params;
	GPIO_InitTypeDef GPIOE_Params;
	uint8_t SPI_Buffer[1];
	uint16_t setup[1];
	uint16_t Frame = 1;
	uint32_t Timeout = 1000;
	uint8_t z;

	
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
	GPIOA_Params.Pin = GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
	GPIOA_Params.Mode = GPIO_MODE_AF_PP;
	GPIOA_Params.Pull = GPIO_NOPULL;
	GPIOA_Params.Speed = GPIO_SPEED_FAST;
	GPIOA_Params.Alternate = GPIO_AF5_SPI1;
	
	//Initialize GPIOA
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	HAL_GPIO_Init(GPIOA, &GPIOA_Params);
	
	//Configure GPIOE Parameters
	GPIOE_Params.Pin = GPIO_PIN_3;
	GPIOE_Params.Mode = GPIO_MODE_OUTPUT_PP;
	GPIOE_Params.Pull = GPIO_PULLUP;
	GPIOE_Params.Speed = GPIO_SPEED_FAST;
	
	//Initialize GPIOE
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;
	HAL_GPIO_Init(GPIOE, &GPIOE_Params);
	
	//Configure GPIOE pin 0 as interrupt pin
	GPIOE->MODER &= 0xFFFFFFFC; //Set pin 0 Moder to input
	
	GPIOE->BSRR |= GPIO_PIN_3;
	__HAL_SPI_ENABLE(&SPI_Params);
	
	//Initialize LED pins
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN; // Enable Port D clock
	GPIOD->MODER |= GPIO_MODER_MODER14_0; // Port D.14 output - red LED
	GPIOD->MODER |= GPIO_MODER_MODER12_0; // Port D.12 output - green LED
	
	//Configure Control Reg 4 on LIS3DSH to output Z axis accelleration
	SPI_Buffer[0] = (0x20);
	GPIOE->BSRR |= GPIO_PIN_3<<16;
	HAL_SPI_Transmit(&SPI_Params, SPI_Buffer, Frame, Timeout);
	SPI_Buffer[0] = (0x14);
	HAL_SPI_Transmit(&SPI_Params, SPI_Buffer, Frame, Timeout);
	GPIOE->BSRR |= GPIO_PIN_3;
	
	//Configure Control Reg 4 on LIS3DSH to output Z axis accelleration
	SPI_Buffer[0] = (0x23);
	GPIOE->BSRR |= GPIO_PIN_3<<16;
	HAL_SPI_Transmit(&SPI_Params, SPI_Buffer, Frame, Timeout);
	SPI_Buffer[0] = (0xC8);
	HAL_SPI_Transmit(&SPI_Params, SPI_Buffer, Frame, Timeout);
	GPIOE->BSRR |= GPIO_PIN_3;
	
	
	while(1){
		if(GPIOE->IDR&0x0001){
			GPIOE->IDR = ~1;
			
			SPI_Buffer[0] = 0x80 | 0x2D;
			GPIOE->BSRR |= GPIO_PIN_3<<16;
			HAL_SPI_Transmit(&SPI_Params, SPI_Buffer, Frame, Timeout);
			SPI_Buffer[0] = 0x00;
			HAL_SPI_Receive(&SPI_Params, SPI_Buffer, Frame, Timeout);
			z = SPI_Buffer[0];
			GPIOE->BSRR |= GPIO_PIN_3;
			
			if((z&0x80) == 0x80){
				GPIOD->BSRR |= 1<<(12);
				GPIOD->BSRR |= 1<<(14+16);
			}else{
				GPIOD->BSRR |= 1<<(12+16);
				GPIOD->BSRR |= 1<<(14);
			}
		}else{
		}
	}
}
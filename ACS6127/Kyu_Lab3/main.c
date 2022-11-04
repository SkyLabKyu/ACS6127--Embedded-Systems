// Lab 3 - Dickin' around with SPI
//Code by James A Cusick (a.k.a "Kyu")
//Last edited 03/11/2022CE

#include "stm32f4xx.h"

int main() {
	SPI_HandleTypeDef SPI_Params;
	GPIO_InitTypeDef GPIO_Params;
	RCC->APB2ENR |= 1<<12; //Enable SPI clock enable
	
	//Configure SPI Parameters	
	SPI_Params.Instance = SPI1;
	SPI_Params.Init.Mode = SPI_MODE_MASTER;
	SPI_Params.Init.NSS = SPI_NSS_SOFT;
	SPI_Params.Init.CLKPolarity = SPI_POLARITY_HIGH;
	SPI_Params.Init.CLKPhase = SPI_PHASE_2EDGE;
	SPI_Params.Init.Direction = SPI_DIRECTION_2LINES;
	SPI_Params.Init.DataSize = SPI_DATASIZE_8BIT;
	SPI_Params.Init.FirstBit = SPI_FIRSTBIT_MSB;
	SPI_Params.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
	
	//Commence initialization procedure
	HAL_SPI_Init(&SPI_Params);
	
	//Configure GPIO Parameters
	GPIO_Params.Pin = (1<<5) | (1<<6) | (1<<7);
	GPIO_Params.Mode = GPIO_MODE_AF_PP;
	GPIO_Params.Pull = GPIO_NOPULL;
	GPIO_Params.Speed = GPIO_SPEED_FAST;
	GPIO_Params.Alternate = GPIO_AF5_SPI1;
	
	//Configure GPIO
	HAL_GPIO_Init(GPIOA, &GPIO_Params);
}
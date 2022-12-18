/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/

#define osObjectsPublic                     // define objects in main module
#include "osObjects.h"                      // RTOS object definitions
#include "kyu-utils.h"											// Custom utilities
#include "Thread.h"
#include "stm32f4xx.h"


/*
 * main: initialize and start the system
 */
int main (void) {
	SPI_HandleTypeDef SPI_Params;
	GPIO_InitTypeDef GPIOA_Params;
	GPIO_InitTypeDef GPIOE_Params;
	uint8_t Init_Buff[1];
	uint16_t Frame = 1;
	uint32_t Timout = 1000;
	
	//===== LED initialization =====//
	LED_init();
		
	
	//===== SPI initialization =====//
	
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN; //Enable SPI clock
	
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
	HAL_GPIO_Init(GPIOE, &GPIOE_Params);				//Initialize Port E register for this pin
	GPIOE->MODER &= 0xFFFFFFFC;								 	//Set pin 0 and 1 Moder to input
	GPIOE->ODR |= GPIO_PIN_3;										//Set pin 3 high (idle state)
	__HAL_SPI_ENABLE(&SPI_Params);

	//===== LS3DSH Initialize =====//
	
	GPIOE->BSRR |= GPIO_PIN_3<<16;
	
//	//Configures write mode and sends the address 0x23(Control Reg 3)
//	Init_Buff[0] = 0x20;
//	HAL_SPI_Transmit(&SPI_Params, Init_Buff, Frame, Timout); 
//	
//	//Xen and Yen set to 1, enabling the LIS3DSH interrupt bit so it knows when to send data
//	Init_Buff[0] = 0x80;
//	HAL_SPI_Transmit(&SPI_Params, Init_Buff, Frame, Timout);
	
	//Configures write mode and sends the address 0x20(Control Reg 4)
	Init_Buff[0] = 0x20;
	HAL_SPI_Transmit(&SPI_Params, Init_Buff, Frame, Timout); 
	
	//Xen and Yen set to 1, enabling output of x and y axis components.
	Init_Buff[0] = 0x13;
	HAL_SPI_Transmit(&SPI_Params, Init_Buff, Frame, Timout);
	
	GPIOE->BSRR |= GPIO_PIN_3;	
	
	osKernelInitialize ();                    // initialize CMSIS-RTOS
		
	//===== Setup phase complete =====//
	
	//Begin declaring threads
	Init_Upd_X();
	Init_Upd_Y();
	
	//Enable threads
  osKernelStart ();
}

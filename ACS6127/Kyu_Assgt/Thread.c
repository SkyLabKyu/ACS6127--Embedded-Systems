#include "cmsis_os.h"                                           // CMSIS RTOS header file
#include "kyu-utils.h"
#include "stm32f4xx.h"


/*----------------------------------------------------------------------------
 *      Thread 1 - Update X | This thread reads the X axis of the accelerometer and updates the output LEDs as appropriate
 *---------------------------------------------------------------------------*/
 
void Upd_X(void const *argument);                              // thread function
osThreadId tid_Upd_X;                                          // thread id
osThreadDef (Upd_X, osPriorityNormal, 1, 0);                   // thread object

int Init_Upd_X(void) {
  tid_Upd_X = osThreadCreate (osThread(Upd_X), NULL);			//Declares thread
  if (!tid_Upd_X) return(-1);																//Returns error code if the thread fails to initialize
  return(0);
}

void Upd_X(void const *argument) {
	SPI_HandleTypeDef SPI_Params;
	uint8_t SPI_Buffer[1];	//Declares variable used to store data recieved or sent via SPI bus
	uint16_t Acc_x;					//Acceleration X component
	uint16_t Frame = 1;
	uint32_t Timout = 1000;
	
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
		
  while (1) {
		Acc_x = 0;
		
		//SPI Retrieves Out X low, getting the 8 LSB bits that correspond to the x axis
		GPIOE->BSRR |= GPIO_PIN_3<<16;
		
		SPI_Buffer[0] = 0x28 | 0x80;
		HAL_SPI_Transmit(&SPI_Params, SPI_Buffer, Frame, Timout);
		
		SPI_Buffer[0] = 0x00;
		HAL_SPI_Receive(&SPI_Params, SPI_Buffer, Frame, Timout);
		Acc_x = SPI_Buffer[0];
		
		GPIOE->BSRR |= GPIO_PIN_3;
		
		//SPI Retrieves Out X High, getting the 8 MSB bits that correspond to the x axis
		GPIOE->BSRR |= GPIO_PIN_3<<16;
				
		SPI_Buffer[0] = 0x29 | 0x80;
		HAL_SPI_Transmit(&SPI_Params, SPI_Buffer, Frame, Timout);
		
		SPI_Buffer[0] = 0x00;
		HAL_SPI_Receive(&SPI_Params, SPI_Buffer, Frame, Timout);
		Acc_x += SPI_Buffer[0]<<8;

		GPIOE->BSRR |= GPIO_PIN_3;
		
		if((Acc_x & 0x8000) ==0x8000){
			LED_blink(12, 1);
			LED_blink(14, 0);
		}else{
			LED_blink(12, 0);
			LED_blink(14, 1);
		}
  }
}

/*----------------------------------------------------------------------------
 *      Thread 2 - Update Y | This thread reads the Y axis of the accelerometer and updates the output LEDs as appropriate
 *---------------------------------------------------------------------------*/
 
void Upd_Y(void const *argument);                              // thread function
osThreadId tid_Upd_Y;                                          // thread id
osThreadDef (Upd_Y, osPriorityNormal, 1, 0);                   // thread object

int Init_Upd_Y(void) {
  tid_Upd_Y = osThreadCreate (osThread(Upd_Y), NULL);			//Declares thread
  if (!tid_Upd_Y) return(-1);																//Returns error code if the thread fails to initialize
  return(0);
}

void Upd_Y(void const *argument) {
	SPI_HandleTypeDef SPI_Params;
	uint8_t SPI_Buffer[1];	//Declares variable used to store data recieved or sent via SPI bus
	uint16_t Acc_y;					//Acceleration Y component
	uint16_t Frame = 1;
	uint32_t Timout = 1000;
	
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
		
  while (1) {
		Acc_y = 0;
		
		//SPI Retrieves Out Y low, getting the 8 LSB bits that correspond to the y ayis
		GPIOE->BSRR |= GPIO_PIN_3<<16;
		
		SPI_Buffer[0] = 0x2a | 0x80;
		HAL_SPI_Transmit(&SPI_Params, SPI_Buffer, Frame, Timout);
		
		SPI_Buffer[0] = 0x00;
		HAL_SPI_Receive(&SPI_Params, SPI_Buffer, Frame, Timout);
		Acc_y = SPI_Buffer[0];
		
		GPIOE->BSRR |= GPIO_PIN_3;
		
		//SPI Retrieves Out Y High, getting the 8 MSB bits that correspond to the y ayis
		GPIOE->BSRR |= GPIO_PIN_3<<16;
				
		SPI_Buffer[0] = 0x2b | 0x80;
		HAL_SPI_Transmit(&SPI_Params, SPI_Buffer, Frame, Timout);
		
		SPI_Buffer[0] = 0x00;
		HAL_SPI_Receive(&SPI_Params, SPI_Buffer, Frame, Timout);
		Acc_y += SPI_Buffer[0]<<8;

		GPIOE->BSRR |= GPIO_PIN_3;
		
		if((Acc_y & 0x8000) ==0x8000){
			LED_blink(15, 1);
			LED_blink(13, 0);
		}else{
			LED_blink(15, 0);
			LED_blink(13, 1);
		}
  }
}
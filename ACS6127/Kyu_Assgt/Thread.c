
#include "cmsis_os.h"                                           // CMSIS RTOS header file
#include "kyu-utils.h"
#include "stm32f4xx.h"

///*----------------------------------------------------------------------------
// *      Thread 1 'Thread_Name': Sample thread
// *---------------------------------------------------------------------------*/
// 
//void Thread (void const *argument);                             // thread function
//osThreadId tid_Thread;                                          // thread id
//osThreadDef (Thread, osPriorityNormal, 1, 0);                   // thread object

//int Init_Thread (void) {

//  tid_Thread = osThreadCreate (osThread(Thread), NULL);
//  if (!tid_Thread) return(-1);
//  
//  return(0);
//}

//void Thread (void const *argument) {

//  while (1) {
//    ; // Insert thread code here...
//    osThreadYield ();                                           // suspend thread
//  }
//}


/*----------------------------------------------------------------------------
 *      Thread 2 LED Set : Sets the 4 LEDs to the appropriate state, based on accelerometer state
 *---------------------------------------------------------------------------*/
 
void LED_Set(void const *argument);                              // thread function
osThreadId tid_LED_Set;                                          // thread id
osThreadDef (LED_Set, osPriorityNormal, 1, 0);                   // thread object

int Init_LED_Set(void) {
  tid_LED_Set = osThreadCreate (osThread(LED_Set), NULL);			//Declares thread
  if (!tid_LED_Set) return(-1);																//Returns error code if the thread fails to initialize
  return(0);
}

void LED_Set(void const *argument) {
	SPI_HandleTypeDef SPI_Params;
	uint8_t SPI_Buffer[1];	//Declares variable used to store data recieved or sent via SPI bus
	uint16_t Acc_x;			//Acceleration X component
	uint16_t Acc_y;				//Acceleration Y component
	uint16_t Frame = 1;
	uint32_t Timout = 1000;
	
	//Configure SPI Parameters	
//	SPI_Params.Instance = SPI1;
//	SPI_Params.Init.Mode = SPI_MODE_MASTER;
//	SPI_Params.Init.NSS = SPI_NSS_SOFT;
//	SPI_Params.Init.CLKPolarity = SPI_POLARITY_HIGH;
//	SPI_Params.Init.CLKPhase = SPI_PHASE_2EDGE;
//	SPI_Params.Init.Direction = SPI_DIRECTION_2LINES;
//	SPI_Params.Init.DataSize = SPI_DATASIZE_8BIT;
//	SPI_Params.Init.FirstBit = SPI_FIRSTBIT_MSB;
//	SPI_Params.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;
	
	osSignalWait(0x01, osWaitForever);
	
  while (1) {
		GPIOE->BSRR |= GPIO_PIN_3<<16;
		
//		SPI_Buffer[0] = 0x28 | 0x80;
//		HAL_SPI_Transmit(&SPI_Params, SPI_Buffer, Frame, Timout);
//		
//		SPI_Buffer[0] = 0x00;
//		HAL_SPI_Receive(&SPI_Params, SPI_Buffer, Frame, Timout);
//		Acc_x |= *SPI_Buffer;
		
//		SPI_Buffer[0] = 0x29 | 0x80;
//		HAL_SPI_Transmit(&SPI_Params, SPI_Buffer, Frame, Timout);
//		
//		SPI_Buffer[0] = 0x00;
//		HAL_SPI_Receive(&SPI_Params, SPI_Buffer, Frame, Timout);
//		Acc_x |= *SPI_Buffer<<8;

		GPIOE->BSRR |= GPIO_PIN_3;
		
		//Once that's done, the registers you need to look at are as follows: 28, 29 (X axis out low and high regs), 2a, 2b (Y axis out low and high regs)
		//Once you got that working you can look into using interrupts to speed the whole thing up
		
		if((Acc_x & 0x8000) == 0x8000){
			LED_blink(15, 1);
			//LED_blink(14, 0);
		}else{
			LED_blink(15, 1);
			//LED_blink(15, 0);
		}
  }
}

///*----------------------------------------------------------------------------
// *      Thread 1 SPI_Init: Fetches accelerometer reading
// *---------------------------------------------------------------------------*/
 
void SPI_Init (void const *argument);                             // thread function
osThreadId tid_SPI_Init;                                          // thread id
osThreadDef (SPI_Init, osPriorityNormal, 1, 0);                   // thread object

int Init_SPI_Init(void) {
  tid_SPI_Init = osThreadCreate (osThread(SPI_Init), NULL);		//Declares thread
  if (!tid_SPI_Init) return(-1);																//Returns error code if the thread fails to initialize
  
  return(0);
}

void SPI_Init(void const *argument) {
	SPI_HandleTypeDef SPI_Params;
	GPIO_InitTypeDef GPIOA_Params;
	uint8_t Init_Buff[1];
	uint16_t Frame = 1;
	uint32_t Timout = 1000;


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
	GPIOA_Params.Pin = GPIO_PIN_3;
	GPIOA_Params.Mode = GPIO_MODE_OUTPUT_PP;
	GPIOA_Params.Pull = GPIO_PULLUP;
	GPIOA_Params.Speed = GPIO_SPEED_FAST;
	
	//Initialize GPIOE
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;
	HAL_GPIO_Init(GPIOE, &GPIOA_Params);				//Initialize Port E register for this pin
	GPIOE->MODER &= 0xFFFFFFFC;								 	//Set pin 0 and 1 Moder to input
	GPIOE->ODR |= GPIO_PIN_3;										//Set pin 3 high (idle state)
	__HAL_SPI_ENABLE(&SPI_Params);

	SPI_Check(SPI_Params);

	//===== LS3DSH Initialize =====//
	
	GPIOE->BSRR |= GPIO_PIN_3<<16;
	
	//Configures write mode and sends the address 0x20(Control Reg 4)
	Init_Buff[0] = 0x20;
	HAL_SPI_Transmit(&SPI_Params, Init_Buff, Frame, Timout); 
	
	//Xen and Yen set to 1, enabling output of x and y axis components.
	Init_Buff[0] = 0x03;
	HAL_SPI_Transmit(&SPI_Params, Init_Buff, Frame, Timout);
	
	GPIOE->BSRR |= GPIO_PIN_3;	
	
	osSignalSet(tid_LED_Set, 0x01);
  osThreadYield ();                                           // suspend thread
  
}

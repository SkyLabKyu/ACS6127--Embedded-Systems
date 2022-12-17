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
	
  //===== LED initialization =====//
	LED_init();

	osKernelInitialize ();                    // initialize CMSIS-RTOS
		
		
	//===== Setup phase complete =====//
	
	//Begin declaring threads
	Init_LED_Set();
	Init_SPI_Init();
	
	//Enable threads
  osKernelStart ();
}

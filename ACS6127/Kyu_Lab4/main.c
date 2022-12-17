/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/

#define osObjectsPublic                     // define objects in main module
#include "osObjects.h"                      // RTOS object definitions
#include "kyu-utils_RTES.h"									// The most useful module in the world
#include "Thread.h"


/*
 * main: initialize and start the system
 */
int main (void) {
  osKernelInitialize ();                    // initialize CMSIS-RTOS

  // initialize peripherals here
	LED_init();
	PB_init();
	

  // create 'thread' functions that start executing,
  // example: tid_name = osThreadCreate (osThread(name), NULL);
	Init_Blink_Green_Thread();
	Init_Blink_Blue_Thread();
	

  osKernelStart ();                         // start thread execution 
}

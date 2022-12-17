
#include "cmsis_os.h"                                           // CMSIS RTOS header file	
#include "stm32f407xx.h"																				// Header for device drivers
#include "kyu-utils_RTES.h"																			// Header for RTES branch of Kyu utils

/*----------------------------------------------------------------------------
 *      Thread 1 'Blink Green': It blinkey the Green, idkwywmtty
 *---------------------------------------------------------------------------*/
 
void Blink_Green_Thread(void const *argument);               // thread function
osThreadId tid_Blink_Green_Thread;                                // thread id
osThreadDef (Blink_Green_Thread, osPriorityNormal, 1, 0);         // thread object

void Blink_Blue_Thread(void const *argument);            			   // thread function
osThreadId tid_Blink_Blue_Thread;                                // thread id
osThreadDef (Blink_Blue_Thread, osPriorityNormal, 1, 0);         // thread object

int Init_Blink_Green_Thread (void) {

  tid_Blink_Green_Thread = osThreadCreate (osThread(Blink_Green_Thread), NULL);
  if (!tid_Blink_Green_Thread) return(-1);
  
  return(0);
}

void Blink_Green_Thread (void const *argument) {
	
  while (1) {
    LED_blink(12, 1);
		osDelay(1009);
		LED_blink(12, 0);
		osDelay(1009);
    osThreadYield ();                                           // suspend thread
  }
}


/*----------------------------------------------------------------------------
 *      Thread 2 'Blink Blue': It blinkey the Blue, idkwywmtty
 *---------------------------------------------------------------------------*/

int Init_Blink_Blue_Thread (void) {

  tid_Blink_Blue_Thread = osThreadCreate (osThread(Blink_Blue_Thread), NULL);
  if (!tid_Blink_Blue_Thread) return(-1);
  
  return(0);
}

void Blink_Blue_Thread (void const *argument) {
	uint8_t state = 0;
	
  while (1) {
		state = PB_check();
		LED_blink(15, state);
		
    osThreadYield ();                                           // suspend thread
  }
}

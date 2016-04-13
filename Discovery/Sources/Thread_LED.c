/**
	******************************************************************************
  * @file    Thread_angles.c
  * @author  Mathieu Wang & Russell Buchanan
	* @version V1.0.0
  * @date    18-March-2016
  * @brief   This file defines and runs the thread to get roll and pitch angles from accelerometers
  ******************************************************************************
  */
	
#include "cmsis_os.h"                   // ARM::CMSIS:RTOS:Keil RTX
#include "stm32f4xx_hal.h"
#include "main.h"
#include "pwm.h"
#include "led.h"

void Thread_LED(void const *argument);                  /** thread function */
osThreadId tid_Thread_LED;                               /** thread id */
osThreadDef(Thread_LED, osPriorityBelowNormal, 1, 0);  

/**----------------------------------------------------------------------------
 *      Create the thread within RTOS context
 *---------------------------------------------------------------------------*/
int start_Thread_LED (void) {

  tid_Thread_LED = osThreadCreate(osThread(Thread_LED), NULL);
  if (tid_Thread_LED == NULL) return(-1); 
  return(0);
}

/**----------------------------------------------------------------------------
 *      Thread  'Thread_angles': set Pitch and Roll display
 *---------------------------------------------------------------------------*/
	void Thread_LED(void const *argument) 
	{
		
		while(1){
			/*************************************
				         Temporary init
			**************************************/
			//osSignalWait(1, osWaitForever);
			osDelay(50);
			//osSignalClear(tid_Thread_LED, 1);
		}
	}

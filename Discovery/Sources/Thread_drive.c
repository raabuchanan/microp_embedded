/**
	******************************************************************************
  * @file    Thread_drive.c
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
#include "util.h"

void Thread_drive(void const *argument);                  /** thread function */
osThreadId tid_Thread_drive;                               /** thread id */
osThreadDef(Thread_drive, osPriorityAboveNormal, 1, 0);  
uint8_t motorSpeed;

extern float pitch, roll;

/**----------------------------------------------------------------------------
 *      Create the thread within RTOS context
 *---------------------------------------------------------------------------*/
int start_Thread_drive (void) {

  tid_Thread_drive = osThreadCreate(osThread(Thread_drive), NULL);
  if (!tid_Thread_drive) return(-1); 
  return(0);
}

/**----------------------------------------------------------------------------
 *      Thread  'Thread_angles': set Pitch and Roll display
 *---------------------------------------------------------------------------*/
	void Thread_drive(void const *argument) 
	{
		drive_PWM_Init();
		
		while(1){
			motorSpeed = (int)roll;
			set_drive_left(motorSpeed);
			set_drive_right(motorSpeed);
			osDelay(50);
		}
	}

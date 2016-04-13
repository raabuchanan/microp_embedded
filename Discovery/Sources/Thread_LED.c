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
void setAllLEDs(void);
void unsetAllLEDs(void);
osThreadId tid_Thread_LED;                               /** thread id */
osThreadDef(Thread_LED, osPriorityBelowNormal, 1, 0);  
extern int doubleTap;
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
			osSignalWait(1, osWaitForever);
			if(doubleTap == 0)
			{
				LED_PWM_DeInit();
				unsetAllLEDs();
			}
			else if (doubleTap == 1)
			{	
				LED_PWM_DeInit();
				LED_GPIO_Init();
				setAllLEDs();
			}
			else if (doubleTap == 2)
			{
				unsetAllLEDs();
				LED_PWM_Init();
			} 
			//todo: Adjust speed of rotation
			else if (doubleTap == 3)
			{
				unsetAllLEDs();
				LED_PWM_Init();
			} 
			osSignalClear(tid_Thread_LED, 1);
		}
	}

void setAllLEDs()
{
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_SET);
}

void unsetAllLEDs()
{
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_RESET);
}

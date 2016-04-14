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
void rotateRight(int speed);
void rotateLeft(int speed);
osThreadId tid_Thread_LED;                               /** thread id */
osThreadDef(Thread_LED, osPriorityBelowNormal, 1, 0);  
uint16_t currentPin =  0;
int scaledSpeed = 0;
extern int doubleTap;
extern float pitch, roll;
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
		currentPin = GPIO_PIN_12;
		while(1){
			osSignalWait(1, osWaitForever);
			if (pitch > 90)
			{	
				LED_PWM_DeInit();
				LED_GPIO_Init();
				setAllLEDs();
			}
			else if (roll > 90)
			{
				unsetAllLEDs();
				//LED_GPIO_DeInit();
				LED_PWM_Init();
			} 
			else if (pitch < 90)
			{
				LED_PWM_DeInit();
				LED_GPIO_Init();
				rotateLeft(10);
			} 
			else if (roll < 90)
			{
				LED_PWM_DeInit();
				LED_GPIO_Init();
				rotateRight(1);
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

void rotateLeft(int speed)
{
	scaledSpeed = ((11-speed)*MAXSPEED)/10;
	osDelay(scaledSpeed);
	switch (currentPin)
	{
		case GPIO_PIN_12:
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET);
			currentPin = GPIO_PIN_13;
			break;
		case GPIO_PIN_13:
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET);
			currentPin = GPIO_PIN_14;
			break;
		case GPIO_PIN_14:
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_SET);
			currentPin = GPIO_PIN_15;
			break;
		case GPIO_PIN_15:
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET);
			currentPin = GPIO_PIN_12;
			break;
	}
}

void rotateRight(int speed)
{
	scaledSpeed = ((10-speed)*MAXSPEED)/10;
	osDelay(scaledSpeed);
	switch (currentPin)
	{
		case GPIO_PIN_12:
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_SET);
			currentPin = GPIO_PIN_15;
			break;
		case GPIO_PIN_13:
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET);
			currentPin = GPIO_PIN_12;
			break;
		case GPIO_PIN_14:
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET);
			currentPin = GPIO_PIN_13;
			break;
		case GPIO_PIN_15:
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET);
			currentPin = GPIO_PIN_14;
			break;
	}
}

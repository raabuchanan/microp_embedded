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
void rotateRight(int delay);
void rotateLeft(int delay);
void setIntensity(int intensity);
void rotate(void);
osThreadId tid_Thread_LED;                               /** thread id */
osThreadDef(Thread_LED, osPriorityBelowNormal, 1, 0);  
uint16_t currentPin =  0;
int scaledSpeed = 0;
extern int doubleTap, speed, intensity;
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
			if (speed == 0)
			{	
				LED_PWM_Init();
				setIntensity(intensity);
			}
			else
			{
				LED_PWM_DeInit();
				LED_GPIO_Init();
				rotate();
				//LED_GPIO_DeInit();
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

void rotate()
{
	scaledSpeed = speed%10;
	scaledSpeed = ((11-scaledSpeed)*MAXSPEED)/10;
	
	if(speed > 10)
		rotateLeft(scaledSpeed);
	else if(speed < 10)
		rotateRight(scaledSpeed);
}

void rotateLeft(int delay)
{
	osDelay(delay);
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

void rotateRight(int delay)
{
	osDelay(delay);
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

void setIntensity(int intensity)
{
	set_green_pwm(intensity);
	set_orange_pwm(intensity);
	set_red_pwm(intensity);
	set_blue_pwm(intensity);
}

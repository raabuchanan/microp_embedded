/**
	******************************************************************************
  * @file    Thread_sevenseg.c
  * @author  Mathieu Wang & Russell Buchanan
	* @version V1.0.0
  * @date    18-March-2016
  * @brief   This file provides the thread definition and function for the 7-segment display
  ******************************************************************************
  */
	
#include "cmsis_os.h"                   // ARM::CMSIS:RTOS:Keil RTX
#include "stm32f4xx_hal.h"
#include "main.h"

#define NORMAL_DELAY 2 /** Normal delay of the 7-segment display: 5ms */
#define ALARM_DELAY 250 /** Alarm mode delay of the 7-segment display: 250 ms */

void Thread_sevenseg (void const *argument);                 /** thread function */
osThreadId tid_Thread_sevenseg;                              /** thread id */
osThreadDef(Thread_sevenseg, osPriorityNormal, 1, 312);			 /** thread definition with above normal priority and a stack size of 312 = 1.5 * 208 (max observed stack usage) */
extern DisplayMode currentMode;
//extern float filtered_temperature;
//extern float pitch, roll;
float TEMPERATURE_THRESHOLD_FOR_ALARM = 35.0f;							 /** alarm threshold */

/**----------------------------------------------------------------------------
 *      Create the thread within RTOS context
 *---------------------------------------------------------------------------*/
int start_Thread_sevenseg (void) {

  tid_Thread_sevenseg = osThreadCreate(osThread(Thread_sevenseg), NULL); // Start Thread_sevenseg
  if (!tid_Thread_sevenseg) return(-1); 
  return(0);
}

/**----------------------------------------------------------------------------
 *      Thread  'Thread_sevenseg': set 7-segment display
 *---------------------------------------------------------------------------*/
	void Thread_sevenseg (void const *argument) {
		while(1){
//			float displayValue = 0;
//			if (currentMode == TEMPERATURE){
//				displayValue = filtered_temperature;
//			}else if (currentMode == PITCH){
//				displayValue = pitch; // get temperature/accelerometer data depending on the current mode
//			}else if (currentMode == ROLL){
//				displayValue = roll; // get temperature/accelerometer data depending on the current mode
//			}
//			
//			if (filtered_temperature > TEMPERATURE_THRESHOLD_FOR_ALARM) {
//				int i;
//				for (i = 0; i < ALARM_DELAY/NORMAL_DELAY; i++) {
//					displayOn7Seg(displayValue); // get temperature/accelerometer data depending o nS
//					osDelay(NORMAL_DELAY);
//				}
//				Reset7SegDisplay();
//				osDelay(ALARM_DELAY);
//			} else {
//				displayOn7Seg(displayValue);
//				osDelay(NORMAL_DELAY);
//			}
		}
	}
	
/*----------------------------------------------------------------------------
 *      
 *---------------------------------------------------------------------------*/

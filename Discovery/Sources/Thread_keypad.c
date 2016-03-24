/**
	******************************************************************************
  * @file    Thread_keypad.c
  * @author  Mathieu Wang & Russell Buchanan
	* @version V1.0.0
  * @date    18-March-2016
  * @brief   This file defines and initializes the keypad thread
  ******************************************************************************
  */
	
#include "stm32f4xx_hal.h"
#include "cmsis_os.h"                   // ARM::CMSIS:RTOS:Keil RTX
#include "main.h"

#define NORMAL_DELAY 2 																				/** Normal delay of the keypad polling: 5ms */

void Thread_keypad (void const *argument);                 		/** thread function */
osThreadId tid_Thread_keypad;                               	/** thread id */
osThreadDef(Thread_keypad, osPriorityAboveNormal, 1, 96);			/** thread definition with above normal priority and a stack size of 96 = 1.5 * 64 (max observed stack usage) */
DisplayMode currentMode;																			/** current mode of display: TEMPERATURE, PITCH, or ROLL */
KeypadState currentState = READY_FOR_KEY_PRESS; 							/** current state of the keypad */
int button_press_ready_counter = 0;             							/** a counter that keeps track of how many milliseconds have passed since the last release */
int BUTTON_PRESS_DELAY = 50;                    							/** delay in ms between a release event and the next possible keypad input */

/**----------------------------------------------------------------------------
 *      Create the thread within RTOS context
 *---------------------------------------------------------------------------*/
int start_Thread_keypad (void) {
  tid_Thread_keypad = osThreadCreate(osThread(Thread_keypad), NULL); // Start Thread_temperature
  if (!tid_Thread_keypad) return(-1); 
  return(0);
}

/**----------------------------------------------------------------------------
 *      Thread  'Thread_keypad': get keypad button presses and set current state
 *---------------------------------------------------------------------------*/
	void Thread_keypad (void const *argument) {
		while(1){
			char currentKey = getKeypadChar();

			if (currentState == READY_FOR_KEY_PRESS) {                         // Currently in READY_FOR_KEY_PRESS state
				if (currentKey != '-') {                                         // Got a key
					currentState = KEY_PRESSED;                                    // Set state to KEY_PRESSED
				}
			} else if (currentState == KEY_PRESSED) {                          // Currently in KEY_PRESSED state
					if (currentKey == '-') {                                       // No key is currently being pressed
						currentState = KEY_RELEASED;
					} else {                                                       // One of the keys is pressed, stay in pressed state
						currentState = KEY_PRESSED;
					}
			}                                      
		
			if (button_press_ready_counter > BUTTON_PRESS_DELAY) {         // KEY_RELEASED state
				currentState = READY_FOR_KEY_PRESS;
				button_press_ready_counter = 0;
			}
			button_press_ready_counter++;

			if (currentKey == '1')
				currentMode = TEMPERATURE;
			else if (currentKey == '2')
				currentMode = PITCH;
			else if (currentKey == '3')
				currentMode = ROLL;
			
			osDelay(NORMAL_DELAY);
		}
	}

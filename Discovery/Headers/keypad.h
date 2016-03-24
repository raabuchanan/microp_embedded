/**
  ******************************************************************************
  * @file    keypad.h
  * @author  Mathieu Wang & Russell Buchanan
  * @version V 1.0.0
  * @date    March 11, 2016
  * @brief   This file provides a function that gets the character currently
  * being selected on the 3x4 alphanumeric keypad. It also contains an enum
  * definition for the different keypad states
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef KEYPAD_H
#define KEYPAD_H

/* Includes ------------------------------------------------------------------*/


/**
 * @brief      Get character currently being selected on the 3x4 alphanumeric
 * keypad
 *
 * @return     the character currently being selected
 */
char getKeypadChar(void);

typedef enum
{
  KEY_PRESSED = 0,
	KEY_RELEASED,
	READY_FOR_KEY_PRESS
} KeypadState;

#endif

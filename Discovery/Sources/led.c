
/**
  ******************************************************************************
  * @file    led.c
  * @author  Mathieu Wang & Russell Buchanan
  * @version V 1.0.0
  * @date    March 11, 2016
  * @brief   This file provides functions that sets the LEDs on the
  * board to a left or right arrow shape and a function that resets all LEDs.
  ******************************************************************************
  */

#include "led.h"


/**
 * @brief      Reset all LEDs
 */
void ResetLEDs(void) {
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_RESET);
}
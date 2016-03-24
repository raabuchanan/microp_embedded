/**
  ******************************************************************************
  * @file    sevenseg.h
  * @author  Mathieu Wang & Russell Buchanan
  * @version V 2.0.0
  * @date    March 16, 2016
  * @brief   This file provides functions that displays a float on the
  * 7-segment display and a function that resets the display.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef SEVENSEG_H
#define SEVENSEG_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "util.h"

/**
 * @brief      Display a float on the 7-segment display.
 *
 * @param[in]  number       the float to be displayed
 */
void displayOn7Seg(float number) ;

/**
 * @brief      Reset the 7-segment display
 */
void Reset7SegDisplay(void);

#endif

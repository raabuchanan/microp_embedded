/**
  ******************************************************************************
  * @file    timer.h
  * @author  Mathieu Wang & Russell Buchanan
  * @version V 1.0.0
  * @date    March 11, 2016
  * @brief   This file provides a function that initializes the hardware timer
  * TIM3 to tick at 1 kHz.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef TIMER_H
#define TIMER_H

/**
 * @brief      Initialize the hardware timer TIM3
 *
 * @param      timHandleTypeDef  the timer handle, which should be initialized
 * with enough memory
 */
void initTimer(void);

#endif

/**
  ******************************************************************************
  * @file    led.h
  * @author  Mathieu Wang & Russell Buchanan
  * @version V 1.0.0
  * @date    March 11, 2016
  * @brief   This file provides functions that sets the LEDs on the
  * board to a left or right arrow shape and a function that resets all LEDs.
  * It also contains an enum definition for the different directions
  ******************************************************************************
  */


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef LED_H
#define LED_H

#define MAXSPEED 50

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

void LED_GPIO_Init(void);
void LED_GPIO_DeInit(void);

#endif
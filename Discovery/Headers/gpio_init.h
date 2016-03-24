/**
  ******************************************************************************
  * @file    gpio_init.h
  * @author  Mathieu Wang & Russell Buchanan
  * @version V 1.0.0
  * @date    March 11, 2016
  * @brief   This file provides a function that initializes the GPIOs with the
  * appriopriate configurations
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef GPIO_H
#define GPIO_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/**
 * @brief      Initialize the GPIOs and set the configurations
 */
void initGPIOs(void);

#endif

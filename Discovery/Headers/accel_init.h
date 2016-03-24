/**
  ******************************************************************************
  * @file    accel_init.h
  * @author  Mathieu Wang & Russell Buchanan
  * @version V 1.0.0
  * @date    March 11, 2016
  * @brief   This file provides a function that initializes the LIS3DSH with the
  * appriopriate configurations
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef ACCEL_INIT_H
#define ACCEL_INIT_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "lis3dsh.h"

/**
 * @brief      Initialize the LIS3DSH accelerometer and set the configurations
 */
void initAccelerometers(void);

#endif

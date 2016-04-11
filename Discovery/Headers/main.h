/*******************************************************************************
  * @file    main.h
  * @author  Ashraf Suyyagh
	* @version V1.2.0
  * @date    17-January-2016
  * @brief   This file demonstrates flasing one LED at an interval of one second
	*          RTX based using CMSIS-RTOS 
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef MAIN_H
#define MAIN_H

/* Includes ------------------------------------------------------------------*/

#include "gpio_init.h"
#include "accel_init.h"
#include "keypad.h"
#include "sevenseg.h"
#include "math.h"
#include "kalmanfilter_c.h"
#include "timer.h"

typedef enum
{
	TEMPERATURE = 0x01,
	PITCH = 0x02,
	ROLL = 0x03
} DisplayMode;

extern TIM_HandleTypeDef* timHandleTypeDef;
extern TIM_HandleTypeDef    PWM_config;
#endif

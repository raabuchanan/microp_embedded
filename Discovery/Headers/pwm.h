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
#ifndef PWM_H
#define PWM_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"              // Keil::Device:STM32Cube HAL:Common

void LED_PWM_Init(void);
void drive_PWM_Init(void);
void LED_PWM_DeInit(void);

void set_green_pwm(uint32_t intensity);
void set_orange_pwm(uint32_t intensity);
void set_red_pwm(uint32_t intensity);
void set_blue_pwm(uint32_t intensity);

void set_drive_left(uint32_t intensity);
void set_drive_right(uint32_t intensity);

#endif

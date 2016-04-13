
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


void LED_GPIO_Init(void){
	
	GPIO_InitTypeDef GPIO_InitStruct;
	
	__GPIOD_CLK_ENABLE(); //GPIOD for LEDs
	
	/*Green LED*/
	GPIO_InitStruct.Pin = GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct); 
	
	/*Orange LED*/
	GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct); 
	
	/*Red LED*/
	GPIO_InitStruct.Pin = GPIO_PIN_14;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct); 
	
	/*Blue LED*/
	GPIO_InitStruct.Pin = GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct); 
}

void LED_GPIO_DeInit(void){
	
	HAL_GPIO_DeInit(GPIOD, GPIO_PIN_12);
	HAL_GPIO_DeInit(GPIOD, GPIO_PIN_13);
	HAL_GPIO_DeInit(GPIOD, GPIO_PIN_14);
	HAL_GPIO_DeInit(GPIOD, GPIO_PIN_15);
	
	__GPIOD_CLK_DISABLE();
	
}


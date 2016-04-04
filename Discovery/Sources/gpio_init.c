/**
  ******************************************************************************
  * @file    gpio_init.c
  * @author  Mathieu Wang & Russell Buchanan
  * @version V 1.1.0
  * @date    March 16, 2016
  * @brief   This file provides a function that initializes the GPIOs with the
  * appriopriate configurations
  ******************************************************************************
  */

#include "gpio_init.h"

/**
 * @brief      Initialize the GPIOs and set the configurations
 */
void initGPIOs(void) {
	GPIO_InitTypeDef GPIOA_InitType;
//	GPIO_InitTypeDef GPIOB_InitType;
	GPIO_InitTypeDef GPIOC_InitType;
	GPIO_InitTypeDef GPIOD_InitType;
	GPIO_InitTypeDef GPIOE_InitType;

	/* Enable GPIO clocks */
	__HAL_RCC_GPIOA_CLK_ENABLE();
//	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOE_CLK_ENABLE();

	/* Initialize GPIOs */

	//GPIOs A, C, D used for 7-segment display and LEDs
	GPIOA_InitType.Pin = GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3; //Only enable used pins
	GPIOA_InitType.Mode = GPIO_MODE_OUTPUT_PP;
	GPIOA_InitType.Pull = GPIO_PULLUP;
	GPIOA_InitType.Speed = GPIO_SPEED_FREQ_HIGH;

	GPIOC_InitType.Pin = GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_4 | GPIO_PIN_5; //Only enable used pins
	GPIOC_InitType.Mode = GPIO_MODE_OUTPUT_PP;
	GPIOC_InitType.Pull = GPIO_PULLUP;
	GPIOC_InitType.Speed = GPIO_SPEED_FREQ_HIGH;

	GPIOD_InitType.Pin = GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11; //Only enable used pins
	GPIOD_InitType.Mode = GPIO_MODE_OUTPUT_PP;
	GPIOD_InitType.Pull = GPIO_PULLUP;
	GPIOD_InitType.Speed = GPIO_SPEED_FREQ_HIGH;

	//GPIOE only used for accelerometer interrupt
	GPIOE_InitType.Pin = GPIO_PIN_0;
	GPIOE_InitType.Mode = GPIO_MODE_IT_RISING;
	GPIOE_InitType.Pull = GPIO_PULLDOWN;
	GPIOE_InitType.Speed = GPIO_SPEED_FREQ_LOW;

//	// GPIOB used for keypad data lines (3 coloumns and 4 rows)
//	GPIOB_InitType.Pin =  GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14; //Only enable used pins
//	GPIOB_InitType.Mode = GPIO_MODE_INPUT;
//	GPIOB_InitType.Pull = GPIO_PULLDOWN; // Pulldown for the keypad pins because we have a 3x4 keypad, with 7 lines for rows anc cols, and 1 line connected to Vdd, so when we pull down, the button will connect the row line to the column line to the power line, so we can read the data from the GPIO pin.
//	GPIOB_InitType.Speed = GPIO_SPEED_FREQ_VERY_HIGH;

	/* Initialize GPIOs */
	HAL_GPIO_Init(GPIOA, &GPIOA_InitType);
//	HAL_GPIO_Init(GPIOB, &GPIOB_InitType);
	HAL_GPIO_Init(GPIOC, &GPIOC_InitType);
	HAL_GPIO_Init(GPIOD, &GPIOD_InitType);
	HAL_GPIO_Init(GPIOE, &GPIOE_InitType);
}

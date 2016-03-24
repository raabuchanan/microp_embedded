/**
  ******************************************************************************
  * @file    keypad.c
  * @author  Mathieu Wang & Russell Buchanan
  * @version V 1.0.0
  * @date    March 11, 2016
  * @brief   This file provides a function that gets the character currently
  * being selected on the 3x4 alphanumeric keypad.
  ******************************************************************************
  */

#include "main.h"

/**
 * @brief      Get character currently being selected on the 3x4 alphanumeric
 * keypad
 *
 * @return     the character currently being selected
 */
char getKeypadChar(void) {

	GPIO_PinState col0, col1, col2;
	GPIO_PinState row0, row1, row2, row3;

	// Read all pins at once because we have an extra line always connected to Vdd (See gpio_init.c for more explanation)
	col0 = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0);
	col1 = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1);
	col2 = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_2);
	row0 = HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_11);
	row1 = HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_12);
	row2 = HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_13);
	row3 = HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_14);

	//printf("%d, %d, %d, %d\n", row0, row1, row2, row3);
	if (col0 == GPIO_PIN_SET) {
		if (row0==GPIO_PIN_SET)
			return '1';
		else if(row1==GPIO_PIN_SET)
			return '4';
		else if(row2==GPIO_PIN_SET)
			return '7';
		else if(row3==GPIO_PIN_SET)
			return '*';
	} else if (col1 == GPIO_PIN_SET) {
		if (row0==GPIO_PIN_SET)
			return '2';
		else if(row1==GPIO_PIN_SET)
			return '5';
		else if(row2==GPIO_PIN_SET)
			return '8';
		else if(row3==GPIO_PIN_SET)
			return '0';
	} else if (col2 == GPIO_PIN_SET) {
		if (row0==GPIO_PIN_SET)
			return '3';
		else if(row1==GPIO_PIN_SET)
			return '6';
		else if(row2==GPIO_PIN_SET)
			return '9';
		else if(row3==GPIO_PIN_SET)
			return '#';
	}
	return '-'; // default is dash, which means not read
}

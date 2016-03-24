/**
  ******************************************************************************
  * @file    sevenseg.c
  * @author  Mathieu Wang & Russell Buchanan
  * @version V 2.0.0
  * @date    March 16, 2016
  * @brief   This file provides functions that displays a float on the
  * 7-segment display and a function that resets the display.
  ******************************************************************************
  */

#include "sevenseg.h"

/* Private function prototypes -----------------------------------------------*/
void ResetSelectLines(void);
void SetSelectLine(int digit);
void SetSegment(char segment);
void Set7SegDigit (char digitChar, int digit, int withDecimal);

/* Private variables ---------------------------------------------------------*/
int digit_index = 0; /** index of the digit that currently is displayed. Always either 0, 1, or 2. */

/**
 * @brief      Display a float on the 7-segment display.
 *
 * @param[in]  number       the float to be displayed
 * @param      digit_index  a pointer to the digit index. The value of this
 * pointer should be initialized to 0 in the calling environment.
 */
void displayOn7Seg(float number) {
	/* Define variables to hold results of float to chars conversion */
	char digitZero = '0';
	char digitOne = '0';
	char digitTwo = '0';
	int decimalPointLocation;

	convertFloatToChars(number, &digitZero, &digitOne, &digitTwo, &decimalPointLocation);

	switch (digit_index)
	{
		case 0:
			Set7SegDigit(digitZero, digit_index, digit_index == decimalPointLocation);
			break;
		case 1:
			Set7SegDigit(digitOne, digit_index, digit_index == decimalPointLocation);
			break;
		case 2:
			Set7SegDigit(digitTwo, digit_index, 0); // never put decimal point after last digit
			break;
		default:
			printf("ERROR: the seven_seg_digit_index should be 0, 1, or 2");
			break;
	}
	digit_index = (digit_index + 1) % 3; // loop through values 0, 1, and 2
}

/**
 * @brief      Reset the 7-segment display. Turn off all segments
 */
void Reset7SegDisplay(void) {
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, GPIO_PIN_RESET);
}

/**
 * @brief      Reset all 4 select lines of the display so a new digit can be
 * selected later
 */
void ResetSelectLines(void) {
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, GPIO_PIN_RESET);
}

/**
 * @brief      Set one of the select lines so a digit is selected
 *
 * @param[in]  digit  the digit to be selected. 0 is the most significant digit
 * whereas 3 is the least significant digit
 */
void SetSelectLine(int digit) {
	switch (digit)
	{
		case 0:
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, GPIO_PIN_SET); // digit 0 (MSB) = PD8
			break;
		case 1:
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9, GPIO_PIN_SET); // digit 1 = PD9
			break;
		case 2:
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10, GPIO_PIN_SET); // digit 2 = PD10
			break;
		case 3:
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, GPIO_PIN_SET); // digit 3 = PD11
			break;
		default :
			break;
	}
}

/**
 * @brief      Set a specific digit to a character. Also display a decimal point
 * after this digit if specified.
 *
 * @param[in]  digitChar    the digit to be displayed
 * @param[in]  digit        the index of the digit to be displayed
 * @param[in]  withDecimal  boolean that specifies whether a decimal point
 * should be displayed after this digit
 */
void Set7SegDigit (char digitChar, int digit, int withDecimal) {
	ResetSelectLines();
	SetSelectLine(digit);
	Reset7SegDisplay();

	switch(digitChar)
	{
		case '0':
			SetSegment('A');
			SetSegment('B');
			SetSegment('C');
			SetSegment('D');
			SetSegment('E');
			SetSegment('F');
			break;
		case '1':
			SetSegment('B');
			SetSegment('C');
			break;
		case '2':
			SetSegment('A');
			SetSegment('B');
			SetSegment('G');
			SetSegment('E');
			SetSegment('D');
			break;
		case '3':
			SetSegment('A');
			SetSegment('B');
			SetSegment('G');
			SetSegment('C');
			SetSegment('D');
			break;
		case '4':
			SetSegment('F');
			SetSegment('G');
			SetSegment('B');
			SetSegment('C');
			break;
		case '5':
			SetSegment('A');
			SetSegment('F');
			SetSegment('G');
			SetSegment('C');
			SetSegment('D');
			break;
		case '6':
			SetSegment('A');
			SetSegment('C');
			SetSegment('D');
			SetSegment('E');
			SetSegment('G');
			SetSegment('F');
			break;
		case '7':
			SetSegment('A');
			SetSegment('B');
			SetSegment('C');
			break;
		case '8':
			SetSegment('A');
			SetSegment('B');
			SetSegment('C');
			SetSegment('D');
			SetSegment('E');
			SetSegment('F');
			SetSegment('G');
			break;
		case '9':
			SetSegment('A');
			SetSegment('B');
			SetSegment('C');
			SetSegment('D');
			SetSegment('F');
			SetSegment('G');
			break;
		default :
			break;
	}
	if (withDecimal) {
		SetSegment('.');
	}
}

/**
 * @brief      Set a specific segment
 *
 * @param[in]  segment  the segment to be set
 */
void SetSegment(char segment) {
	switch(segment)
	{
		//No PA13, 14, B3!
		case 'A':
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_SET); //A = PC1
			break;
		case 'B':
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_SET); //B = PC2
			break;
		case 'C':
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET); //C = PA1
			break;
		case 'D':
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_SET); //D = PA2
			break;
		case 'E':
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_SET); //E = PA3
			break;
		case 'F':
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_SET); //F = PC4
			break;
		case 'G':
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_SET); //G = PC5
			break;
		case '.':
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, GPIO_PIN_SET); //. = PD7
			break;

		default :
			break;
	}
}


/**
  ******************************************************************************
  * @file    util.h
  * @author  Mathieu Wang & Russell Buchanan
  * @version V 1.0.0
  * @date    March 11, 2016
  * @brief   This file provides utility functions that convert characters to
  * floats or vice-versa
  ******************************************************************************
  */

#include "util.h"

/**
 * @brief      Convert characters to float, assuming the input string is a valid
 * float
 *
 * @param      str   string containing characters that can be converted to a
 * float
 *
 * @return     the converted float
 */
float convertCharsToFloat(char* str) {
	if (str[0] == 'x') return -1.0f; // not read
	else if (str[1] == 'x') str[1] = '\0';
	else if (str[2] == 'x') str[2] = '\0';
	else str[3] = '\0';

	return atof(str);
}

/**
 * @brief      Convert float to characters
 *
 * @param[in]  number                the float to be converted
 * @param      digitZero             the character representing the first digit
 * of the float
 * @param      digitOne              the character representing the second digit
 * of the float
 * @param      digitTwo              the character representing the third digit
 * of the float
 * @param      decimalPointLocation  the location of the decimal point. Is equal
 * to the index of the digit right before the decimal point
 */
void convertFloatToChars(float number, char* digitZero, char* digitOne, char* digitTwo, int* decimalPointLocation) {
	char digitHundreds = ((int)number / 100) + '0';
	char digitTens = (((int)number % 100) / 10) + '0';
	char digitOnes = ((int)number % 10) + '0';
	char digitTenths = ((int)(number * 10) % 10) + '0';
	char digitHundredths = ((int)(number * 100) % 10) + '0';

	if (digitHundreds == '0' && digitTens == '0') {
		*digitZero = digitOnes;
		*digitOne = digitTenths;
		*digitTwo = digitHundredths;
		*decimalPointLocation = 0;
	} else if (digitHundreds == '0') {
		*digitZero = digitTens;
		*digitOne = digitOnes;
		*digitTwo = digitTenths;
		*decimalPointLocation = 1;
	} else {
		*digitZero = digitHundreds;
		*digitOne = digitTens;
		*digitTwo = digitOnes;
		*decimalPointLocation = 2;
	}
}

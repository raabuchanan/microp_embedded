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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef UTIL_H
#define UTIL_H

/* Includes ------------------------------------------------------------------*/
#include <stdlib.h>
#include <stdint.h>

/**
 * @brief Union containing the integer representation (32-bit IEEE-754) of a float
 */
typedef union {
	float f;
  int i;
} f32_u;

/**
 * @brief      Convert characters to float, assuming the input string is a valid
 * float
 *
 * @param      str   string containing characters that can be converted to a
 * float
 *
 * @return     the converted float
 */
float convertCharsToFloat(char* str);

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
void convertFloatToChars(float number, char* digitZero, char* digitOne, char* digitTwo, int* decimalPointLocation);

/**
 * @brief 		Convert a float to its IEEE-754 representation in 4 bytes
 *
 * @param[in[ buff									the 4-byte long buffer containing the result
 * @param[in] f											the float to be converted
 */
void convertFloatToBytes(uint8_t buff[], float f);

#endif

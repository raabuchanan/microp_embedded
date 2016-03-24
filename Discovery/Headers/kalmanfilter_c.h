/**
  ******************************************************************************
  * @file    kalmanfilter_c.h
  * @author  Mathieu Wang & Russell Buchanan
  * @version V 1.0.0
  * @date    March 11, 2016
  * @brief   This file provides a function that filters a float value using
  * Kalman filter. It also contains a struct definition for the state variables
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef KALMANFILTER_C_H
#define KALMANFILTER_C_H

/* Includes ------------------------------------------------------------------*/
#include "stdio.h"
#include <stdlib.h>

/* Exported types ------------------------------------------------------------*/
typedef struct kalman_state kalman_state;

struct kalman_state
{
	float q;
	float r;
	float x;
	float p;
	float k;
};

/* Exported functions ------------------------------------------------------- */
/**
 * @brief      Kalman filter
 *
 * @param[in]  input   the input to be filtered
 * @param      kstate  the struct holding the current Kalman state values
 *
 * @return     the filtered value
 */
float kalmanfilter_c(float input, kalman_state* kstate);

#endif /* KALMAN_FILTER_C_H */

/**
  ******************************************************************************
  * @file    kalmanfilter_c.c
  * @author  Mathieu Wang & Russell Buchanan
  * @version V 1.0.0
  * @date    March 11, 2016
  * @brief   This file provides a function that filters a float value using
  * Kalman filter.
  ******************************************************************************
  */

/** Includes ------------------------------------------------------------------*/
#include "kalmanfilter_c.h"

/**
 * @brief      Kalman filter
 *
 * @param[in]  input   the input to be filtered
 * @param      kstate  the struct holding the current Kalman state parameters
 *
 * @return     the filtered value
 */
float kalmanfilter_c(float input, kalman_state* kstate)
{
	kstate->p = kstate->p + kstate->q;
	kstate->k = kstate->p / (kstate->p + kstate->r);
	kstate->x = kstate->x + kstate->k * (input - kstate->x);
	kstate->p = (1 - kstate->k) * kstate->p;

	return kstate->x;
}

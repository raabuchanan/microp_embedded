/**
  ******************************************************************************
  * @file    timer.c
  * @author  Mathieu Wang & Russell Buchanan
  * @version V 1.1.0
  * @date    March 16, 2016
  * @brief   This file provides a function that initializes the hardware timer
  * TIM3 to tick at 100 Hz and gives the interrupt a priority of 1.
  ******************************************************************************
  */

#include "stm32f4xx_hal.h"

/**
 * @brief      Initialize the hardware timer TIM3
 *
 * @param      timHandleTypeDef  the timer handle, which should be initialized
 * with enough memory
 */
void initTimer(TIM_HandleTypeDef* timHandleTypeDef) {
	TIM_Base_InitTypeDef timInitTypeDef;

	__TIM3_CLK_ENABLE(); // Enable clock for TIM3 timer

	timInitTypeDef.Prescaler = 1000;
	timInitTypeDef.CounterMode = TIM_COUNTERMODE_DOWN;
	timInitTypeDef.Period = 840; // 840 * 1000 = 840 000 --> final frequency is 100 Hz because clock is 84MHz
	timInitTypeDef.ClockDivision = TIM_CLOCKDIVISION_DIV1;

	timHandleTypeDef -> Instance = TIM3;
	timHandleTypeDef -> Init = timInitTypeDef;

	HAL_TIM_Base_Init(timHandleTypeDef);
	HAL_TIM_Base_Start_IT(timHandleTypeDef);

	HAL_NVIC_SetPriority(TIM3_IRQn, 0, 1); // timer (used to check for keypad presses) has higher priority than accelerometer to make sure an accelerometer reading will not block the keypad reading when the two interrupts conflict
	HAL_NVIC_EnableIRQ(TIM3_IRQn);

	//SET_BIT(DBGMCU->APB1FZ, DBGMCU_APB1_FZ_DBG_TIM3_STOP); // Stop TIM3 when debugging
}

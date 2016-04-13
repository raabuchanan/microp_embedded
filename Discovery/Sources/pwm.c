/**
  ******************************************************************************
  * @file    pwm.c
  * @author  
  * @version V 1.0.0
  * @date    
  * @brief   
  ******************************************************************************
  */
#include "pwm.h"

void pwm_init(void){
	TIM_HandleTypeDef pwmHandleTypeDef;
	TIM_Base_InitTypeDef pwmInitTypeDef;
	TIM_OC_InitTypeDef pwmOCInitTypeDef;
	
	pwmOCInitTypeDef.OCFastMode = TIM_OCFAST_DISABLE;
	pwmOCInitTypeDef.OCMode = TIM_OCMODE_PWM1;
	pwmOCInitTypeDef.OCPolarity = TIM_OCPOLARITY_HIGH;
	pwmOCInitTypeDef.Pulse = 100; //Maximum
	
	pwmInitTypeDef.Prescaler = 200;
	pwmInitTypeDef.CounterMode = TIM_COUNTERMODE_DOWN;
	pwmInitTypeDef.Period = 840; // 840 * 200 = 840 000 --> final frequency is 500 Hz because clock is 84MHz
	pwmInitTypeDef.ClockDivision = TIM_CLOCKDIVISION_DIV1;

	pwmHandleTypeDef.Instance = TIM4;
	pwmHandleTypeDef.Init = pwmInitTypeDef;

	HAL_TIM_PWM_Init(&pwmHandleTypeDef);
	HAL_TIM_PWM_ConfigChannel(&pwmHandleTypeDef, &pwmOCInitTypeDef, TIM_CHANNEL_1); //Channel 1 for Green

//	HAL_NVIC_SetPriority(TIM3_IRQn, 0, 1); // timer (used to check for keypad presses) has higher priority than accelerometer to make sure an accelerometer reading will not block the keypad reading when the two interrupts conflict
//	HAL_NVIC_EnableIRQ(TIM3_IRQn);
	HAL_TIM_PWM_Start(&pwmHandleTypeDef, TIM_CHANNEL_1);
}

void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	__TIM4_CLK_ENABLE(); // Enable clock for TIM4 timer
	
	__GPIOD_CLK_ENABLE(); //GPIOD for LEDs
	
	/*Green LED*/
	GPIO_InitStruct.Pin = GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF2_TIM4;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct); 
	
}





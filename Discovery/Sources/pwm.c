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
TIM_HandleTypeDef pwmLEDsHandleTypeDef;
TIM_Base_InitTypeDef pwmLEDsInitTypeDef;

TIM_OC_InitTypeDef greenOCInitTypeDef;
TIM_OC_InitTypeDef orangeOCInitTypeDef;
TIM_OC_InitTypeDef redOCInitTypeDef;
TIM_OC_InitTypeDef blueOCInitTypeDef;


void pwm_init_LEDs(void){

	pwmLEDsInitTypeDef.Prescaler = 200;
	pwmLEDsInitTypeDef.CounterMode = TIM_COUNTERMODE_DOWN;
	pwmLEDsInitTypeDef.Period = 840; // 840 * 200 = 840 000 --> final frequency is 500 Hz because clock is 84MHz
	pwmLEDsInitTypeDef.ClockDivision = TIM_CLOCKDIVISION_DIV1;

	pwmLEDsHandleTypeDef.Instance = TIM4;
	pwmLEDsHandleTypeDef.Init = pwmLEDsInitTypeDef;

	HAL_TIM_PWM_Init(&pwmLEDsHandleTypeDef);
	
	greenOCInitTypeDef.OCFastMode = TIM_OCFAST_DISABLE;
	greenOCInitTypeDef.OCMode = TIM_OCMODE_PWM1;
	greenOCInitTypeDef.OCPolarity = TIM_OCPOLARITY_HIGH;
	greenOCInitTypeDef.Pulse = 0; //Initialize to zero intensity

	orangeOCInitTypeDef.OCFastMode = TIM_OCFAST_DISABLE;
	orangeOCInitTypeDef.OCMode = TIM_OCMODE_PWM1;
	orangeOCInitTypeDef.OCPolarity = TIM_OCPOLARITY_HIGH;
	orangeOCInitTypeDef.Pulse = 0; //Initialize to zero intensity
	
	redOCInitTypeDef.OCFastMode = TIM_OCFAST_DISABLE;
	redOCInitTypeDef.OCMode = TIM_OCMODE_PWM1;
	redOCInitTypeDef.OCPolarity = TIM_OCPOLARITY_HIGH;
	redOCInitTypeDef.Pulse = 0; //Initialize to zero intensity
	
	blueOCInitTypeDef.OCFastMode = TIM_OCFAST_DISABLE;
	blueOCInitTypeDef.OCMode = TIM_OCMODE_PWM1;
	blueOCInitTypeDef.OCPolarity = TIM_OCPOLARITY_HIGH;
	blueOCInitTypeDef.Pulse = 0; //Initialize to zero intensity
	
	
	HAL_TIM_PWM_ConfigChannel(&pwmLEDsHandleTypeDef, &greenOCInitTypeDef, TIM_CHANNEL_1); //Channel 1 for Green
	HAL_TIM_PWM_ConfigChannel(&pwmLEDsHandleTypeDef, &orangeOCInitTypeDef, TIM_CHANNEL_2); //Channel 2 for Orange
	HAL_TIM_PWM_ConfigChannel(&pwmLEDsHandleTypeDef, &redOCInitTypeDef, TIM_CHANNEL_3); //Channel 3 for Red
	HAL_TIM_PWM_ConfigChannel(&pwmLEDsHandleTypeDef, &blueOCInitTypeDef, TIM_CHANNEL_4); //Channel 4 for Blue

	HAL_TIM_PWM_Start(&pwmLEDsHandleTypeDef, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&pwmLEDsHandleTypeDef, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&pwmLEDsHandleTypeDef, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&pwmLEDsHandleTypeDef, TIM_CHANNEL_4);
	
}

void set_green_pwm(uint32_t intensity){
	HAL_TIM_PWM_Stop(&pwmLEDsHandleTypeDef, TIM_CHANNEL_1);
	greenOCInitTypeDef.Pulse = intensity;
	HAL_TIM_PWM_ConfigChannel(&pwmLEDsHandleTypeDef, &greenOCInitTypeDef, TIM_CHANNEL_1); //Channel 1 for Green
	HAL_TIM_PWM_Start(&pwmLEDsHandleTypeDef, TIM_CHANNEL_1);
}

void set_orange_pwm(uint32_t intensity){
	HAL_TIM_PWM_Stop(&pwmLEDsHandleTypeDef, TIM_CHANNEL_2);
	orangeOCInitTypeDef.Pulse = intensity;
	HAL_TIM_PWM_ConfigChannel(&pwmLEDsHandleTypeDef, &orangeOCInitTypeDef, TIM_CHANNEL_2); //Channel 2 for Orange
	HAL_TIM_PWM_Start(&pwmLEDsHandleTypeDef, TIM_CHANNEL_2);
}
void set_red_pwm(uint32_t intensity){
	HAL_TIM_PWM_Stop(&pwmLEDsHandleTypeDef, TIM_CHANNEL_3);
	redOCInitTypeDef.Pulse = intensity;
	HAL_TIM_PWM_ConfigChannel(&pwmLEDsHandleTypeDef, &redOCInitTypeDef, TIM_CHANNEL_3); //Channel 3 for Red
	HAL_TIM_PWM_Start(&pwmLEDsHandleTypeDef, TIM_CHANNEL_3);
}
void set_blue_pwm(uint32_t intensity){
	HAL_TIM_PWM_Stop(&pwmLEDsHandleTypeDef, TIM_CHANNEL_4);
	blueOCInitTypeDef.Pulse = intensity;
	HAL_TIM_PWM_ConfigChannel(&pwmLEDsHandleTypeDef, &blueOCInitTypeDef, TIM_CHANNEL_4); //Channel 4 for Blue
	HAL_TIM_PWM_Start(&pwmLEDsHandleTypeDef, TIM_CHANNEL_4);
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
	
	/*Orange LED*/
	GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF2_TIM4;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct); 
	
	/*Red LED*/
	GPIO_InitStruct.Pin = GPIO_PIN_14;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF2_TIM4;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct); 
	
	/*Blue LED*/
	GPIO_InitStruct.Pin = GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF2_TIM4;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct); 
	
}





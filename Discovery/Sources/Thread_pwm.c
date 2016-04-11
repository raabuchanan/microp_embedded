/**
	******************************************************************************
  * @file    Thread_temperature.c
  * @author  Mathieu Wang & Russell Buchanan
	* @version V1.0.0
  * @date    18-March-2016
  * @brief   This file initializes the ADC, reads the temperature, and provides
	* a function to get the Kalman-filtered data temperature.
  ******************************************************************************
  */
	
#include "cmsis_os.h"                   // ARM::CMSIS:RTOS:Keil RTX
#include "stm32f4xx_hal.h"
#include "main.h"

void Thread_pwm (void const *argument);                 	/** thread function */
osThreadId tid_Thread_pwm;                              	/** thread id */
osThreadDef(Thread_pwm, osPriorityBelowNormal, 1, 348);		/** thread definition with below normal priority and a stack size of 348 = 1.5 * 232 (max observed stack usage) */
//GPIO_InitTypeDef 				LED_configuration;
//GPIO_InitTypeDef 				PWM_configuration;
TIM_HandleTypeDef    PWM_config;
																				/** the temperature mutex definition */

/**----------------------------------------------------------------------------
 *      Create the thread within RTOS context
 *---------------------------------------------------------------------------*/
int start_Thread_pwm (void) {

  tid_Thread_pwm = osThreadCreate(osThread(Thread_pwm), NULL); // Start Thread_pwm
  if (!tid_Thread_pwm) return(-1); 
  return(0);
}

/**----------------------------------------------------------------------------
 *      Thread  'Thread_pwm': do PWM stuff
 *---------------------------------------------------------------------------*/
	void Thread_pwm (void const *argument) {
		
		
		
		while(1){
				osDelay(1000);
				HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
		}
	}
	
/**----------------------------------------------------------------------------
 *      Initialize GPIO for LEDs
 *---------------------------------------------------------------------------*/
void initializeLED_IO (void){
	
	/*__HAL_RCC_GPIOD_CLK_ENABLE();
	__TIM2_CLK_ENABLE();
	
	LED_configuration.Pin		= GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
	LED_configuration.Mode 	= GPIO_MODE_OUTPUT_PP;
	LED_configuration.Speed	= GPIO_SPEED_FREQ_VERY_HIGH;
	LED_configuration.Pull	= GPIO_NOPULL;
	
	HAL_GPIO_Init(GPIOD, &LED_configuration);
	
	PWM_configuration.Pin		= GPIO_PIN_11; //TIM2_CH4
	PWM_configuration.Mode 	= GPIO_MODE_OUTPUT_PP;
	PWM_configuration.Speed	= GPIO_SPEED_FREQ_VERY_HIGH;
	PWM_configuration.Pull	= GPIO_NOPULL;
		
	HAL_GPIO_Init(GPIOB, &PWM_configuration);
	
	PWM_config.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	PWM_config.Init.CounterMode = TIM_COUNTERMODE_DOWN;
	PWM_config.Init.Period = 200;
	PWM_config.Init.Prescaler = 24;
	PWM_config.Init.RepetitionCounter = 0x00;
	PWM_config.Channel = HAL_TIM_ACTIVE_CHANNEL_4;
	PWM_config.Instance = TIM2;
	
	HAL_TIM_PWM_Init(&PWM_config);
	HAL_NVIC_SetPriority(TIM2_IRQn, 0, 1); // timer (used to check for keypad presses) has higher priority than accelerometer to make sure an accelerometer reading will not block the keypad reading when the two interrupts conflict
	HAL_NVIC_EnableIRQ(TIM2_IRQn);*/
}


/*----------------------------------------------------------------------------
 *      
 *---------------------------------------------------------------------------*/

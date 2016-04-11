/**
	*****************************************************************************
  * @file    main.c
  * @author  Ashraf Suyyagh
	* @version V1.2.0
  * @date    17-January-2016
  * @brief   This file demonstrates flasing one LED at an interval of one second
	*          RTX based using CMSIS-RTOS 
  ******************************************************************************
  */

#include "stm32f4xx_hal.h"              // Keil::Device:STM32Cube HAL:Common
#include "cmsis_os.h"                   // ARM::CMSIS:RTOS:Keil RTX
#include "RTE_Components.h"             // Component selection
#include "main.h"
#include "nucleo_interface.h"

extern void initializeADC_IO			(void);
extern void start_Thread_sevenseg			(void);
extern void start_Thread_temperature			(void);
extern void start_Thread_angles			(void);
extern void start_Thread_keypad		(void);
extern void Thread_sevenseg(void const *argument);
extern void Thread_temperature(void const *argument);
extern void Thread_keypad(void const *argument);
extern osThreadId tid_Thread_temperature;
extern osThreadId tid_Thread_sevenseg;
extern osThreadId tid_Thread_keypad;
extern osThreadId tid_Thread_angles;
extern SPI_HandleTypeDef nucleoSPIHandle;
extern void initializeLED_IO (void);
extern TIM_HandleTypeDef    PWM_config;
int pwm = 0;
int started = 100;
int initialized = 100;
GPIO_InitTypeDef 				LED_configuration;
GPIO_InitTypeDef 				PWM_configuration;
TIM_OC_InitTypeDef oc_config;
TIM_HandleTypeDef* timHandleTypeDef;            /** timer handler to be initialized */
osMutexId temp_mutex;														/** temperature mutex */
osMutexId pitch_mutex;													/** pitch mutex */
osMutexId roll_mutex;														/** roll mutex */

SPI_HandleTypeDef SPI_Handle;

/**
	These lines are mandatory to make CMSIS-RTOS RTX work with te new Cube HAL
*/
#ifdef RTE_CMSIS_RTOS_RTX
extern uint32_t os_time;

uint32_t HAL_GetTick(void) { 
  return os_time; 
}
#endif

/**
  * System Clock Configuration
  */
void SystemClock_Config(void) {
  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();

  /* The voltage scaling allows optimizing the power consumption when the
     device is clocked below the maximum system frequency (see datasheet). */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
     clocks dividers */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 |
                                RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);
}

/**
  * Main function
  */
int main (void) {
	uint8_t data[18];
	HAL_StatusTypeDef Rx;
	char test[8];

  //osKernelInitialize();                     /* initialize CMSIS-RTOS          */

  HAL_Init();                               /* Initialize the HAL Library     */
	
  SystemClock_Config();                     /* Configure the System Clock     */
	
	/* Initialize GPIOs */
//	initGPIOs();

	//nucleo_SPI_init();
	//initializeLED_IO();

	//HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET);
	//HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
	//HAL_TIM_PWM_Start_IT(&PWM_config, TIM_CHANNEL_4);
	/* Initialize the ADC IO */
//	initializeADC_IO();

	/* Initialize the accelerometer configs */
//	initAccelerometers();

	/* Initialize timer */
//timHandleTypeDef = malloc(sizeof(*timHandleTypeDef));
//initTimer(timHandleTypeDef);

	/* Start the threads */
	//start_Thread_angles();
	//start_Thread_temperature();
	//start_Thread_sevenseg();
	//start_Thread_keypad();
  
	//osKernelStart();  /* start thread execution*/
	
	/*while(1){
		
//		Rx = HAL_SPI_Receive (&nucleoSPIHandle, data, 18, 1);
			convertFloatToBytes(data+1, 22.5f);
		
//		if (data[0] == '!' && data[17] == '$'){
			test[0] = data[1];
			test[1] = data[2];
			test[2] = data[3];
			test[3] = data[4];
			test[4] = data[5];
			test[5] = data[6];
			test[6] = data[7];
			test[7] = data[8];
//		}

	}*/
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_TIM4_CLK_ENABLE(); 
	__TIM4_CLK_ENABLE();
	
	LED_configuration.Pin		= GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
	LED_configuration.Mode 	= GPIO_MODE_OUTPUT_PP;
	LED_configuration.Speed	= GPIO_SPEED_FREQ_VERY_HIGH;
	LED_configuration.Pull	= GPIO_NOPULL;
	LED_configuration.Alternate = GPIO_AF2_TIM4;
	HAL_GPIO_Init(GPIOD, &LED_configuration);
//	
//	PWM_configuration.Pin		= GPIO_PIN_11; //TIM2_CH4
//	PWM_configuration.Mode 	= GPIO_MODE_OUTPUT_PP;
//	PWM_configuration.Speed	= GPIO_SPEED_FREQ_VERY_HIGH;
//	PWM_configuration.Pull	= GPIO_NOPULL;
//	PWM_configuration.Alternate = GPIO_AF2_TIM4;
//		
//	HAL_GPIO_Init(GPIOB, &PWM_configuration);
	
	PWM_config.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	PWM_config.Init.CounterMode = TIM_COUNTERMODE_DOWN;
	PWM_config.Init.Period = 200;
	PWM_config.Init.Prescaler = 24;
	PWM_config.Init.RepetitionCounter = 0xF0;
	PWM_config.Channel = HAL_TIM_ACTIVE_CHANNEL_1;
	PWM_config.Instance = TIM4;	

  oc_config.OCMode = TIM_OCMODE_PWM2;
  oc_config.Pulse = 1000;
  oc_config.OCNIdleState = TIM_OCNIDLESTATE_SET;
  oc_config.OCIdleState = TIM_OCIDLESTATE_RESET;
  oc_config.OCFastMode = TIM_OCFAST_ENABLE;
	oc_config.OCNPolarity = TIM_OCPOLARITY_LOW;
	
	HAL_TIM_OC_Init(&PWM_config);
	HAL_TIM_OC_ConfigChannel(&PWM_config, &oc_config, TIM_CHANNEL_1);
	HAL_NVIC_SetPriority(TIM4_IRQn, 0, 1); // timer (used to check for keypad presses) has higher priority than accelerometer to make sure an accelerometer reading will not block the keypad reading when the two interrupts conflict
	HAL_NVIC_EnableIRQ(TIM4_IRQn);
	initialized = HAL_TIM_PWM_Init(&PWM_config);
	started = HAL_TIM_PWM_Start_IT(&PWM_config, TIM_CHANNEL_1);
	HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
	//HAL_TIM_Base_Init(&PWM_config);
	//HAL_TIM_Base_Start_IT(&PWM_config);
	while (1)
  {
	for (pwm=0;pwm<=200;pwm++)  //darkest to brightest: 0-100% duty cycle
		{
		__HAL_TIM_SetCompare(&PWM_config, TIM_CHANNEL_4, pwm); //update pwm value
		HAL_Delay(10);
		}
	HAL_Delay(400);  //hold for 400ms
	for (pwm=200;pwm>=0;pwm--)  //brightest to darkest: 100-0% duty cycle
		{
		__HAL_TIM_SetCompare(&PWM_config, TIM_CHANNEL_4, pwm);
		HAL_Delay(10);
		}
	HAL_Delay(400);   //hold for 400ms

 /* USER CODE END WHILE */
  /* USER CODE BEGIN 3 */
 }
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	osSignalSet(tid_Thread_angles, 1);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	//osSignalSet(tid_Thread_temperature, 1);
	if (htim->Instance == TIM4)
	{		
		HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
	}
	
}
/*void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == TIM2)
	{		
		count++;
		HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
	}
}*/

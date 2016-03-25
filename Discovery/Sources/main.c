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
	uint8_t data;
  osKernelInitialize();                     /* initialize CMSIS-RTOS          */

  HAL_Init();                               /* Initialize the HAL Library     */
	

	
  SystemClock_Config();                     /* Configure the System Clock     */
	
	/* Initialize GPIOs */
	initGPIOs();

	/* Initialize the ADC IO */
	initializeADC_IO();

	/* Initialize the accelerometer configs */
	initAccelerometers();

	/* Initialize timer */
	timHandleTypeDef = malloc(sizeof(*timHandleTypeDef));
	initTimer(timHandleTypeDef);

	/* Start the threads */
	start_Thread_angles();
	start_Thread_temperature();
	//start_Thread_sevenseg();
	//start_Thread_keypad();
  
	osKernelStart();  /* start thread execution         */
	
	__HAL_RCC_SPI1_CLK_ENABLE();
	
	HAL_SPI_MspInit(&SPI_Handle); /* Initialize the SPI low level resources */
	
	HAL_SPI_DeInit(&SPI_Handle);
	
	SPI_Handle.Instance = SPI2;
	SPI_Handle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
	SPI_Handle.Init.CLKPhase = SPI_PHASE_1EDGE;
	SPI_Handle.Init.CLKPolarity = SPI_POLARITY_LOW;
	SPI_Handle.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	SPI_Handle.Init.CRCPolynomial = 7;
	SPI_Handle.Init.DataSize = SPI_DATASIZE_8BIT;
	SPI_Handle.Init.Direction = SPI_DIRECTION_2LINES;
	SPI_Handle.Init.FirstBit = SPI_FIRSTBIT_MSB;
	SPI_Handle.Init.Mode = SPI_MODE_SLAVE;
	SPI_Handle.Init.NSS = SPI_NSS_SOFT;
	SPI_Handle.Init.TIMode = SPI_TIMODE_DISABLED;
	
	HAL_SPI_Init(&SPI_Handle);

	__HAL_SPI_ENABLE(&SPI_Handle);
	
	while(1){
	HAL_SPI_Receive (&SPI_Handle, &data, 0x01, 10);
	}
	
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	osSignalSet(tid_Thread_angles, 1);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	osSignalSet(tid_Thread_temperature, 1);
}

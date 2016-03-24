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

void Thread_temperature (void const *argument);                 	/** thread function */
osThreadId tid_Thread_temperature;                              	/** thread id */
osThreadDef(Thread_temperature, osPriorityBelowNormal, 1, 348);		/** thread definition with below normal priority and a stack size of 348 = 1.5 * 232 (max observed stack usage) */

ADC_HandleTypeDef ADC1_Handle;																		/** the ADC handle type */
ADC_InitTypeDef ADC1_InitType;																		/** the ADC init type */
ADC_ChannelConfTypeDef ADC1_Channel;															/** the ADC channel type */
HAL_ADC_StateTypeDef ADC1_State;																	/** the ADC state type */
HAL_StatusTypeDef ADC1_Status;																		/** the ADC status type */
HAL_LockTypeDef ADC1_Lock;																				/** the ADC lock type */
DMA_HandleTypeDef ADC1_DMA_Handle;																/** the ADC DMA handle type */
uint32_t ADC1_Data;																								/** the ADC data */
uint32_t ADC1_ErrorCode;																					/** the ADC error code */
kalman_state kstate = {1, 100, 1000, 0, 0}; // {q, r, x, p, k}		/** the Kalman state values for temperature filtering */
float	filtered_voltage;																					  /** the filtered voltage */
float filtered_temperature;																				/** the filtered temperature */
extern osMutexId temp_mutex;																			/** the temperature mutex */
osMutexDef(temp_mutex);																						/** the temperature mutex definition */

/**----------------------------------------------------------------------------
 *      Create the thread within RTOS context
 *---------------------------------------------------------------------------*/
int start_Thread_temperature (void) {

  tid_Thread_temperature = osThreadCreate(osThread(Thread_temperature), NULL); // Start Thread_temperature
  if (!tid_Thread_temperature) return(-1); 
  return(0);
}

/**----------------------------------------------------------------------------
 *      Thread  'Thread_temperature': get temperature data
 *---------------------------------------------------------------------------*/
	void Thread_temperature (void const *argument) {
		
		temp_mutex = osMutexCreate(osMutex(temp_mutex));
		
		while(1){
			osSignalWait(1, osWaitForever);

			HAL_ADC_Start(&ADC1_Handle);
			HAL_ADC_PollForConversion(&ADC1_Handle, 100); // 100ms timeout
			ADC1_Data = HAL_ADC_GetValue(&ADC1_Handle);
			HAL_ADC_Stop(&ADC1_Handle);
			__HAL_ADC_CLEAR_FLAG(&ADC1_Handle, ADC_FLAG_EOC);
			
			filtered_voltage = kalmanfilter_c(ADC1_Data, &kstate);
			
			osMutexWait(temp_mutex, osWaitForever);
			filtered_temperature = (filtered_voltage/4096.0f)*3*400 - 279; // convert from voltage to temperature in degrees Celsius			
			osMutexRelease(temp_mutex);
			
			osSignalClear(tid_Thread_temperature, 1);
		}
	}

/**----------------------------------------------------------------------------
 *      Initialize the ADC
 *---------------------------------------------------------------------------*/
void initializeADC_IO (void){
	
	__HAL_RCC_ADC1_CLK_ENABLE();
	
	/* Set ADC Type */
	ADC1_InitType.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
	ADC1_InitType.Resolution = ADC_RESOLUTION_12B;
	ADC1_InitType.DataAlign = ADC_DATAALIGN_RIGHT;
	ADC1_InitType.ScanConvMode = DISABLE;
	ADC1_InitType.EOCSelection = ADC_EOC_SINGLE_CONV;
	ADC1_InitType.ContinuousConvMode = DISABLE;
	ADC1_InitType.DMAContinuousRequests = DISABLE;
	ADC1_InitType.NbrOfConversion = 1;
	ADC1_InitType.DiscontinuousConvMode = DISABLE;
	ADC1_InitType.NbrOfDiscConversion = 1;
	ADC1_InitType.ExternalTrigConv = ADC_SOFTWARE_START;
	ADC1_InitType.ExternalTrigConvEdge = ADC_SOFTWARE_START;

	/* Set ADC Handler */
	ADC1_Handle.Instance = ADC1;
	ADC1_Handle.Init = ADC1_InitType;
	ADC1_Handle.NbrOfCurrentConversionRank = 1;
	ADC1_Handle.DMA_Handle = &ADC1_DMA_Handle;
	ADC1_Handle.Lock = ADC1_Lock;
	ADC1_Handle.State = ADC1_State;
	ADC1_Handle.ErrorCode = ADC1_ErrorCode;
	
	/* Set Channel Parameters*/
	ADC1_Channel.Channel = ADC_CHANNEL_16;
	ADC1_Channel.Offset = 0;
	ADC1_Channel.SamplingTime = ADC_SAMPLETIME_480CYCLES;
	ADC1_Channel.Rank = 1;
	
	/* Configure ADC Channel*/
	HAL_ADC_ConfigChannel(&ADC1_Handle, &ADC1_Channel);

}
	
/*----------------------------------------------------------------------------
 *      
 *---------------------------------------------------------------------------*/

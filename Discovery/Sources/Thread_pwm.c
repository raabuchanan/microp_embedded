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
int start_Thread_pwm (void) {

  tid_Thread_pwm = osThreadCreate(osThread(Thread_pwm), NULL); // Start Thread_temperature
  if (!tid_Thread_pwm) return(-1); 
  return(0);
}

/**----------------------------------------------------------------------------
 *      Thread  'Thread_temperature': get temperature data
 *---------------------------------------------------------------------------*/
	void Thread_pwm (void const *argument) {
		
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
			
			osSignalClear(tid_Thread_pwm, 1);
		}
	}


/*----------------------------------------------------------------------------
 *      
 *---------------------------------------------------------------------------*/

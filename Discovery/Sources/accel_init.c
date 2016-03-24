/**
  ******************************************************************************
  * @file    accel_init.c
  * @author  Mathieu Wang & Russell Buchanan
  * @version V 1.0.0
  * @date    March 11, 2016
  * @brief   This file provides a function that initializes the LIS3DSH with the
  * appriopriate configurations
  ******************************************************************************
  */

#include "accel_init.h"

/**
 * @brief      Initialize the LIS3DSH accelerometer and set the configurations.
 */
void initAccelerometers(void) {
	/* TypeDef Declarations */
	LIS3DSH_InitTypeDef LIS3DSH_InitType;
	LIS3DSH_DRYInterruptConfigTypeDef LIS3DSH_DRYInterruptConfigType;

	/* Initialize Accelerometer */
	LIS3DSH_InitType.AA_Filter_BW = LIS3DSH_AA_BW_50; // Use min value because our sampling frequency is low (25 Hz). Filters out most frequencies larger than half of our sampling frequency
  LIS3DSH_InitType.Axes_Enable = LIS3DSH_XYZ_ENABLE;
	LIS3DSH_InitType.Continous_Update = LIS3DSH_ContinousUpdate_Disabled; // Disable continuous update to make sure we have full reading (with both LSB and MSB) before updating the output register
	LIS3DSH_InitType.Full_Scale = LIS3DSH_FULLSCALE_2; // 2G because we don't expect to apply a lot of external force to the board. This way we get the max resolution
	LIS3DSH_InitType.Power_Mode_Output_DataRate = LIS3DSH_DATARATE_25; //Specified in Lab manual
  LIS3DSH_InitType.Self_Test = LIS3DSH_SELFTEST_NORMAL;
	LIS3DSH_Init(&LIS3DSH_InitType);

	/* Set InterruptConfigType parameters*/
	LIS3DSH_DRYInterruptConfigType.Dataready_Interrupt = LIS3DSH_DATA_READY_INTERRUPT_ENABLED;
	LIS3DSH_DRYInterruptConfigType.Interrupt_signal = LIS3DSH_ACTIVE_HIGH_INTERRUPT_SIGNAL;
	LIS3DSH_DRYInterruptConfigType.Interrupt_type = LIS3DSH_INTERRUPT_REQUEST_PULSED;
	LIS3DSH_DataReadyInterruptConfig(&LIS3DSH_DRYInterruptConfigType);

	/* Set Interrupt parameters*/
	HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0); // lower priority than timer (used for keypad presses) to make sure accelerometer readings will not block keypad readings (which occur much more rarely)
	HAL_NVIC_EnableIRQ(EXTI0_IRQn);
}

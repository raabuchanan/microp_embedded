/**
	******************************************************************************
  * @file    Thread_angles.c
  * @author  Mathieu Wang & Russell Buchanan
	* @version V1.0.0
  * @date    18-March-2016
  * @brief   This file defines and runs the thread to get roll and pitch angles from accelerometers
  ******************************************************************************
  */
	
#include "cmsis_os.h"                   // ARM::CMSIS:RTOS:Keil RTX
#include "stm32f4xx_hal.h"
#include "main.h"

#define M_PI 3.14159265358979323846            							/** an approximation of pi */
#define XOFFSET (-75.11 - 42.78)/2              						/** offset on the x-axis as calculated from calibration measurements */
#define YOFFSET (-9.68 - 3.98)/2                						/** offset on the y-axis as calculated from calibration measurements */
#define ZOFFSET (-65.13 - 19.04)/2              						/** offset on the z-axis as calculated from calibration measurements */

void Thread_angles (void const *argument);                  /** thread function */
osThreadId tid_Thread_angles;                               /** thread id */
osThreadDef(Thread_angles, osPriorityBelowNormal, 1, 864);  /** thread definition with below normal priority and a stack size of 864 = 1.5 * 576 (max observed stack usage) */
extern osThreadId tid_Thread_doubleTap;

float filteredX;
float filteredY;
float filteredZ;
float filtered_acc[3];
float pitch /** the pich angle */, roll 										/** the roll angle */;
kalman_state xacc_state = {5,100,0,0,0};       							/** Kalman state variables for the X Acceleration */
kalman_state yacc_state = {5,100,0,0,0};        						/** Kalman state variables for the Y Acceleration */
kalman_state zacc_state = {5,100,0,0,0};        						/** Kalman state variables for the Z Acceleration */

extern osMutexId pitch_mutex;																/** pitch mutex */
extern osMutexId roll_mutex;															  /** roll mutex */
osMutexDef(pitch_mutex);																		
osMutexDef(roll_mutex);

/**----------------------------------------------------------------------------
 *      Create the thread within RTOS context
 *---------------------------------------------------------------------------*/
int start_Thread_angles (void) {

  tid_Thread_angles = osThreadCreate(osThread(Thread_angles), NULL); // Start Thread_sevenseg
  if (!tid_Thread_angles) return(-1); 
  return(0);
}

/**----------------------------------------------------------------------------
 *      Thread  'Thread_angles': set Pitch and Roll display
 *---------------------------------------------------------------------------*/
	void Thread_angles (void const *argument) {
		
		pitch_mutex = osMutexCreate(osMutex(pitch_mutex));
		roll_mutex = osMutexCreate(osMutex(roll_mutex));
		
		while(1){
			float out[3], acc[3];
			
			osSignalWait(1, osWaitForever);
			
			LIS3DSH_ReadACC(out);

			/* Calculations based on Eq 8 and 9 in Doc 15*/
			/* Y direction is towards mini usb connector, Z is downwards*/
			/* Use RHR to digure out X*/

			acc[0] = out[0] - XOFFSET;
			acc[1] = out[1] - YOFFSET;
			acc[2] = out[2] - ZOFFSET;
			
			filtered_acc[0] = kalmanfilter_c(acc[0],&xacc_state);
			filteredX = filtered_acc[0];
			filtered_acc[1] = kalmanfilter_c(acc[1],&yacc_state);
			filteredY = filtered_acc[1];
			filtered_acc[2] = kalmanfilter_c(acc[2],&zacc_state);
			filteredZ = filtered_acc[2];

			osSignalSet(tid_Thread_doubleTap, 1);
			
			osMutexWait(pitch_mutex, osWaitForever);
			pitch = 90 + (180/M_PI)*atan2(filtered_acc[1], sqrt(pow(filtered_acc[0], 2) + pow(filtered_acc[2], 2))); //Pitch in degrees
			osMutexRelease(pitch_mutex);
	
			osMutexWait(roll_mutex, osWaitForever);
			roll  = 90 + (180/M_PI)*atan2(filtered_acc[0], sqrt(pow(filtered_acc[1], 2) + pow(filtered_acc[2], 2))); //Roll in degrees
			osMutexRelease(roll_mutex);
			
			osSignalClear(tid_Thread_angles, 1);
		}
	}

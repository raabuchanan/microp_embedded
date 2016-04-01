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

void Thread_doubleTap (void const *argument);                  /** thread function */
osThreadId tid_Thread_doubleTap;                               /** thread id */
osThreadDef(Thread_doubleTap, osPriorityAboveNormal, 1, 864);  /** thread definition with below normal priority and a stack size of 864 = 1.5 * 576 (max observed stack usage) */

int readingIndex = 0;
int i = 0;
int j = 0;
int flag = 0;
int doubleTap = 0;
int updateAvg = 0;
int cyclePassed = 0;
int beginCountdown = 0;
float active = 0;
float spikingAverage[] = {1000,1000,1000,1000,1000,1000,1000,1000,1000,1000};
float globalAverageZ[] = {1000,1000,1000,1000,1000,1000,1000,1000,1000,1000};
float globalAverageX[] = {1000,1000,1000,1000,1000,1000,1000,1000,1000,1000};
float globalAverageY[] = {1000,1000,1000,1000,1000,1000,1000,1000,1000,1000};

float average;
float returnGap = 0;
float test;//output the zaxis
float previousAvgZ;
float previousAvgX;
float previousAvgY;
float runningAvgZ;//output average with high value included
float runningAvgX;//output average with high value included
float runningAvgY;//output average with high value included

extern float filtered_acc[3];
extern osMutexId pitch_mutex;																/** pitch mutex */
extern osMutexId roll_mutex;															  /** roll mutex */
osMutexDef(pitch_mutex);																		
osMutexDef(roll_mutex);

/**----------------------------------------------------------------------------
 *      Create the thread within RTOS context
 *---------------------------------------------------------------------------*/
int start_Thread_doubleTap (void) {

  tid_Thread_doubleTap = osThreadCreate(osThread(Thread_doubleTap), NULL); // Start Thread_sevenseg
  if (!tid_Thread_doubleTap) return(-1); 
  return(0);
}

/**----------------------------------------------------------------------------
 *      Thread  'Thread_angles': set Pitch and Roll display
 *---------------------------------------------------------------------------*/
	void Thread_doubleTap (void const *argument) {
				
		while(1){
			/*************************************
				         Temporary init
			**************************************/
			osSignalWait(1, osWaitForever);
			if(beginCountdown)
				cyclePassed++;
			test = filtered_acc[2];
			if(j<50)
			{
				spikingAverage[readingIndex++%10] = filtered_acc[2];
				j++;
			}
			else
			{			
				//
				globalAverageX[readingIndex%10] = filtered_acc[0];
				globalAverageY[readingIndex%10] = filtered_acc[1];
				globalAverageZ[readingIndex++%10] = filtered_acc[2];

				for(i = 0,average = 0,runningAvgZ = 0, runningAvgX = 0, runningAvgY = 0; i<10 ;i++)
				{
					average += spikingAverage[i];
					runningAvgX += globalAverageX[i];
					runningAvgY += globalAverageY[i];
					runningAvgZ += globalAverageZ[i];
				}
				average /= 10;	
				runningAvgZ /= 10;
				runningAvgX /= 10;
				runningAvgY /= 10;
				
				//Waits for stable value of the average 
				//to make sure the board is not moving
				if(fabs(previousAvgZ-runningAvgZ)<5 && 
					 fabs(previousAvgX-runningAvgX)<5 &&
					 fabs(previousAvgY-runningAvgY)<5)
				{
					//Values safe to use 
					for(i = 0; i<10 ;i++)
						//Stores all values from the gloabl running average 
						//to the local average to detect spike
						spikingAverage[i] = globalAverageZ[i];
					//unlock the flag
					updateAvg = 1;
				}
				else 
					//locak the flag
					updateAvg = 0;
				
				//Runs only when boad is stable ie. When it is not moving
				if(updateAvg)
				{
					//if tap detected
					if(fabs(filtered_acc[2] - average)>10 && fabs(filtered_acc[2] - average)<50)
					{
						//If first spike, save the spiked value
						//And record as first tap
						if(flag == 0)
						{
							active = filtered_acc[2];
							beginCountdown = 1;
							returnGap = fabs(filtered_acc[2] - average);
							flag++;
						}		
						//Once tapped, if nothing else happens, value should return to
						//approximately average value, thus if another spike detected 
						//then, double tap!						
						else
						{
							//If filtered acceleration is getting smaller,
							//then board returning to original position
							if(fabs(filtered_acc[2] - average) < returnGap)
								returnGap = fabs(filtered_acc[2] - average);
							else
								flag++;							
						}
						//If flagged twice, then double tap detected
						if(flag == 2)
						{
							doubleTap++;
							flag = 0;
						}						
						/*flag++;	
						beginCountdown = 1;
						if(flag >= 4)
						{
							doubleTap++;
							flag = 0;
						}*/
					}
					else
						//Does not include the "spiked" value in the average
						//calculation
						spikingAverage[readingIndex++%10] = filtered_acc[2];
				}		
				if(cyclePassed > 10)
				{
					flag = 0;
					cyclePassed = 0;
					beginCountdown = 0;
					active = 0;
					returnGap = 0;
				}
				previousAvgZ = runningAvgZ;
				previousAvgX = runningAvgX;
				previousAvgY = runningAvgY;
			}
			osSignalClear(tid_Thread_doubleTap, 1);
		}
	}

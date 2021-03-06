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

void Thread_doubleTap(void const *argument);                  /** thread function */
float computeAvg(float* avg);
osThreadId tid_Thread_doubleTap;                               /** thread id */
osThreadDef(Thread_doubleTap, osPriorityAboveNormal, 1, 250);  

int readingIndex = 0;
int i = 0;
int j = 0;
int tappedOnce = 0;
int tappedTwice = 0;
int doubleTap = 0;
int updateAvg = 0;
int tappedTimer = 0;
int cyclePassed = 0;
int beginCountdown = 0;
int debounce = 1;
int skippedIteration = 0;
int spiked = 0;
int doubleTapFlag = 0;

float active = 0;
float spikingAverage[10];
float globalAverageZ[10];
float globalAverageX[10];
float globalAverageY[10];
float previousAverageZ[10];
float previousAverageX[10];
float previousAverageY[10];

float average;
float returnGap = 0;
float test;//output the zaxis
float previousAvgZ;
float previousAvgX;
float previousAvgY;
float runningAvgZ;//output average with high value included
float runningAvgX;//output average with high value included
float runningAvgY;//output average with high value included*/
kalman_state kstateX = {1, 100, 1060, 0, 0}; // {q, r, x, p, k}
kalman_state kstateY = {1, 100, 1060, 0, 0}; // {q, r, x, p, k}
kalman_state kstateZ = {1, 100, 1060, 0, 0}; // {q, r, x, p, k}
kalman_state spikingKState =  {1, 100, 1060, 0, 0}; // {q, r, x, p, k}
extern float filteredX;
extern float filteredY;
extern float filteredZ;

/**----------------------------------------------------------------------------
 *      Create the thread within RTOS context
 *---------------------------------------------------------------------------*/
int start_Thread_doubleTap (void) {

  tid_Thread_doubleTap = osThreadCreate(osThread(Thread_doubleTap), NULL);
  if (tid_Thread_doubleTap == NULL) return(-1); 
  return(0);
}

/**----------------------------------------------------------------------------
 *      Thread  'Thread_angles': set Pitch and Roll display
 *---------------------------------------------------------------------------*/
	void Thread_doubleTap(void const *argument) 
	{
		
		while(1){
			/*************************************
				         Temporary init
			**************************************/
			osSignalWait(1, osWaitForever);
			if(beginCountdown)
				cyclePassed++;
			test = filteredZ;
			if(j<20)
			{		

				runningAvgX = kalmanfilter_c(filteredX, &kstateX);
				runningAvgY = kalmanfilter_c(filteredY, &kstateY);
				runningAvgZ = kalmanfilter_c(filteredZ, &kstateZ);
				if(j<10)
				{
					previousAverageX[j] = runningAvgX;
					previousAverageY[j] = runningAvgY;
					previousAverageZ[j] = runningAvgZ;
				}
				else if(j>10)
				{
					globalAverageX[j%10] = runningAvgX;
					globalAverageY[j%10] = runningAvgY;
					globalAverageZ[j%10] = runningAvgZ;
				}
				previousAvgX = computeAvg(previousAverageX);
				previousAvgY = computeAvg(previousAverageY);
				previousAvgZ = computeAvg(previousAverageZ);

				runningAvgX = computeAvg(globalAverageX);
				runningAvgY = computeAvg(globalAverageY);
				runningAvgZ = computeAvg(globalAverageZ);
				j++;
			}
			else
			{			
//				for(i = 0,average = 0,runningAvgZ = 0, runningAvgX = 0, runningAvgY = 0; i<10 ;i++)
//				{
//					average += spikingAverage[i];
//					runningAvgX += globalAverageX[i];
//					runningAvgY += globalAverageY[i];
//					runningAvgZ += globalAverageZ[i];
//				}
//				average /= 10;	
//				runningAvgZ /= 10;
//				runningAvgX /= 10;
//				runningAvgY /= 10;
				runningAvgX = computeAvg(globalAverageX);
				runningAvgY = computeAvg(globalAverageY);
				runningAvgZ = computeAvg(globalAverageZ);
				//Waits for stable value of the average 
				//to make sure the board is not moving
				if(fabs(previousAvgZ-runningAvgZ)<5 && 
					 fabs(previousAvgX-runningAvgX)<5 &&
					 fabs(previousAvgY-runningAvgY)<5)
				{

						//Values safe to use 
//						for(i = 0; i<10 ;i++)
//						{
//							//Stores all values from the gloabl running average 
//							//to the local average to detect spike
//							//if(fabs(filteredZ - average) < 50)
//								spikingAverage[i] = globalAverageZ[i];
//						}
						//unlock the flag
						if(updateAvg == 0)
						{
							spikingKState.q = 1;
							spikingKState.r = 100;
							spikingKState.x = 1060;
							spikingKState.p = 0;
							spikingKState.k = 0;
							average =  kalmanfilter_c(filteredZ, &spikingKState);
						}
						updateAvg = 1;

				}
				else
					//lock the flag
					updateAvg = 0;
				
				//Runs only when boad is stable ie. When it is not moving
				if(updateAvg)
				{					
					if(debounce>0)
						debounce--;
					//if tap detected
					if(average - filteredZ  >10 /*&& fabs(filteredZ - average)<100*/ && debounce == 0)
					{
						//If first spike, save the spiked value
						//And record as first tap
						if(!tappedOnce)
						{
							active = filteredZ;
							beginCountdown = 1;
							tappedOnce = 1;
						}		
						//Once tapped, if nothing else happens, value should return to
						//approximately average value, thus if another spike detected 
						//then, double tap!						
						else
						{
							tappedTimer++;
							//If filtered acceleration is getting smaller,
							//then board returning to original position
							/*if(fabs(filteredZ - average) < returnGap)
								returnGap = fabs(filteredZ - average);*/
							if(tappedTimer > 12)
								tappedTwice = 1;							
						}
						//If flagged twice, then double tap detected
						if(tappedTwice)
						{
							doubleTapFlag = 1;
							doubleTap++;
							tappedOnce = 0;
							tappedTwice = 0;
							tappedTimer = 0;
							cyclePassed = 30;
							debounce = 20;
						}		
					}
					else
						average =  kalmanfilter_c(filteredZ, &spikingKState);
						
				}		
				globalAverageX[readingIndex%10] = kalmanfilter_c(filteredX, &kstateX);
				globalAverageY[readingIndex%10] = kalmanfilter_c(filteredX, &kstateY);
				globalAverageZ[readingIndex++%10] = kalmanfilter_c(filteredX, &kstateZ);
				if(cyclePassed > 20)
				{
					tappedOnce = 0;
					tappedTwice = 0;
					cyclePassed = 0;
					beginCountdown = 0;
					tappedTimer = 0;
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

float computeAvg(float* avg)
{
	int i = 0;
	float average = 0;
	for(i=0;i<10;i++)
	{
		average += avg[i];
	}
	average /= 10;
	return average;
}
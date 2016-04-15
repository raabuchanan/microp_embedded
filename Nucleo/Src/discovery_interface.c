/**
	******************************************************************************
  * @file    discovery_interface.c
  * @author  
	* @version V1.0.0
  * @date    
  * @brief   
  ******************************************************************************
  */

#include "discovery_interface.h"
#include "main.h"
#include "sensor_service.h"

SPI_HandleTypeDef discoverySPIHandle;
uint8_t data[RX_PKG_SIZE];
uint8_t pkg[TX_PKG_SIZE];
extern uint8_t LEDIntensity;
extern uint8_t LEDSpeed;
int IS_TRANSMITTING = 0;
int RXCounter = 0;
int successfulRX = 0;

void discovery_SPI_init(void)
{
  discoverySPIHandle.Instance = SPI2; /*SPI2 is used since SPI1 is reserved for the accelerometer*/
  discoverySPIHandle.Init.Mode = SPI_MODE_MASTER;/*The Nucleo is set as SPI Master*/
  discoverySPIHandle.Init.Direction = SPI_DIRECTION_2LINES; /*Two-way communication with Discovery*/
  discoverySPIHandle.Init.DataSize = SPI_DATASIZE_8BIT;
  discoverySPIHandle.Init.CLKPolarity = SPI_POLARITY_LOW;
  discoverySPIHandle.Init.CLKPhase = SPI_PHASE_1EDGE;
  discoverySPIHandle.Init.NSS = SPI_NSS_SOFT;/*There is only one Master and one Slave*/
  discoverySPIHandle.Init.FirstBit = SPI_FIRSTBIT_MSB;
  discoverySPIHandle.Init.TIMode = SPI_TIMODE_DISABLED;
  discoverySPIHandle.Init.CRCPolynomial = 7;
  discoverySPIHandle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;/*Operating at lower speeds apear to improve communication*/
  discoverySPIHandle.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  
  HAL_SPI_Init(&discoverySPIHandle);
	
}


/* 
	Recieves data from Discovery. Activated by interrpt callback
  If data was successfully transmitted the phone is updated 
	over bluetooth
*/
int doubleTapTest = 0;
HAL_StatusTypeDef update_phone(void){
	HAL_StatusTypeDef updateStatus;
	RXCounter ++;
	updateStatus = HAL_SPI_Receive (&discoverySPIHandle, data, RX_PKG_SIZE,1);
	
	for (int i=0;i<3;i++){
		if(data[i] == '!' && data[i+1] == '!' && data[i+2] == '!'){
			
			Acc_Update(data+i+3);
			Temp_Update(data+i+11);
			
			if (data[i+15]==0x01){
				Double_Tap_Notify();
				doubleTapTest ++;
			}
			
			successfulRX++;
		}
}
	return updateStatus;
}

/*Collects Data and formats into package then sends to discovery*/
HAL_StatusTypeDef update_discovery(void){
	IS_TRANSMITTING = 1; /*Flag to prevent recieving from interupting*/
	HAL_StatusTypeDef txStatus; 
	pkg[0]  = '!';/*Build Package with leading '!' and terminating '$'*/
	pkg[1]  = '!';
	pkg[2]  = '!';
	pkg[3]  = LEDIntensity;
	pkg[4]  = LEDSpeed;
	pkg[5]  = '$';
	pkg[6]  = '$';
	pkg[7] = '$';
	
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET);/*Send signal on GPIO line to nucleo*/
	txStatus = HAL_SPI_Transmit(&discoverySPIHandle, pkg, TX_PKG_SIZE,1);/*Transmit Data*/
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_RESET);
	IS_TRANSMITTING = 0;
	return txStatus;/*HAL_OK indicates sucesful transmision*/
}





/**
	******************************************************************************
  * @file    nucleo_interface.c
  * @author  
	* @version V1.0.0
  * @date    
  * @brief   
  ******************************************************************************
  */
	
#include "nucleo_interface.h"
#include "util.h"
#include "main.h"

SPI_HandleTypeDef nucleoSPIHandle;
extern float filtered_temperature;
extern float pitch, roll;
extern int doubleTapFlag;
uint8_t data[RX_PKG_SIZE];
uint8_t pkg[TX_PKG_SIZE];
int IS_TRANSMITTING = 0;
int RXCounter = 0;
int successfulRX = 0;
int speed = 0;
int intensity = 0;

void nucleo_SPI_init(void)
{
  nucleoSPIHandle.Instance = SPI2; /*SPI2 is used since SPI1 is reserved for the accelerometer*/
  nucleoSPIHandle.Init.Mode = SPI_MODE_SLAVE;  /*The Discovery is set as SPI slave*/
  nucleoSPIHandle.Init.Direction = SPI_DIRECTION_2LINES; /*Two-way communication with nucleo*/
  nucleoSPIHandle.Init.DataSize = SPI_DATASIZE_8BIT; 
  nucleoSPIHandle.Init.CLKPolarity = SPI_POLARITY_LOW;
  nucleoSPIHandle.Init.CLKPhase = SPI_PHASE_1EDGE;
  nucleoSPIHandle.Init.NSS = SPI_NSS_SOFT; /*There is only one Master and one Slave*/
  nucleoSPIHandle.Init.FirstBit = SPI_FIRSTBIT_MSB;
  nucleoSPIHandle.Init.TIMode = SPI_TIMODE_DISABLED;
  nucleoSPIHandle.Init.CRCPolynomial = 7;
  nucleoSPIHandle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256; /*Clock is set by Master*/
  nucleoSPIHandle.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  
  HAL_SPI_Init(&nucleoSPIHandle);
	
}


/*Collects Data and formats into package then sends to nucleo*/
HAL_StatusTypeDef send_pkg(void){

	HAL_StatusTypeDef txStatus;
	IS_TRANSMITTING = 1; /*Flag to prevent recieving from interupting*/
	pkg[0] = '!'; /*Build Package with leading 3 '!' and 3 terminating '$'*/
	pkg[1] = '!';
	pkg[2] = '!';
	convertFloatToBytes(pkg+3, roll);
	convertFloatToBytes(pkg+7, pitch);
	convertFloatToBytes(pkg+11, filtered_temperature);
	pkg[TX_PKG_SIZE-4] = doubleTapFlag;
	pkg[TX_PKG_SIZE-3] = '$';
	pkg[TX_PKG_SIZE-2] = '$';
	pkg[TX_PKG_SIZE-1] = '$';
	
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);/*Send signal on GPIO line to nucleo*/
	txStatus = HAL_SPI_Transmit (&nucleoSPIHandle, pkg, TX_PKG_SIZE,1); /*Transmit Data*/
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);
	IS_TRANSMITTING = 0;
	doubleTapFlag = 0;
	return txStatus;/*HAL_OK indicates sucesful transmision*/
}

/* Recieves data from nucleo. Activated by interrpt callback*/
HAL_StatusTypeDef receive_pkg(void){
	HAL_StatusTypeDef updateStatus;
	int i;
	RXCounter ++;
	
	updateStatus = HAL_SPI_Receive (&nucleoSPIHandle, data, RX_PKG_SIZE,1);
	
	for (i=0;i<3;i++){
		if(data[i] == '!' && data[i+1] == '!' && data[i+2] == '!'){
			intensity = data[i+3];
			speed = data[i+4];
			successfulRX ++;
		}
	}
	return updateStatus;
}
















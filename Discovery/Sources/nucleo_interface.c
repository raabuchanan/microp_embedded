#include "nucleo_interface.h"

/*Nucleo wireing:

SPI2_SCK  -> blue
SPI2_MISO -> red
SPI2_MOSI -> yellow
SPI2_CS   -> orange
*/
#include "util.h"
#include "main.h"

SPI_HandleTypeDef nucleoSPIHandle;
extern float filtered_temperature;
extern float pitch, roll;
uint8_t data[10];
uint8_t pkg[PKG_SIZE]; // = "abcdefghijklmnopqr";
int IS_TRANSMITTING = 0;


void nucleo_SPI_init(void)
{
  nucleoSPIHandle.Instance = SPI2;
  nucleoSPIHandle.Init.Mode = SPI_MODE_SLAVE;
  nucleoSPIHandle.Init.Direction = SPI_DIRECTION_2LINES;
  nucleoSPIHandle.Init.DataSize = SPI_DATASIZE_8BIT;
  nucleoSPIHandle.Init.CLKPolarity = SPI_POLARITY_LOW;
  nucleoSPIHandle.Init.CLKPhase = SPI_PHASE_1EDGE;
  nucleoSPIHandle.Init.NSS = SPI_NSS_SOFT;
  nucleoSPIHandle.Init.FirstBit = SPI_FIRSTBIT_MSB;
  nucleoSPIHandle.Init.TIMode = SPI_TIMODE_DISABLED;
  nucleoSPIHandle.Init.CRCPolynomial = 7;
  nucleoSPIHandle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256; //too fast when we used 8. TODO: further testing
  nucleoSPIHandle.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  
  HAL_SPI_Init(&nucleoSPIHandle);
	
//#ifdef OPTIMIZED_SPI /* used by the server (L0 and F4, not L4) for the throughput test */
//  /* Added HAP to enable SPI since Optimized SPI Transmit, Receive and Transmit/Receive APIs are 
//     used for BlueNRG, BlueNRG-MS SPI communication in order to get the best performance in terms of 
//     BLE throughput */
//  __HAL_SPI_ENABLE(&nucleoSPIHandle);
//#endif
}


/*Collects Data and formats into package then sends to nucleo*/
HAL_StatusTypeDef send_pkg(uint32_t timeOut){

	HAL_StatusTypeDef txStatus;
	
	/*Build Package with leading '!' and terminating '$'*/
	pkg[0] = '!';
	pkg[1] = '!';
	pkg[2] = '!';
	convertFloatToBytes(pkg+3, roll);
	convertFloatToBytes(pkg+7, pitch);
	convertFloatToBytes(pkg+11, filtered_temperature);
	pkg[PKG_SIZE-3] = '$';
	pkg[PKG_SIZE-2] = '$';
	pkg[PKG_SIZE-1] = '$';
	
	IS_TRANSMITTING = 1;
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);
	txStatus = HAL_SPI_Transmit (&nucleoSPIHandle, pkg, PKG_SIZE,1);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);
	IS_TRANSMITTING = 0;
	
	return txStatus;
}


/*Polls for Data from Nucleo*/
HAL_StatusTypeDef receive_pkg(void){
	HAL_StatusTypeDef updateStatus;
	

	updateStatus = HAL_SPI_Receive (&nucleoSPIHandle, data, 10,1);
	
//	for (int i=0;i<25;i++){
//		if(data[i] == '!' && data[i+1] == '!' && data[i+2] == '!'){
//			Acc_Update(data+i+3);
//			Temp_Update(data+i+11);
//		}
//}
	return updateStatus;
}
















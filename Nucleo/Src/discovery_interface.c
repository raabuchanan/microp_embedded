#include "discovery_interface.h"
#include "main.h"
#include "sensor_service.h"

/*Nucleo wireing:

SPI2_SCK  -> blue
SPI2_MISO -> red
SPI2_MOSI -> yellow
SPI2_CS   -> orange

*/
SPI_HandleTypeDef discoverySPIHandle;
void discovery_SPI_init(void)
{
  discoverySPIHandle.Instance = SPI2;
  discoverySPIHandle.Init.Mode = SPI_MODE_SLAVE;
  discoverySPIHandle.Init.Direction = SPI_DIRECTION_2LINES;
  discoverySPIHandle.Init.DataSize = SPI_DATASIZE_8BIT;
  discoverySPIHandle.Init.CLKPolarity = SPI_POLARITY_LOW;
  discoverySPIHandle.Init.CLKPhase = SPI_PHASE_1EDGE;
  discoverySPIHandle.Init.NSS = SPI_NSS_SOFT;
  discoverySPIHandle.Init.FirstBit = SPI_FIRSTBIT_MSB;
  discoverySPIHandle.Init.TIMode = SPI_TIMODE_DISABLED;
  discoverySPIHandle.Init.CRCPolynomial = 7;
  discoverySPIHandle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32; // shouldn't matter. Depends on master's clk rate
  discoverySPIHandle.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  
  HAL_SPI_Init(&discoverySPIHandle);
	
#ifdef OPTIMIZED_SPI /* used by the server (L0 and F4, not L4) for the throughput test */
  /* Added HAP to enable SPI since Optimized SPI Transmit, Receive and Transmit/Receive APIs are 
     used for BlueNRG, BlueNRG-MS SPI communication in order to get the best performance in terms of 
     BLE throughput */
  __HAL_SPI_ENABLE(&discoverySPIHandle);
#endif
}


/*Polls for Data from Discovery*/
HAL_StatusTypeDef update_phone(uint32_t timeOut){
	HAL_StatusTypeDef updateStatus;
	uint8_t data[PKG_SIZE];

	updateStatus = HAL_SPI_Receive (&discoverySPIHandle, data, PKG_SIZE, 10);
	
	for (int i=0;i<3;i++){
		if(data[i] == '!' && data[i+1] == '!' && data[i+2] == '!'){
			Acc_Update(data+i+3);
			Temp_Update(data+i+11);
		}
}
	return updateStatus;
}

///*Polls for Data from Discovery*/
//HAL_StatusTypeDef update_discovery(uint8_t* data){
//	uint8_t pkg[10];
//	HAL_StatusTypeDef txStatus;
//	
//	/*Build Package with leading '!' and terminating '$'*/
//	pkg[0] = '!';
//	pkg[1] = '!';
//	pkg[2] = '!';
//	pkg[3] = data[0];
//	pkg[4] = data[1];
//	pkg[5] = data[2];
//	pkg[6] = data[3];
//	pkg[7] = '$';
//	pkg[8] = '$';
//	pkg[9] = '$';
//	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET);
//	txStatus = HAL_SPI_Transmit (&discoverySPIHandle, pkg, 10, 10);
//	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_RESET);
//	return txStatus;
//}
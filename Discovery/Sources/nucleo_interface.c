#include "nucleo_interface.h"

/*Nucleo wireing:

SPI2_SCK  -> blue
SPI2_MISO -> red
SPI2_MOSI -> yellow
SPI2_CS   -> orange

*/
SPI_HandleTypeDef nucleoSPIHandle;

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
  nucleoSPIHandle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
  nucleoSPIHandle.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  
  HAL_SPI_Init(&nucleoSPIHandle);
	
#ifdef OPTIMIZED_SPI /* used by the server (L0 and F4, not L4) for the throughput test */
  /* Added HAP to enable SPI since Optimized SPI Transmit, Receive and Transmit/Receive APIs are 
     used for BlueNRG, BlueNRG-MS SPI communication in order to get the best performance in terms of 
     BLE throughput */
  __HAL_SPI_ENABLE(&nucleoSPIHandle);
#endif
}
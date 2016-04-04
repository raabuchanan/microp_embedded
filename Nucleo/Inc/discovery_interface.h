
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef DISCOVERY_INTERFACE_H
#define DISCOVERY_INTERFACE_H


/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"


SPI_HandleTypeDef discoverySPIHandle;

void discovery_SPI_init(void);
	 
#endif


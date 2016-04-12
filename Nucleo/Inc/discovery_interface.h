
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef DISCOVERY_INTERFACE_H
#define DISCOVERY_INTERFACE_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

void discovery_SPI_init(void);
HAL_StatusTypeDef update_phone(uint32_t timeOut);
	 
#endif


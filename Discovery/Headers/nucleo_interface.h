
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef DISCOVERY_INTERFACE_H
#define DISCOVERY_INTERFACE_H



/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
void nucleo_SPI_init(void);
HAL_StatusTypeDef send_pkg(void);
HAL_StatusTypeDef receive_pkg(void);

#endif


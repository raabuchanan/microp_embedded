/**
  ******************************************************************************
  * @file    sensor_service.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    04-July-2014
  * @brief   Add a sample service using a vendor specific profile.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2014 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
#include "sensor_service.h"

/** @addtogroup X-CUBE-BLE1_Applications
 *  @{
 */

/** @addtogroup SensorDemo
 *  @{
 */
 
/** @defgroup SENSOR_SERVICE
 * @{
 */

/** @defgroup SENSOR_SERVICE_Private_Variables
 * @{
 */
/* Private variables ---------------------------------------------------------*/
volatile int connected = FALSE;
volatile uint8_t set_connectable = 1;
volatile uint16_t connection_handle = 0;
volatile uint8_t notification_enabled = FALSE;
uint16_t accServHandle, rollCharHandle, pitchCharHandle;
uint16_t tempServHandle, tempCharHandle;
uint16_t ledServHandle, ledSpeedCharHandle, ledIntensityCharHandle;
uint16_t doubleTapServHandle, doubleTapCharHandle;
uint8_t ledState = 0;
extern uint8_t bnrg_expansion_board;

/**
 * @}
 */

/** @defgroup SENSOR_SERVICE_Private_Macros
 * @{
 */
/* Private macros ------------------------------------------------------------*/
#define COPY_UUID_128(uuid_struct, uuid_15, uuid_14, uuid_13, uuid_12, uuid_11, uuid_10, uuid_9, uuid_8, uuid_7, uuid_6, uuid_5, uuid_4, uuid_3, uuid_2, uuid_1, uuid_0) \
do {\
    uuid_struct[0] = uuid_0; uuid_struct[1] = uuid_1; uuid_struct[2] = uuid_2; uuid_struct[3] = uuid_3; \
        uuid_struct[4] = uuid_4; uuid_struct[5] = uuid_5; uuid_struct[6] = uuid_6; uuid_struct[7] = uuid_7; \
            uuid_struct[8] = uuid_8; uuid_struct[9] = uuid_9; uuid_struct[10] = uuid_10; uuid_struct[11] = uuid_11; \
                uuid_struct[12] = uuid_12; uuid_struct[13] = uuid_13; uuid_struct[14] = uuid_14; uuid_struct[15] = uuid_15; \
}while(0)


#define COPY_ACC_SERVICE_UUID(uuid_struct)  	COPY_UUID_128(uuid_struct,0x00,0x36,0x6e,0x80, 0xcf,0x3a, 0x11,0xe1, 0x9a,0xb4, 0x00,0x02,0xa5,0xd5,0xc5,0x1b)
#define COPY_ROLL_CHAR_UUID(uuid_struct)    	COPY_UUID_128(uuid_struct,0x01,0x36,0x6e,0x80, 0xcf,0x3a, 0x11,0xe1, 0x9a,0xb4, 0x00,0x02,0xa5,0xd5,0xc5,0x1b)
#define COPY_PITCH_CHAR_UUID(uuid_struct)     COPY_UUID_128(uuid_struct,0x02,0x36,0x6e,0x80, 0xcf,0x3a, 0x11,0xe1, 0x9a,0xb4, 0x00,0x02,0xa5,0xd5,0xc5,0x1b)

#define COPY_TEMP_SERVICE_UUID(uuid_struct)  	COPY_UUID_128(uuid_struct,0x03,0x36,0x6e,0x80, 0xcf,0x3a, 0x11,0xe1, 0x9a,0xb4, 0x00,0x02,0xa5,0xd5,0xc5,0x1b)
#define COPY_TEMP_CHAR_UUID(uuid_struct)      COPY_UUID_128(uuid_struct,0x04,0x36,0x6e,0x80, 0xcf,0x3a, 0x11,0xe1, 0x9a,0xb4, 0x00,0x02,0xa5,0xd5,0xc5,0x1b)

#define COPY_DOUBLETAP_SERVICE_UUID(uuid_struct) COPY_UUID_128(uuid_struct,0x05,0x36,0x6e,0x80, 0xcf,0x3a, 0x11,0xe1, 0x9a,0xb4, 0x00,0x02,0xa5,0xd5,0xc5,0x1b)
#define COPY_DOUBLETAP_CHAR_UUID(uuid_struct)    COPY_UUID_128(uuid_struct,0x06,0x36,0x6e,0x80, 0xcf,0x3a, 0x11,0xe1, 0x9a,0xb4, 0x00,0x02,0xa5,0xd5,0xc5,0x1b)

#define COPY_LED_SERVICE_UUID(uuid_struct)  			COPY_UUID_128(uuid_struct,0x07,0x36,0x6e,0x80, 0xcf,0x3a, 0x11,0xe1, 0x9a,0xb4, 0x00,0x02,0xa5,0xd5,0xc5,0x1b)
#define COPY_LED_SPEED_CHAR_UUID(uuid_struct)      		COPY_UUID_128(uuid_struct,0x08,0x36,0x6e,0x80, 0xcf,0x3a, 0x11,0xe1, 0x9a,0xb4, 0x00,0x02,0xa5,0xd5,0xc5,0x1b)
#define COPY_LED_INTENSITY_CHAR_UUID(uuid_struct)  COPY_UUID_128(uuid_struct,0x09,0x36,0x6e,0x80, 0xcf,0x3a, 0x11,0xe1, 0x9a,0xb4, 0x00,0x02,0xa5,0xd5,0xc5,0x1b)

/* Store Value into a buffer in Little Endian Format */
#define STORE_LE_16(buf, val)    ( ((buf)[0] =  (uint8_t) (val)    ) , \
                                   ((buf)[1] =  (uint8_t) (val>>8) ) )

#define STORE_LE_32(buf, val)    ( ((buf)[0] =  (uint8_t) (val)    ) , \
                                   ((buf)[1] =  (uint8_t) (val>>8) ) , \
																	 ((buf)[2] =  (uint8_t) (val>>16) ) , \
																	 ((buf)[3] =  (uint8_t) (val>>24) ) )


/**
 * @}
 */

/** @defgroup SENSOR_SERVICE_Exported_Functions 
 * @{
 */ 
/**
 * @brief  Add an accelerometer service using a vendor specific profile.
 *
 * @param  None
 * @retval tBleStatus Status
 */
tBleStatus Add_Acc_Service(void)
{
  tBleStatus ret;

  uint8_t uuid[16];
  
  COPY_ACC_SERVICE_UUID(uuid);
  ret = aci_gatt_add_serv(UUID_TYPE_128,  uuid, PRIMARY_SERVICE, 8,
                          &accServHandle);
  if (ret != BLE_STATUS_SUCCESS) goto fail;    
  
  COPY_ROLL_CHAR_UUID(uuid);
  ret =  aci_gatt_add_char(accServHandle, UUID_TYPE_128, uuid, 4,
                           CHAR_PROP_READ,
                           ATTR_PERMISSION_NONE,
                           GATT_DONT_NOTIFY_EVENTS,
                           16, 0, &rollCharHandle);
  if (ret != BLE_STATUS_SUCCESS) goto fail;
  
  COPY_PITCH_CHAR_UUID(uuid);  
  ret =  aci_gatt_add_char(accServHandle, UUID_TYPE_128, uuid, 4,
                           CHAR_PROP_READ,
                           ATTR_PERMISSION_NONE,
                           GATT_DONT_NOTIFY_EVENTS,
                           16, 0, &pitchCharHandle);
  if (ret != BLE_STATUS_SUCCESS) goto fail;
  
  PRINTF("Service ACC added. Handle 0x%04X, Roll Charac handle: 0x%04X, Pitch Charac handle: 0x%04X\n", accServHandle, rollCharHandle, pitchCharHandle);	
  return BLE_STATUS_SUCCESS; 
  
fail:
  PRINTF("Error while adding ACC service.\n");
  return BLE_STATUS_ERROR ;
    
}

/**
 * @brief  Update acceleration characteristic value.
 *
 * @param  Structure containing angle values in degrees
 * @retval Status
 */
tBleStatus Acc_Update(uint8_t data[])
{  
  tBleStatus ret;    

  ret = aci_gatt_update_char_value(accServHandle, rollCharHandle, 0, 4, data);
	ret = aci_gatt_update_char_value(accServHandle, pitchCharHandle, 0, 4, data+4);
	
  if (ret != BLE_STATUS_SUCCESS){
    PRINTF("Error while updating ACC characteristic.\n") ;
    return BLE_STATUS_ERROR ;
  }
  return BLE_STATUS_SUCCESS;	
}

/**
 * @brief  Add the Temperature service.
 *
 * @param  None
 * @retval Status
 */
tBleStatus Add_Temperature_Service(void)
{
  tBleStatus ret;
  uint8_t uuid[16];
  uint16_t uuid16;
  charactFormat charFormat;
  uint16_t descHandle;
  
  COPY_TEMP_SERVICE_UUID(uuid);
  ret = aci_gatt_add_serv(UUID_TYPE_128,  uuid, PRIMARY_SERVICE, 4,
                          &tempServHandle);
  if (ret != BLE_STATUS_SUCCESS) goto fail;
  
  /* Temperature Characteristic */
  COPY_TEMP_CHAR_UUID(uuid);  
  ret =  aci_gatt_add_char(tempServHandle, UUID_TYPE_128, uuid, 4,
                           CHAR_PROP_READ, ATTR_PERMISSION_NONE,
                           GATT_DONT_NOTIFY_EVENTS,
                           16, 0, &tempCharHandle);
  if (ret != BLE_STATUS_SUCCESS) goto fail;
  
  charFormat.format = FORMAT_SINT16;
  charFormat.exp = -1;
  charFormat.unit = UNIT_TEMP_CELSIUS;
  charFormat.name_space = 0;
  charFormat.desc = 0;
  
  uuid16 = CHAR_FORMAT_DESC_UUID;
  
  ret = aci_gatt_add_char_desc(tempServHandle,
                               tempCharHandle,
                               UUID_TYPE_16,
                               (uint8_t *)&uuid16, 
                               7,
                               7,
                               (void *)&charFormat, 
                               ATTR_PERMISSION_NONE,
                               ATTR_ACCESS_READ_ONLY,
                               0,
                               16,
                               FALSE,
                               &descHandle);
  if (ret != BLE_STATUS_SUCCESS) goto fail;
  
  PRINTF("Service TEMP added. Handle 0x%04X, TEMP Charac handle: 0x%04X\n",tempServHandle, tempCharHandle);	
  return BLE_STATUS_SUCCESS; 
  
fail:
  PRINTF("Error while adding TEMP service.\n");
  return BLE_STATUS_ERROR ;
  
}

/**
 * @brief  Update temperature characteristic value.
 * @param  Temperature in degrees 
 * @retval Status
 */
tBleStatus Temp_Update(uint8_t temp_data[])
{  
  tBleStatus ret;

  ret = aci_gatt_update_char_value(tempServHandle, tempCharHandle, 0, 4,
                                   temp_data);
  
  if (ret != BLE_STATUS_SUCCESS){
    PRINTF("Error while updating TEMP characteristic.\n") ;
    return BLE_STATUS_ERROR ;
  }
  return BLE_STATUS_SUCCESS;
	
}


/**
 * @brief  Add LED button service using a vendor specific profile.
 * @param  None
 * @retval Status
 */
tBleStatus Add_LED_Service(void)
{
  tBleStatus ret;
  uint8_t uuid[16];
  
  /* copy "LED service UUID" defined above to 'uuid' local variable */
  COPY_LED_SERVICE_UUID(uuid);
  /* 
   * now add "LED service" to GATT server, service handle is returned
   * via 'ledServHandle' parameter of aci_gatt_add_serv() API. 
  */  
  ret = aci_gatt_add_serv(UUID_TYPE_128, uuid, PRIMARY_SERVICE, 8,
                          &ledServHandle);
  if (ret != BLE_STATUS_SUCCESS) goto fail;    
  
  /* copy "LED speed characteristic UUID" defined above to 'uuid' local variable */  
  COPY_LED_SPEED_CHAR_UUID(uuid);
  /* 
   * now add "LED speed characteristic" to LED service, characteristic handle 
   * is returned via 'ledSpeedCharHandle' parameter of aci_gatt_add_char() API.
   * This characteristic is writable, as specified by 'CHAR_PROP_WRITE' parameter.
  */   
  ret =  aci_gatt_add_char(ledServHandle, UUID_TYPE_128, uuid, 4,
                           CHAR_PROP_WRITE | CHAR_PROP_WRITE_WITHOUT_RESP, ATTR_PERMISSION_NONE, GATT_NOTIFY_ATTRIBUTE_WRITE,
                           16, 1, &ledSpeedCharHandle);
  if (ret != BLE_STATUS_SUCCESS) goto fail;  

  /* copy "LED intensity characteristic UUID" defined above to 'uuid' local variable */  
  COPY_LED_INTENSITY_CHAR_UUID(uuid);
  /* 
   * now add "LED intensity characteristic" to LED service, characteristic handle 
   * is returned via 'ledIntensityCharHandle' parameter of aci_gatt_add_char() API.
   * This characteristic is writable, as specified by 'CHAR_PROP_WRITE' parameter.
  */   
  ret =  aci_gatt_add_char(ledServHandle, UUID_TYPE_128, uuid, 4,
                           CHAR_PROP_WRITE | CHAR_PROP_WRITE_WITHOUT_RESP, ATTR_PERMISSION_NONE, GATT_NOTIFY_ATTRIBUTE_WRITE,
                           16, 1, &ledIntensityCharHandle);
  if (ret != BLE_STATUS_SUCCESS) goto fail;  


	PRINTF("Service LED added. Handle 0x%04X, LED speed Charac handle: 0x%04X, LED intensity Charac handle: 0x%04X\n", ledServHandle, ledSpeedCharHandle, ledIntensityCharHandle);
  return BLE_STATUS_SUCCESS; 
  
fail:
  PRINTF("Error while adding LED service.\n");
  return BLE_STATUS_ERROR;
}

/**
 * @brief  Add the double-tap service using a vendor specific profile.
 *
 * @param  None
 * @retval tBleStatus Status
 */
tBleStatus Add_Double_Tap_Service(void)
{
  tBleStatus ret;

  uint8_t uuid[16];
  
  COPY_DOUBLETAP_SERVICE_UUID(uuid);
  ret = aci_gatt_add_serv(UUID_TYPE_128,  uuid, PRIMARY_SERVICE, 4,
                          &doubleTapServHandle);
  if (ret != BLE_STATUS_SUCCESS) goto fail;    
  
  COPY_DOUBLETAP_CHAR_UUID(uuid);
  ret =  aci_gatt_add_char(doubleTapServHandle, UUID_TYPE_128, uuid, 4,
                           CHAR_PROP_NOTIFY, ATTR_PERMISSION_NONE, 0,
                           16, 0, &doubleTapCharHandle);
  if (ret != BLE_STATUS_SUCCESS) goto fail;
  
  PRINTF("Service DT added. Handle 0x%04X, Double Charac handle: 0x%04X\n", doubleTapServHandle, doubleTapCharHandle);	
  return BLE_STATUS_SUCCESS; 
  
fail:
  PRINTF("Error while adding DT service.\n");
  return BLE_STATUS_ERROR ;
    
}

/**
 * @brief  Send a notification for a Double Tap event.
 *
 * @param  None
 * @retval tBleStatus Status
 */
tBleStatus Double_Tap_Notify(void)
{  
  uint8_t val;
  tBleStatus ret;
	
  val = 0x01;	
  ret = aci_gatt_update_char_value(doubleTapServHandle, doubleTapCharHandle, 0, 1,
                                   &val);
	
  if (ret != BLE_STATUS_SUCCESS){
    PRINTF("Error while updating DT characteristic.\n") ;
    return BLE_STATUS_ERROR ;
  }
  return BLE_STATUS_SUCCESS;	
}


/**
 * @brief  This function is called attribute value corresponding to 
 *         ledButtonCharHandle characteristic gets modified.
 * @param  Handle of the attribute
 * @param  Size of the modified attribute data
 * @param  Pointer to the modified attribute data
 * @retval None
 */

uint8_t LEDIntensity;
uint8_t LEDSpeed;

void Attribute_Modified_CB(uint16_t handle, uint8_t data_length, uint8_t *att_data)
{
  if(handle == ledSpeedCharHandle + 1){
		LEDSpeed = *(att_data);
		PRINTF("Got LED speed data0: %x\n", *att_data);
  } else if(handle == ledIntensityCharHandle + 1){
		PRINTF("Got LED intensity data0: %x\n", *att_data);
		LEDIntensity = *(att_data);
  }
}

/**
 * @brief  Puts the device in connectable mode.
 *         If you want to specify a UUID list in the advertising data, those data can
 *         be specified as a parameter in aci_gap_set_discoverable().
 *         For manufacture data, aci_gap_update_adv_data must be called.
 * @param  None 
 * @retval None
 */
/* Ex.:
 *
 *  tBleStatus ret;    
 *  const char local_name[] = {AD_TYPE_COMPLETE_LOCAL_NAME,'B','l','u','e','N','R','G'};    
 *  const uint8_t serviceUUIDList[] = {AD_TYPE_16_BIT_SERV_UUID,0x34,0x12};    
 *  const uint8_t manuf_data[] = {4, AD_TYPE_MANUFACTURER_SPECIFIC_DATA, 0x05, 0x02, 0x01};
 *  
 *  ret = aci_gap_set_discoverable(ADV_IND, 0, 0, PUBLIC_ADDR, NO_WHITE_LIST_USE,
 *                                 8, local_name, 3, serviceUUIDList, 0, 0);    
 *  ret = aci_gap_update_adv_data(5, manuf_data);
 *
 */
void setConnectable(void)
{  
  tBleStatus ret;
  
  const char local_name[] = {AD_TYPE_COMPLETE_LOCAL_NAME,'B','L','E','-','G','0','9'};
  
  /* disable scan response */
  hci_le_set_scan_resp_data(0,NULL);
  PRINTF("General Discoverable Mode.\n");
  
  ret = aci_gap_set_discoverable(ADV_IND, 0, 0, PUBLIC_ADDR, NO_WHITE_LIST_USE,
                                 sizeof(local_name), local_name, 0, NULL, 0, 0);
  if (ret != BLE_STATUS_SUCCESS) {
    PRINTF("Error while setting discoverable mode (%d)\n", ret);    
  }  
}

/**
 * @brief  This function is called when there is a LE Connection Complete event.
 * @param  uint8_t Address of peer device
 * @param  uint16_t Connection handle
 * @retval None
 */
void GAP_ConnectionComplete_CB(uint8_t addr[6], uint16_t handle)
{  
  connected = TRUE;
  connection_handle = handle;
  
  PRINTF("Connected to device:");
  for(int i = 5; i > 0; i--){
    PRINTF("%02X-", addr[i]);
  }
  PRINTF("%02X\n", addr[0]);
}

/**
 * @brief  This function is called when the peer device gets disconnected.
 * @param  None 
 * @retval None
 */
void GAP_DisconnectionComplete_CB(void)
{
  connected = FALSE;
  PRINTF("Disconnected\n");
  /* Make the device connectable again. */
  set_connectable = TRUE;
  notification_enabled = FALSE;
}

/**
 * @brief  Read request callback.
 * @param  uint16_t Handle of the attribute
 * @retval None
 */
void Read_Request_CB(uint16_t handle)
{  
  //EXIT:
  if(connection_handle != 0)
    aci_gatt_allow_read(connection_handle);
}

/**
 * @brief  Callback processing the ACI events.
 * @note   Inside this function each event must be identified and correctly
 *         parsed.
 * @param  void* Pointer to the ACI packet
 * @retval None
 */
void HCI_Event_CB(void *pckt)
{
  hci_uart_pckt *hci_pckt = pckt;
  /* obtain event packet */
  hci_event_pckt *event_pckt = (hci_event_pckt*)hci_pckt->data;
  
  if(hci_pckt->type != HCI_EVENT_PKT)
    return;
  
  switch(event_pckt->evt){
    
  case EVT_DISCONN_COMPLETE:
    {
      GAP_DisconnectionComplete_CB();
    }
    break;
    
  case EVT_LE_META_EVENT:
    {
      evt_le_meta_event *evt = (void *)event_pckt->data;
      
      switch(evt->subevent){
      case EVT_LE_CONN_COMPLETE:
        {
          evt_le_connection_complete *cc = (void *)evt->data;
          GAP_ConnectionComplete_CB(cc->peer_bdaddr, cc->handle);
        }
        break;
      }
    }
    break;
    
  case EVT_VENDOR:
    {
      evt_blue_aci *blue_evt = (void*)event_pckt->data;
      switch(blue_evt->ecode){
				case EVT_BLUE_GATT_ATTRIBUTE_MODIFIED:         
					{
						/* this callback is invoked when a GATT attribute is modified
						extract callback data and pass to suitable handler function */
						if (bnrg_expansion_board == IDB05A1) {
							evt_gatt_attr_modified_IDB05A1 *evt = (evt_gatt_attr_modified_IDB05A1*)blue_evt->data;
							Attribute_Modified_CB(evt->attr_handle, evt->data_length, evt->att_data); 
						}
						else {
							evt_gatt_attr_modified_IDB04A1 *evt = (evt_gatt_attr_modified_IDB04A1*)blue_evt->data;
							Attribute_Modified_CB(evt->attr_handle, evt->data_length, evt->att_data); 
						}                       
					}
					break; 


				case EVT_BLUE_GATT_READ_PERMIT_REQ:
					{
						evt_gatt_read_permit_req *pr = (void*)blue_evt->data;                    
						Read_Request_CB(pr->attr_handle);                    
					}
					break;
				}
    }
    break;
  }    
}
/**
 * @}
 */
 
/**
 * @}
 */

/**
 * @}
 */

 /**
 * @}
 */
 
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

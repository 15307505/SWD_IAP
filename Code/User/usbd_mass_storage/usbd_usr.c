/**
  ******************************************************************************
  * @file    usbd_usr.c
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    19-March-2012
  * @brief   This file includes the user application layer
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "usbd_usr.h"
#include <stdio.h>
#include "usbd_msc_core.h"
#include "usbd_desc.h"

#define usb_printf	printf
//#define usb_printf(...)

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) /*!< IAR Compiler */
    #pragma data_alignment=4
  #endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */

__ALIGN_BEGIN USB_OTG_CORE_HANDLE     USB_OTG_dev __ALIGN_END ;

// 使能U盘设备 
void usbd_OpenMassStorage(void) 
{
	USBD_Init(&USB_OTG_dev,
			USB_OTG_FS_CORE_ID,
			&USR_desc,
			&USBD_MSC_cb,
			&USRD_cb);
}

// 移除U盘设备 
void usbd_CloseMassStorage(void) 
{
	DCD_DevDisconnect(&USB_OTG_dev);
}

/** @defgroup USBD_USR_Private_Variables
  * @{
  */
/*  Points to the DEVICE_PROP structure of current device */
/*  The purpose of this register is to speed up the execution */

USBD_Usr_cb_TypeDef USRD_cb =
{
  USBD_USR_Init,
  USBD_USR_DeviceReset,
  USBD_USR_DeviceConfigured,
  USBD_USR_DeviceSuspended,
  USBD_USR_DeviceResumed,

  USBD_USR_DeviceConnected,
  USBD_USR_DeviceDisconnected,
};

/**
  * @}
  */

/** @defgroup USBD_USR_Private_Constants
  * @{
  */

#define USER_INFORMATION1  "INFO : Single Lun configuration"
#define USER_INFORMATION2  "INFO : microSD is used"
/**
  * @}
  */



/** @defgroup USBD_USR_Private_FunctionPrototypes
  * @{
  */
/**
  * @}
  */


/** @defgroup USBD_USR_Private_Functions
  * @{
  */

/**
* @brief  Displays the message on LCD on device lib initialization
* @param  None
* @retval None
*/
void USBD_USR_Init(void)
{
	usb_printf("> USBD_USR_Init \r\n" );
}

/**
* @brief  Displays the message on LCD on device reset event
* @param  speed : device speed
* @retval None
*/
void USBD_USR_DeviceReset (uint8_t speed)
{
	/* 在USB未连接状态，这个函数每隔20ms进来一次 */
 switch (speed)
 {
   case USB_OTG_SPEED_HIGH:
     usb_printf("     USBD_USR_DeviceReset  [HS]\r\n" );
     break;

  case USB_OTG_SPEED_FULL:
     usb_printf("     USBD_USR_DeviceReset [FS]\r\n" );
     break;
 default:
     usb_printf("     USB Device Library v1.1.0  [??]\r\n" );
	 break;

 }
}


/**
* @brief  Displays the message on LCD on device config event
* @param  None
* @retval Staus
*/
void USBD_USR_DeviceConfigured (void)
{
	usb_printf("> MSC Interface started.\r\n");

}
/**
* @brief  Displays the message on LCD on device suspend event
* @param  None
* @retval None
*/
void USBD_USR_DeviceSuspended(void)
{
	usb_printf("> Device In suspend mode.\r\n");
}


/**
* @brief  Displays the message on LCD on device resume event
* @param  None
* @retval None
*/
void USBD_USR_DeviceResumed(void)
{
	usb_printf("> Device Resumed.\r\n");
}

/**
* @brief  USBD_USR_DeviceConnected
*         Displays the message on LCD on device connection Event
* @param  None
* @retval Staus
*/
void USBD_USR_DeviceConnected (void)
{
	usb_printf("> USB Device Connected.\r\n");
	//g_ucCommRun = 1;
}


/**
* @brief  USBD_USR_DeviceDisonnected
*         Displays the message on LCD on device disconnection Event
* @param  None
* @retval Staus
*/
void USBD_USR_DeviceDisconnected (void)
{
	usb_printf("> USB Device Disconnected.\r\n");
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

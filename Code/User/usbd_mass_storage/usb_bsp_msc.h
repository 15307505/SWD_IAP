/*
*********************************************************************************************************
*
*	模块名称 : USB设备驱动模块,大容量存储设备
*	文件名称 : usb_bsp_msc.h
*	版    本 : V1.0
*	说    明 : 这是USB Device模块h文件。应用程序包含这一个h文件即可。
*
*********************************************************************************************************
*/

#ifndef _USB_BSP_USR_H
#define _USB_BSP_USR_H

#include "usbd_msc_core.h"
#include "usbd_usr.h"
#include "usbd_desc.h"
#include "usb_conf.h"
#include "usb_bsp.h"

void usbd_OpenMassStorage(void);
void usbd_CloseMassStorage(void);

extern USB_OTG_CORE_HANDLE     USB_OTG_dev;

#endif


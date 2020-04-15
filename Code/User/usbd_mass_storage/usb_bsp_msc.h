/*
*********************************************************************************************************
*
*	ģ������ : USB�豸����ģ��,�������洢�豸
*	�ļ����� : usb_bsp_msc.h
*	��    �� : V1.0
*	˵    �� : ����USB Deviceģ��h�ļ���Ӧ�ó��������һ��h�ļ����ɡ�
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


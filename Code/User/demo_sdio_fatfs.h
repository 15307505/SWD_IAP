/*
*********************************************************************************************************
*
*	ģ������ : SD��FatFS ��ʾģ�顣
*	�ļ����� : demo_sdio_fatfs.h
*	��    �� : V1.0
*	˵    �� : ͷ�ļ�
*
*********************************************************************************************************
*/

#ifndef _DEMO_SDIO_FATFS_H
#define _DEMO_SDIO_FATFS_H

/* ���ⲿ���õĺ������� */
void DemoFatFS(void);
void show_sdcard_info(void);
void DispMenu(void);
void ViewRootDir(void);
void CreateNewFile(void);
void ReadFileData(void);
void CreateDir(void);
void DeleteDirFile(void);
void WriteFileTest(void);

#endif



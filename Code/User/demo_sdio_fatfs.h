/*
*********************************************************************************************************
*
*	模块名称 : SD卡FatFS 演示模块。
*	文件名称 : demo_sdio_fatfs.h
*	版    本 : V1.0
*	说    明 : 头文件
*
*********************************************************************************************************
*/

#ifndef _DEMO_SDIO_FATFS_H
#define _DEMO_SDIO_FATFS_H

/* 供外部调用的函数声明 */
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



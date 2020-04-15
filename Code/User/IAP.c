#include "bsp.h"

#define ERR_MAX 99

_IAP_t IAP_t;  //定义一个结构体	
unsigned char IAPState=0;
unsigned int  sector;

/* 本函数使用的变量占用较多，请修改启动文件，保证堆栈空间够用 */
FRESULT result;
FATFS fs;
FIL file;
DIR DirInf;
FILINFO FileInf;
unsigned int bw;	

extern u8 Maintask_Status;

void OpenFile(void)
{
//	/* 本函数使用的局部变量占用较多，请修改启动文件，保证堆栈空间够用 */
//	FRESULT result;
//	FATFS fs;
//	FIL file;
//	DIR DirInf;
//  FILINFO FileInf;
  u8 pBuf[6];
 	/* 挂载文件系统 */
	result = f_mount(FS_SD, &fs);			/* Mount a logical drive */
	if (result != FR_OK)
	{
		printf("挂载文件系统失败(%d)\r\n", result);
	}

	/* 打开根文件夹 */
	result = f_opendir(&DirInf, "/"); /* 如果不带参数，则从当前目录开始 */
	if (result != FR_OK)
	{
		printf("打开根目录失败(%d)\r\n", result);
		return;
	}
  /*获取文件信息 */
	result = f_stat("LED0.bin", &FileInf); //读取 folder 目录下 newname.txt 文件的信息

	if( result )
	{
		printf("Read file massage err \r\n");//没读取文件信息成功
	}
	else
	{
		OLED_ClrScr(0);
		OLED_DispStr(2, 2 ,  FileInf.fname , &tFont12); //将需要文件显示屏幕
		
		printf("newname.txt size : %lu\r\n",FileInf.fsize);//读取文件的长度，即占多少字节
		OLED_DispStr(2,20,"文件大小      Byte" ,&tFont12);
		
		pBuf[0] = FileInf.fsize / 10000  	 + 0x30;
		pBuf[1] = FileInf.fsize / 1000 %10 + 0x30;
		pBuf[2] = FileInf.fsize / 100 %10  + 0x30;
		pBuf[3] = FileInf.fsize / 10 %10   + 0x30;
		pBuf[4] = FileInf.fsize  %10       + 0x30;
		pBuf[5] = '\0';
		OLED_DispStr(55,21,(u8 *)pBuf ,&tFont12);
		
		printf("fdate : %d\r\n",FileInf.fdate); //读取文件的最近修改日期 转化为2进制，
		printf("ftime : %d\r\n",FileInf.ftime); //读取文件的最近修改时间 转化为2进制
//    OLED_DispStr(2,34,"修改日期" ,&tFont12);
		
		sector = FileInf.fsize/512;
  bsp_DelayMS(2000);
	}
	/* 打开文件 */
	result = f_open(&file, "LED0.bin", FA_OPEN_EXISTING | FA_READ);
	if (result !=  FR_OK)
	{
		printf("Don't Find File : LED0.bin\r\n");
		return;
	} 
//	      u32 i;
//				f_read(&file, &File_buf, FileInf.fsize, &bw);
//				for(i=0; i< FileInf.fsize; i++)
//				{
//					printf("0x%02X, ",File_buf[i]);
//				}
	
	
//  bsp_DelayMS(2000);
}

void CloseFile(void)
{
	/* 本函数使用的局部变量占用较多，请修改启动文件，保证堆栈空间够用 */
	FIL file;
	
		/* 关闭文件*/
	f_close(&file);

	/* 卸载文件系统 */
	f_mount(FS_SD, NULL);
	
}
void IAPData(void)
{
	u8 pBuf[4];
  u16 temp;

		//开始IAP升级操作
		//第一步  进入boot模式 目前客户BOOT是上电自启动，不需要APP跳转	
		switch(IAPState)
		{
			case 0:  					
						OLED_ClrScr(0);
						OLED_DispStr(2,2,"开始升级" ,&tFont12);	
			      IAP_t.ErrCnt=0;
			      COM1Receive.readIndex = COM1Receive.writeIndex;
			      clearBuff(USART1);
						IAP_t.Current_Sector = 0;   //清除标志位
						IAPState = 1;
						break;

			case 1:  		
						if(COM1Receive.readIndex != COM1Receive.writeIndex) //有数据
						{	
							if(!popBuff(USART1))
							{
								break;
							}	
							temp = CRC16_CCITT(COM1Temp.rbuf , 10);
							if((COM1Temp.rbuf[10] == temp / 256)&&(COM1Temp.rbuf[11] == temp % 256))
								 if( (COM1Temp.rbuf[4] == 0x95) && (COM1Temp.rbuf[5] == 0xff))
								 {
									OLED_DispStr(2,18,"进入boot模式成功" ,&tFont12);
									OLED_DispStr(2,34,"开始擦所有扇区" ,&tFont12);
									clearBuff(USART1);
									IAP_t.ErrCnt=0;
									IAPState = 2;								 
								 }	
						}
						else
						{          
							IAP_SendCMD(0xff);
							IAP_t.Rxd_TimeOut = 1000;
							while(IAP_t.Rxd_TimeOut);
							
							OLED_DispStr(2,18,"进入boot模式中" ,&tFont12);
							OLED_DispStr(2,34,"尝试第  次" ,&tFont12);
							pBuf[0] = IAP_t.ErrCnt / 10 + 0x30;
							pBuf[1] = IAP_t.ErrCnt % 10 + 0x30;
							pBuf[2] = '\0';
							OLED_DispStr(40,35,(u8 *)pBuf ,&tFont12);
							
							IAP_t.ErrCnt++;
							if(IAP_t.ErrCnt > ERR_MAX)
							{
								IAP_t.ErrCnt=0;
								OLED_ClrScr(0);
								OLED_DispStr(2,18,"进入boot模式失败" ,&tFont12);
								
								IAPState = 255;  
								break;
							}
						}				
						break;
						
			case 2:		
						if(COM1Receive.readIndex != COM1Receive.writeIndex) //有数据
						{	
							if(!popBuff(USART1))
							{
								break;
							}	
							temp = CRC16_CCITT(COM1Temp.rbuf , 10);
							if((COM1Temp.rbuf[10] == temp / 256)&&(COM1Temp.rbuf[11] == temp % 256))
								 if( (COM1Temp.rbuf[4] == 0x95) && (COM1Temp.rbuf[5] == 0x01))
								 {
									OLED_DispStr(2,34,"所有扇区擦除成功" ,&tFont12);
									clearBuff(USART1);
									IAP_t.ErrCnt=0;
									IAPState = 3;								
								 }		
						}
						else
						{
											          
							IAP_SendCMD(0x01); 
							IAP_t.Rxd_TimeOut = 4000;		
							OLED_DispStr(2,34,"开始擦所有扇区" ,&tFont12);
							while(IAP_t.Rxd_TimeOut);
							
//							pBuf[0] = IAP_t.ErrCnt / 10 + 0x30;
//							pBuf[1] = IAP_t.ErrCnt % 10 + 0x30;
//							pBuf[2] = '\0';
//							OLED_DispStr(95,35,(u8 *)pBuf ,&tFont12);
							
							IAP_t.ErrCnt++;
							if(IAP_t.ErrCnt > 5)
							{
								IAP_t.ErrCnt=0;
								OLED_ClrScr(0);
								OLED_ClrScr(0);
								OLED_DispStr(2,24,"擦所有扇区失败   " ,&tFont12);
								IAPState = 255;  // 进入休眠
								break;
						  }
						}
						break;				
				
			case 3:
				    if(COM1Receive.readIndex != COM1Receive.writeIndex) //有数据
						{	
							if(!popBuff(USART1))
							{
								break;
							}	
							temp = CRC16_CCITT(COM1Temp.rbuf , 10);
							if((COM1Temp.rbuf[10] == temp / 256)&&(COM1Temp.rbuf[11] == temp % 256))
								 if( (COM1Temp.rbuf[4] == 0x95) && (COM1Temp.rbuf[5] == 0x08))
								 {
									IAP_t.Current_Sector ++;	
									bsp_LedToggle(1); //闪烁LED灯
									if(IAP_t.Current_Sector > sector)
									{
										clearBuff(USART1);
									  IAP_t.ErrCnt=0;
									  IAPState = 4;
										OLED_ClrScr(0);
										OLED_DispStr(2,28,"所有扇区写入成功 " ,&tFont12);
									}
								 }							 
						}
						else
						{
							IAP_SendData(IAP_t.Current_Sector);
							IAP_t.Rxd_TimeOut = 2000;
              while(IAP_t.Rxd_TimeOut); //发送完等待ACK
							
							OLED_DispStr(2,48,"开始写第   个扇区" ,&tFont12);
							pBuf[0] = IAP_t.Current_Sector /100     + 0x30;
							pBuf[1] = IAP_t.Current_Sector /10 %10  + 0x30;
							pBuf[2] = IAP_t.Current_Sector     %10  + 0x30;
							pBuf[3] = '\0';
							OLED_DispStr(53,50,(u8 *)pBuf ,&tFont12);				
						}			
            break;
						
			case 4:
				    IAP_SendCMD(0xfe);
			      bsp_DelayMS(1000);
			      OLED_ClrScr(0);
			      OLED_DispStr(15,15,"升级成功" ,&tFont12);
			      OLED_DispStr(15,35,"跳转用户区" ,&tFont12);
//			      bsp_LedOn(2);
//			      bsp_LedOff(1);
			      IAPState = 255;
				    break;
			
			case 255:			
				    Maintask_Status = State_Init;
			      CloseFile();
			      IAPState = 0;
            break;
			default :
						break;
		}
			bsp_DelayMS(100);

}

void IAP_SendCMD(u8 cmd)
{
	u16 temp;
	u8 IAP_Data[525];
	
	IAP_Data[0] = 0x68;   //S0H
	
	IAP_Data[1] = 0x00;   //LEN
	IAP_Data[2] = 0x06;
	
	IAP_Data[3] = 0x68;   //IOF

	IAP_Data[4] = 0x15;
	
	IAP_Data[5] = cmd;    //SubCMD
	
	IAP_Data[6] = 0;
	IAP_Data[7] = 0;
	IAP_Data[8] = 0;
	IAP_Data[9] = 0;
	
	temp = CRC16_CCITT(IAP_Data , 10);
	IAP_Data[10] = temp / 256;
	IAP_Data[11] = temp % 256;
	
	IAP_Data[12] = 0x16;
	
	COM1_SendData(IAP_Data , 13);
}

void IAP_SendData(u8 Current_Sector)
{
	u16 temp,i,len;
	u8 IAP_Data[530];
	
	/* 读取文件 */
	if(Current_Sector < sector)
	{
		f_read(&file, &File_buf, 512, &bw);
		memcpy(IAP_Data + 10,File_buf,512);	
		IAP_Data[1] = 0x02;   //LEN
	  IAP_Data[2] = 0x06;
		len = 512;
	}
	else
	{
		f_read(&file, &File_buf, 512, &bw);
		memcpy(IAP_Data + 10,File_buf,FileInf.fsize%512);
		IAP_Data[1] = (FileInf.fsize%512 + 6) /256;   //LEN
	  IAP_Data[2] = (FileInf.fsize%512 + 6) %256;
		len = FileInf.fsize % 512;
	}
	
	IAP_Data[0] = 0x68;   //S0H
	IAP_Data[3] = 0x68;   //IOF

	IAP_Data[4] = 0x15;

	IAP_Data[5] = 0x08;    //SubCMD
	
	IAP_Data[6] = ((Current_Sector*512) + 0x4000) >> 24;
	IAP_Data[7] = ((Current_Sector*512) + 0x4000) >> 16;
	IAP_Data[8] = ((Current_Sector*512) + 0x4000) >> 8;
	IAP_Data[9] = ((Current_Sector*512) + 0x4000);
	
	temp = CRC16_CCITT(IAP_Data , len + 10);
	IAP_Data[len + 10] = temp / 256;
	IAP_Data[len + 11] = temp % 256;
  
	IAP_Data[len + 12] = 0x16;
	
	COM1_SendData(IAP_Data , len + 13);
}



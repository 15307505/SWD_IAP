#include "bsp.h"

#define ERR_MAX 99

_IAP_t IAP_t;  //����һ���ṹ��	
unsigned char IAPState=0;
unsigned int  sector;

/* ������ʹ�õı���ռ�ý϶࣬���޸������ļ�����֤��ջ�ռ乻�� */
FRESULT result;
FATFS fs;
FIL file;
DIR DirInf;
FILINFO FileInf;
unsigned int bw;	

extern u8 Maintask_Status;

void OpenFile(void)
{
//	/* ������ʹ�õľֲ�����ռ�ý϶࣬���޸������ļ�����֤��ջ�ռ乻�� */
//	FRESULT result;
//	FATFS fs;
//	FIL file;
//	DIR DirInf;
//  FILINFO FileInf;
  u8 pBuf[6];
 	/* �����ļ�ϵͳ */
	result = f_mount(FS_SD, &fs);			/* Mount a logical drive */
	if (result != FR_OK)
	{
		printf("�����ļ�ϵͳʧ��(%d)\r\n", result);
	}

	/* �򿪸��ļ��� */
	result = f_opendir(&DirInf, "/"); /* ���������������ӵ�ǰĿ¼��ʼ */
	if (result != FR_OK)
	{
		printf("�򿪸�Ŀ¼ʧ��(%d)\r\n", result);
		return;
	}
  /*��ȡ�ļ���Ϣ */
	result = f_stat("LED0.bin", &FileInf); //��ȡ folder Ŀ¼�� newname.txt �ļ�����Ϣ

	if( result )
	{
		printf("Read file massage err \r\n");//û��ȡ�ļ���Ϣ�ɹ�
	}
	else
	{
		OLED_ClrScr(0);
		OLED_DispStr(2, 2 ,  FileInf.fname , &tFont12); //����Ҫ�ļ���ʾ��Ļ
		
		printf("newname.txt size : %lu\r\n",FileInf.fsize);//��ȡ�ļ��ĳ��ȣ���ռ�����ֽ�
		OLED_DispStr(2,20,"�ļ���С      Byte" ,&tFont12);
		
		pBuf[0] = FileInf.fsize / 10000  	 + 0x30;
		pBuf[1] = FileInf.fsize / 1000 %10 + 0x30;
		pBuf[2] = FileInf.fsize / 100 %10  + 0x30;
		pBuf[3] = FileInf.fsize / 10 %10   + 0x30;
		pBuf[4] = FileInf.fsize  %10       + 0x30;
		pBuf[5] = '\0';
		OLED_DispStr(55,21,(u8 *)pBuf ,&tFont12);
		
		printf("fdate : %d\r\n",FileInf.fdate); //��ȡ�ļ�������޸����� ת��Ϊ2���ƣ�
		printf("ftime : %d\r\n",FileInf.ftime); //��ȡ�ļ�������޸�ʱ�� ת��Ϊ2����
//    OLED_DispStr(2,34,"�޸�����" ,&tFont12);
		
		sector = FileInf.fsize/512;
  bsp_DelayMS(2000);
	}
	/* ���ļ� */
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
	/* ������ʹ�õľֲ�����ռ�ý϶࣬���޸������ļ�����֤��ջ�ռ乻�� */
	FIL file;
	
		/* �ر��ļ�*/
	f_close(&file);

	/* ж���ļ�ϵͳ */
	f_mount(FS_SD, NULL);
	
}
void IAPData(void)
{
	u8 pBuf[4];
  u16 temp;

		//��ʼIAP��������
		//��һ��  ����bootģʽ Ŀǰ�ͻ�BOOT���ϵ�������������ҪAPP��ת	
		switch(IAPState)
		{
			case 0:  					
						OLED_ClrScr(0);
						OLED_DispStr(2,2,"��ʼ����" ,&tFont12);	
			      IAP_t.ErrCnt=0;
			      COM1Receive.readIndex = COM1Receive.writeIndex;
			      clearBuff(USART1);
						IAP_t.Current_Sector = 0;   //�����־λ
						IAPState = 1;
						break;

			case 1:  		
						if(COM1Receive.readIndex != COM1Receive.writeIndex) //������
						{	
							if(!popBuff(USART1))
							{
								break;
							}	
							temp = CRC16_CCITT(COM1Temp.rbuf , 10);
							if((COM1Temp.rbuf[10] == temp / 256)&&(COM1Temp.rbuf[11] == temp % 256))
								 if( (COM1Temp.rbuf[4] == 0x95) && (COM1Temp.rbuf[5] == 0xff))
								 {
									OLED_DispStr(2,18,"����bootģʽ�ɹ�" ,&tFont12);
									OLED_DispStr(2,34,"��ʼ����������" ,&tFont12);
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
							
							OLED_DispStr(2,18,"����bootģʽ��" ,&tFont12);
							OLED_DispStr(2,34,"���Ե�  ��" ,&tFont12);
							pBuf[0] = IAP_t.ErrCnt / 10 + 0x30;
							pBuf[1] = IAP_t.ErrCnt % 10 + 0x30;
							pBuf[2] = '\0';
							OLED_DispStr(40,35,(u8 *)pBuf ,&tFont12);
							
							IAP_t.ErrCnt++;
							if(IAP_t.ErrCnt > ERR_MAX)
							{
								IAP_t.ErrCnt=0;
								OLED_ClrScr(0);
								OLED_DispStr(2,18,"����bootģʽʧ��" ,&tFont12);
								
								IAPState = 255;  
								break;
							}
						}				
						break;
						
			case 2:		
						if(COM1Receive.readIndex != COM1Receive.writeIndex) //������
						{	
							if(!popBuff(USART1))
							{
								break;
							}	
							temp = CRC16_CCITT(COM1Temp.rbuf , 10);
							if((COM1Temp.rbuf[10] == temp / 256)&&(COM1Temp.rbuf[11] == temp % 256))
								 if( (COM1Temp.rbuf[4] == 0x95) && (COM1Temp.rbuf[5] == 0x01))
								 {
									OLED_DispStr(2,34,"�������������ɹ�" ,&tFont12);
									clearBuff(USART1);
									IAP_t.ErrCnt=0;
									IAPState = 3;								
								 }		
						}
						else
						{
											          
							IAP_SendCMD(0x01); 
							IAP_t.Rxd_TimeOut = 4000;		
							OLED_DispStr(2,34,"��ʼ����������" ,&tFont12);
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
								OLED_DispStr(2,24,"����������ʧ��   " ,&tFont12);
								IAPState = 255;  // ��������
								break;
						  }
						}
						break;				
				
			case 3:
				    if(COM1Receive.readIndex != COM1Receive.writeIndex) //������
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
									bsp_LedToggle(1); //��˸LED��
									if(IAP_t.Current_Sector > sector)
									{
										clearBuff(USART1);
									  IAP_t.ErrCnt=0;
									  IAPState = 4;
										OLED_ClrScr(0);
										OLED_DispStr(2,28,"��������д��ɹ� " ,&tFont12);
									}
								 }							 
						}
						else
						{
							IAP_SendData(IAP_t.Current_Sector);
							IAP_t.Rxd_TimeOut = 2000;
              while(IAP_t.Rxd_TimeOut); //������ȴ�ACK
							
							OLED_DispStr(2,48,"��ʼд��   ������" ,&tFont12);
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
			      OLED_DispStr(15,15,"�����ɹ�" ,&tFont12);
			      OLED_DispStr(15,35,"��ת�û���" ,&tFont12);
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
	
	/* ��ȡ�ļ� */
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



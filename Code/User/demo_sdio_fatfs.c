/*
*********************************************************************************************************
*
*	ģ������ : SD��Fat�ļ�ϵͳ��ʾģ�顣
*	�ļ����� : demo_sdio_fatfs.c
*	��    �� : V1.0
*	˵    �� : ��������ֲFatFS�ļ�ϵͳ���汾 R0.09b������ʾ��δ����ļ�����ȡ�ļ�������Ŀ¼��ɾ���ļ�
*			���������ļ���д�ٶȡ�
*********************************************************************************************************
*/

#include "bsp.h"

/* ���ڲ��Զ�д�ٶ� */
#define TEST_FILE_LEN			(2*1024*1024)	/* ���ڲ��Ե��ļ����� */
#define BUF_SIZE				(4*1024)		/* ÿ�ζ�дSD����������ݳ��� */
uint8_t g_TestBuf[BUF_SIZE];


/*
*********************************************************************************************************
*	�� �� ��: DemoFatFS
*	����˵��: FatFS�ļ�ϵͳ��ʾ������
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void DemoFatFS(void)
{
	uint8_t cmd;

	/* ��ӡ�����б��û�����ͨ�����ڲ���ָ�� */
	DispMenu();
	while(1)
	{
		bsp_Idle();		/* ���������bsp.c�ļ����û������޸��������ʵ��CPU���ߺ�ι�� */

		if (comGetChar(COM6, &cmd))	/* �Ӵ��ڶ���һ���ַ�(��������ʽ) */
		{
			printf("\r\n");
			switch (cmd)
			{
				case '1':
					printf("��1 - ViewRootDir��\r\n");
					ViewRootDir();		/* ��ʾSD����Ŀ¼�µ��ļ��� */
					break;

				case '2':
					printf("��2 - CreateNewFile��\r\n");
					CreateNewFile();		/* ����һ�����ļ�,д��һ���ַ��� */
					break;

				case '3':
					printf("��3 - ReadFileData��\r\n");
					ReadFileData();		/* ��ȡ��Ŀ¼��armfly.txt������ */
					break;

				case '4':
					printf("��4 - CreateDir��\r\n");
					CreateDir();		/* ����Ŀ¼ */
					break;

				case '5':
					printf("��5 - DeleteDirFile��\r\n");
					DeleteDirFile();	/* ɾ��Ŀ¼���ļ� */
					break;

				case '6':
					printf("��6 - TestSpeed��\r\n");
					WriteFileTest();	/* �ٶȲ��� */
					break;

				default:
					DispMenu();
					break;
			}
		}
	}
}

/*
*********************************************************************************************************
*	�� �� ��: DispMenu
*	����˵��: ��ʾ������ʾ�˵�
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void DispMenu(void)
{
	printf("\r\n------------------------------------------------\r\n");
	printf("��ѡ���������:\r\n");
	printf("1 - ��ʾSD����Ŀ¼�µ��ļ��б�\r\n");
	printf("2 - ����һ�����ļ�armfly.txt\r\n");
	printf("3 - ��armfly.txt�ļ�������\r\n");
	printf("4 - ����Ŀ¼\r\n");
	printf("5 - ɾ���ļ���Ŀ¼\r\n");
	printf("6 - ��д�ļ��ٶȲ���\r\n");
}

/*
*********************************************************************************************************
*	�� �� ��: ViewRootDir
*	����˵��: ��ʾSD����Ŀ¼�µ��ļ��������γ��ļ�����
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void ViewRootDir(void)
{
	/* ������ʹ�õľֲ�����ռ�ý϶࣬���޸������ļ�����֤��ջ�ռ乻�� */
	FRESULT result;
	FATFS fs;
	DIR DirInf;
	FILINFO FileInf;
	uint32_t cnt = 0;
	uint8_t i=0;
//	char lfname[256];

 	/* �����ļ�ϵͳ */
	result = f_mount(FS_SD, &fs);	/* Mount a logical drive */
	if (result != FR_OK)
	{
		printf("�����ļ�ϵͳʧ�� (%d)\r\n", result);
	}

	/* �򿪸��ļ��� */
	result = f_opendir(&DirInf, "/"); /* ���������������ӵ�ǰĿ¼��ʼ */
	if (result != FR_OK)
	{
		printf("�򿪸�Ŀ¼ʧ�� (%d)\r\n", result);
		return;
	}

//	/* ��ȡ��ǰ�ļ����µ��ļ���Ŀ¼ */
//	FileInf.lfname = lfname;
//	FileInf.lfsize = 256;

	printf("����        |  �ļ���С | �ļ��� \r\n");
	for (cnt = 0; ;cnt++)
	{
		result = f_readdir(&DirInf,&FileInf); 		/* ��ȡĿ¼��������Զ����� */
		if (result != FR_OK || FileInf.fname[0] == 0)
		{
			break;
		}

		if (FileInf.fname[0] == '.')
		{
			continue;
		}

		/* �ж����ļ�������Ŀ¼ */
		if (FileInf.fattrib & AM_DIR)
		{
			printf("(0x%02d)Ŀ¼  ", FileInf.fattrib);
		}
		else
		{
			printf("(0x%02d)�ļ�  ", FileInf.fattrib);
		}

		/* ��ӡ�ļ���С, ���4G */
		printf(" %10d", FileInf.fsize);

		printf("  %s |", FileInf.fname);	/* ���ļ��� */

//		printf("  %s\r\n", (char *)FileInf.lfname);	/* ���ļ��� */
//		if(FileInf.fsize > 1000)
//		{
//			OLED_DispStr(10, 2 + (i++)*16 ,  FileInf.fname , &tFont12); //����Ҫ�ļ���ʾ��Ļ
//		}
	}
  
	
	/* ж���ļ�ϵͳ */
	f_mount(FS_SD, NULL);
}

/*
*********************************************************************************************************
*	�� �� ��: CreateNewFile
*	����˵��: ��SD������һ�����ļ����ļ�������д��www.armfly.com��
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void CreateNewFile(void)
{
	/* ������ʹ�õľֲ�����ռ�ý϶࣬���޸������ļ�����֤��ջ�ռ乻�� */
	FRESULT result;
	FATFS fs;
	FIL file;
	DIR DirInf;
	uint32_t bw;

 	/* �����ļ�ϵͳ */
	result = f_mount(FS_SD, &fs);			/* Mount a logical drive */
	if (result != FR_OK)
	{
		printf("�����ļ�ϵͳʧ�� (%d)\r\n", result);
	}

	/* �򿪸��ļ��� */
	result = f_opendir(&DirInf, "/"); /* ���������������ӵ�ǰĿ¼��ʼ */
	if (result != FR_OK)
	{
		printf("�򿪸�Ŀ¼ʧ�� (%d)\r\n", result);
		return;
	}

	/* ���ļ� */
	result = f_open(&file, "armfly.txt", FA_CREATE_ALWAYS | FA_WRITE);

	/* дһ������ */
	result = f_write(&file, "FatFS Write Demo \r\n www.armfly.com \r\n", 34, &bw);
	if (result == FR_OK)
	{
		printf("armfly.txt �ļ�д��ɹ�\r\n");
	}
	else
	{
		printf("armfly.txt �ļ�д��ʧ��\r\n");
	}

	/* �ر��ļ�*/
	f_close(&file);

	/* ж���ļ�ϵͳ */
	f_mount(FS_SD, NULL);
}

unsigned char File_buf[1024];
/*
*********************************************************************************************************
*	�� �� ��: ReadFileData
*	����˵��: ��ȡ�ļ�
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void ReadFileData(void)
{
	/* ������ʹ�õľֲ�����ռ�ý϶࣬���޸������ļ�����֤��ջ�ռ乻�� */
	FRESULT result;
	FATFS fs;
	FIL file;
	DIR DirInf;
	uint32_t bw;
  FILINFO FileInf;
	unsigned int sector; //512�ֽ�Ϊһ������ 
	
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
		printf("newname.txt size : %lu\r\n",FileInf.fsize);//��ȡ�ļ��ĳ��ȣ���ռ�����ֽ�
		
		printf("fdate : %d\r\n",FileInf.fdate); //��ȡ�ļ�������޸����� ת��Ϊ2���ƣ�
		
		printf("ftime : %d\r\n",FileInf.ftime);//��ȡ�ļ�������޸�ʱ�䣬ת��Ϊ2����
		
		printf("fattrib : %d\r\n",FileInf.fattrib);//��ʾ�ļ������ԣ���ʲô�ļ�
	}
	
	/* ���ļ� */
	
	result = f_open(&file, "LED0.bin", FA_OPEN_EXISTING | FA_READ);
	if (result !=  FR_OK)
	{
		printf("Don't Find File : LED0.bin\r\n");
		return;
	}     
		
	/* ��ȡ�ļ� */
	sector = FileInf.fsize/512;
	for(unsigned int i; i < sector; i++)
	{
		f_read(&file, &File_buf, 512, &bw);
		COM1_SendData(File_buf,512);
		bsp_LedToggle(1);
	}
	f_read(&file, &File_buf, 512, &bw);
  COM1_SendData(File_buf,FileInf.fsize%512);	
	
	/* �ر��ļ�*/
	f_close(&file);

	/* ж���ļ�ϵͳ */
	f_mount(FS_SD, NULL);
}

/*
*********************************************************************************************************
*	�� �� ��: CreateDir
*	����˵��: ��SD����Ŀ¼����Dir1��Dir2Ŀ¼����Dir1Ŀ¼�´�����Ŀ¼Dir1_1
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void CreateDir(void)
{
	/* ������ʹ�õľֲ�����ռ�ý϶࣬���޸������ļ�����֤��ջ�ռ乻�� */
	FRESULT result;
	FATFS fs;

 	/* �����ļ�ϵͳ */
	result = f_mount(FS_SD, &fs);			/* Mount a logical drive */
	if (result != FR_OK)
	{
		printf("�����ļ�ϵͳʧ�� (%d)\r\n", result);
	}

	/* ����Ŀ¼/Dir1 */
	result = f_mkdir("/Dir1");
	if (result == FR_OK)
	{
		printf("f_mkdir Dir1 Ok\r\n");
	}
	else if (result == FR_EXIST)
	{
		printf("Dir1 Ŀ¼�Ѿ�����(%d)\r\n", result);
	}
	else
	{
		printf("f_mkdir Dir1 ʧ�� (%d)\r\n", result);
		return;
	}

	/* ����Ŀ¼/Dir2 */
	result = f_mkdir("/Dir2");
	if (result == FR_OK)
	{
		printf("f_mkdir Dir2 Ok\r\n");
	}
	else if (result == FR_EXIST)
	{
		printf("Dir2 Ŀ¼�Ѿ�����(%d)\r\n", result);
	}
	else
	{
		printf("f_mkdir Dir2 ʧ�� (%d)\r\n", result);
		return;
	}

	/* ������Ŀ¼ /Dir1/Dir1_1	   ע�⣺������Ŀ¼Dir1_1ʱ�������ȴ�����Dir1 */
	result = f_mkdir("/Dir1/Dir1_1"); /* */
	if (result == FR_OK)
	{
		printf("f_mkdir Dir1_1 �ɹ�\r\n");
	}
	else if (result == FR_EXIST)
	{
		printf("Dir1_1 Ŀ¼�Ѿ����� (%d)\r\n", result);
	}
	else
	{
		printf("f_mkdir Dir1_1 ʧ�� (%d)\r\n", result);
		return;
	}

	/* ж���ļ�ϵͳ */
	f_mount(FS_SD, NULL);
}

/*
*********************************************************************************************************
*	�� �� ��: DeleteDirFile
*	����˵��: ɾ��SD����Ŀ¼�µ� armfly.txt �ļ��� Dir1��Dir2 Ŀ¼
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void DeleteDirFile(void)
{
	/* ������ʹ�õľֲ�����ռ�ý϶࣬���޸������ļ�����֤��ջ�ռ乻�� */
	FRESULT result;
	FATFS fs;
	char FileName[13];
	uint8_t i;

 	/* �����ļ�ϵͳ */
	result = f_mount(FS_SD, &fs);			/* Mount a logical drive */
	if (result != FR_OK)
	{
		printf("�����ļ�ϵͳʧ�� (%d)\r\n", result);
	}

	#if 0
	/* �򿪸��ļ��� */
	result = f_opendir(&DirInf, "/"); /* ���������������ӵ�ǰĿ¼��ʼ */
	if (result != FR_OK)
	{
		printf("�򿪸�Ŀ¼ʧ��(%d)\r\n", result);
		return;
	}
	#endif

	/* ɾ��Ŀ¼/Dir1 ����Ϊ������Ŀ¼�ǿգ�������Ŀ¼)���������ɾ����ʧ�ܡ�*/
	result = f_unlink("/Dir1");
	if (result == FR_OK)
	{
		printf("ɾ��Ŀ¼Dir1�ɹ�\r\n");
	}
	else if (result == FR_NO_FILE)
	{
		printf("û�з����ļ���Ŀ¼ :%s\r\n", "/Dir1");
	}
	else
	{
		printf("ɾ��Dir1ʧ��(������� = %d) �ļ�ֻ����Ŀ¼�ǿ�\r\n", result);
	}

	/* ��ɾ��Ŀ¼/Dir1/Dir1_1 */
	result = f_unlink("/Dir1/Dir1_1");
	if (result == FR_OK)
	{
		printf("ɾ����Ŀ¼/Dir1/Dir1_1�ɹ�\r\n");
	}
	else if ((result == FR_NO_FILE) || (result == FR_NO_PATH))
	{
		printf("û�з����ļ���Ŀ¼ :%s\r\n", "/Dir1/Dir1_1");
	}
	else
	{
		printf("ɾ����Ŀ¼/Dir1/Dir1_1ʧ��(������� = %d) �ļ�ֻ����Ŀ¼�ǿ�\r\n", result);
	}

	/* ��ɾ��Ŀ¼/Dir1 */
	result = f_unlink("/Dir1");
	if (result == FR_OK)
	{
		printf("ɾ��Ŀ¼Dir1�ɹ�\r\n");
	}
	else if (result == FR_NO_FILE)
	{
		printf("û�з����ļ���Ŀ¼ :%s\r\n", "/Dir1");
	}
	else
	{
		printf("ɾ��Dir1ʧ��(������� = %d) �ļ�ֻ����Ŀ¼�ǿ�\r\n", result);
	}

	/* ɾ��Ŀ¼/Dir2 */
	result = f_unlink("/Dir2");
	if (result == FR_OK)
	{
		printf("ɾ��Ŀ¼ Dir2 �ɹ�\r\n");
	}
	else if (result == FR_NO_FILE)
	{
		printf("û�з����ļ���Ŀ¼ :%s\r\n", "/Dir2");
	}
	else
	{
		printf("ɾ��Dir2 ʧ��(������� = %d) �ļ�ֻ����Ŀ¼�ǿ�\r\n", result);
	}

	/* ɾ���ļ� armfly.txt */
	result = f_unlink("armfly.txt");
	if (result == FR_OK)
	{
		printf("ɾ���ļ� armfly.txt �ɹ�\r\n");
	}
	else if (result == FR_NO_FILE)
	{
		printf("û�з����ļ���Ŀ¼ :%s\r\n", "armfly.txt");
	}
	else
	{
		printf("ɾ��armfly.txtʧ��(������� = %d) �ļ�ֻ����Ŀ¼�ǿ�\r\n", result);
	}

	/* ɾ���ļ� speed1.txt */
	for (i = 0; i < 20; i++)
	{
		sprintf(FileName, "Speed%02d.txt", i);		/* ÿд1�Σ���ŵ��� */
		result = f_unlink(FileName);
		if (result == FR_OK)
		{
			printf("ɾ���ļ�%s�ɹ�\r\n", FileName);
		}
		else if (result == FR_NO_FILE)
		{
			printf("û�з����ļ�:%s\r\n", FileName);
		}
		else
		{
			printf("ɾ��%s�ļ�ʧ��(������� = %d) �ļ�ֻ����Ŀ¼�ǿ�\r\n", FileName, result);
		}
	}

	/* ж���ļ�ϵͳ */
	f_mount(FS_SD, NULL);
}

/*
*********************************************************************************************************
*	�� �� ��: WriteFileTest
*	����˵��: �����ļ���д�ٶ�
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void WriteFileTest(void)
{
	/* ������ʹ�õľֲ�����ռ�ý϶࣬���޸������ļ�����֤��ջ�ռ乻�� */
	FRESULT result;
	FATFS fs;
	FIL file;
	DIR DirInf;
	uint32_t bw;
	uint32_t i,k;
	uint32_t runtime1,runtime2,timelen;
	uint8_t err = 0;
	char TestFileName[13];
	static uint8_t s_ucTestSn = 0;

	for (i = 0; i < sizeof(g_TestBuf); i++)
	{
		g_TestBuf[i] = (i / 512) + '0';
	}

  	/* �����ļ�ϵͳ */
	result = f_mount(FS_SD, &fs);			/* Mount a logical drive */
	if (result != FR_OK)
	{
		printf("�����ļ�ϵͳʧ�� (%d)\r\n", result);
	}

	/* �򿪸��ļ��� */
	result = f_opendir(&DirInf, "/"); /* ���������������ӵ�ǰĿ¼��ʼ */
	if (result != FR_OK)
	{
		printf("�򿪸�Ŀ¼ʧ�� (%d)\r\n", result);
		return;
	}

	/* ���ļ� */
	sprintf(TestFileName, "Speed%02d.txt", s_ucTestSn++);		/* ÿд1�Σ���ŵ��� */
	result = f_open(&file, TestFileName, FA_CREATE_ALWAYS | FA_WRITE);

	/* дһ������ */
	printf("��ʼд�ļ�%s %dKB ...\r\n", TestFileName, TEST_FILE_LEN / 1024);
	runtime1 = bsp_GetRunTime();	/* ��ȡϵͳ����ʱ�� */
	for (i = 0; i < TEST_FILE_LEN / BUF_SIZE; i++)
	{
		result = f_write(&file, g_TestBuf, sizeof(g_TestBuf), &bw);
		if (result == FR_OK)
		{
			if (((i + 1) % 8) == 0)
			{
				printf(".");
			}
		}
		else
		{
			err = 1;
			printf("%s�ļ�дʧ��\r\n", TestFileName);
			break;
		}
	}
	runtime2 = bsp_GetRunTime();	/* ��ȡϵͳ����ʱ�� */

	if (err == 0)
	{
		timelen = (runtime2 - runtime1);
		printf("\r\n  д��ʱ : %dms   ƽ��д�ٶ� : %dB/S (%dKB/S)\r\n",
			timelen,
			(TEST_FILE_LEN * 1000) / timelen,
			((TEST_FILE_LEN / 1024) * 1000) / timelen);
	}

	f_close(&file);		/* �ر��ļ�*/


	/* ��ʼ���ļ����� */
	result = f_open(&file, TestFileName, FA_OPEN_EXISTING | FA_READ);
	if (result !=  FR_OK)
	{
		printf("û���ҵ��ļ�: %s\r\n", TestFileName);
		return;
	}

	printf("��ʼ���ļ� %dKB ...\r\n", TEST_FILE_LEN / 1024);
	runtime1 = bsp_GetRunTime();	/* ��ȡϵͳ����ʱ�� */
	for (i = 0; i < TEST_FILE_LEN / BUF_SIZE; i++)
	{
		result = f_read(&file, g_TestBuf, sizeof(g_TestBuf), &bw);
		if (result == FR_OK)
		{
			if (((i + 1) % 8) == 0)
			{
				printf(".");
			}

			/* �Ƚ�д��������Ƿ���ȷ�������ᵼ�¶����ٶȽ�����͵� 3.5MBytes/S */
			for (k = 0; k < sizeof(g_TestBuf); k++)
			{
				if (g_TestBuf[k] != (k / 512) + '0')
				{
				  	err = 1;
					printf("Speed1.txt �ļ����ɹ����������ݳ���\r\n");
					break;
				}
			}
			if (err == 1)
			{
				break;
			}
		}
		else
		{
			err = 1;
			printf("Speed1.txt �ļ���ʧ��\r\n");
			break;
		}
	}
	runtime2 = bsp_GetRunTime();	/* ��ȡϵͳ����ʱ�� */

	if (err == 0)
	{
		timelen = (runtime2 - runtime1);
		printf("\r\n  ����ʱ : %dms   ƽ�����ٶ� : %dB/S (%dKB/S)\r\n", timelen,
			(TEST_FILE_LEN * 1000) / timelen, ((TEST_FILE_LEN / 1024) * 1000) / timelen);
	}

	/* �ر��ļ�*/
	f_close(&file);

	/* ж���ļ�ϵͳ */
	f_mount(FS_SD, NULL);
}

//ͨ�����ڴ�ӡSD�������Ϣ
void show_sdcard_info(void)
{
	switch(SDCardInfo.CardType)
	{
		case SDIO_STD_CAPACITY_SD_CARD_V1_1:printf("Card Type:SDSC V1.1\r\n");break;
		case SDIO_STD_CAPACITY_SD_CARD_V2_0:printf("Card Type:SDSC V2.0\r\n");break;
		case SDIO_HIGH_CAPACITY_SD_CARD:printf("Card Type:SDHC V2.0\r\n");break;
		case SDIO_MULTIMEDIA_CARD:printf("Card Type:MMC Card\r\n");break;
	}	
  printf("Card ManufacturerID:%d\r\n",SDCardInfo.SD_cid.ManufacturerID);	//������ID
 	printf("Card RCA:%d\r\n",SDCardInfo.RCA);								//����Ե�ַ
	printf("Card Capacity:%d MB\r\n",(u32)(SDCardInfo.CardCapacity>>20));	//��ʾ����
 	printf("Card BlockSize:%d\r\n\r\n",SDCardInfo.CardBlockSize);			//��ʾ���С
}


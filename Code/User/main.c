#include "bsp.h"			/* �ײ�Ӳ������ */


/* ���������������̷������� */
#define EXAMPLE_NAME	"SWD DownLoader"
#define EXAMPLE_DATE	"2020-03-15"
#define DEMO_VER		   "1.0"

/* �������ļ��ڵ��õĺ������� */
static void PrintfLogo(void);
extern u8 nonside[];

FONT_T tFont12, tFont16, tFont24, tFont32;          
u8 CRCTest[] = {0x68,0x00,0x06,0x68,0x15,0xff,0,0,0,0};   
u8 Maintask_Status = State_Init;

/*
*********************************************************************************************************
*	�� �� ��: main
*	����˵��: c�������
*	��    �Σ���
*	�� �� ֵ: �������(���账��)
*********************************************************************************************************
*/
int main(void)
{
	{
		/* ����������� */
		tFont16.FontCode = FC_ST_16;	/* ������� 16���� */
		tFont16.FrontColor = 1;		/* ������ɫ 0 �� 1 */
		tFont16.BackColor = 0;		/* ���ֱ�����ɫ 0 �� 1 */
		tFont16.Space = 0;			/* ���ּ�࣬��λ = ���� */

		tFont12.FontCode = FC_ST_12;	/* ������� 12���� */
		tFont12.FrontColor = 1;		/* ������ɫ 0 �� 1 */
		tFont12.BackColor = 0;		/* ���ֱ�����ɫ 0 �� 1 */
		tFont12.Space = 1;			/* ���ּ�࣬��λ = ���� */
		
		tFont24.FontCode = FC_ST_24;	/* ������� 24���� */
		tFont24.FrontColor = 1;		/* ������ɫ 0 �� 1 */
		tFont24.BackColor = 0;		/* ���ֱ�����ɫ 0 �� 1 */
		tFont24.Space = 1;			/* ���ּ�࣬��λ = ���� */
		
		tFont32.FontCode = FC_ST_32;	/* ������� 32���� */
		tFont32.FrontColor = 1;		/* ������ɫ 0 �� 1 */
		tFont32.BackColor = 0;		/* ���ֱ�����ɫ 0 �� 1 */
		tFont32.Space = 1;			/* ���ּ�࣬��λ = ���� */	
	}
	/*
		ST�̼����е������ļ��Ѿ�ִ���� SystemInit() �������ú����� system_stm32f4xx.c �ļ�����Ҫ������
	����CPUϵͳ��ʱ�ӣ��ڲ�Flash����ʱ������FSMC�����ⲿSRAM
	*/
	bsp_Init();		/* Ӳ����ʼ�� */
	PrintfLogo();	/* ��ӡ������Ϣ������1 */
	
	OLED_ClrScr(0);

	OLED_DispStr(30,0,"SWD����" ,&tFont12);
	bsp_DelayMS(1200);
  
 BEEP_Stop();
	
	ViewRootDir();		      /* ��ʾSD����Ŀ¼�µ��ļ��� */
	usbd_OpenMassStorage();	/* ��ʼ��USB Device������ΪMass Storage */
//	
	printf("��2 - ʹ��U�̡�\r\n");
	
	/* ����������ѭ���� */
	
	while (1)
	{
		bsp_Idle();		/* ���������bsp.c�ļ����û������޸��������ʵ��CPU���ߺ�ι�� */
		
//		switch(Maintask_Status) //IAP���������ĳ���
//		{
//			case State_Init: 
//			        if( IsKeyDown1() == 1 ) 
//										 {
//											// Test_Hex2Bin();
//											 
//											 BEEP_KeyTone();
//											 OpenFile(); //���ļ�ϵͳ
//											 Maintask_Status = State_IAP;
//										 }
//				             break;
//			case State_KEY:
//			               Maintask_Status = State_IAP;
//				             break;
//			case State_IAP:
//				             IAPData();
//				             break;
//			case State_Sleep:
//				        break;
//      default :
//                break;						
//		}

		switch(Maintask_Status)  //SWD���صĳ���
		{
			case State_Init: 
			        if( IsKeyDown1() == 1 ) 
										 {
												bsp_DelayMS(100);
											 BEEP_KeyTone();
											 OpenFile(); //���ļ�ϵͳ
											 Maintask_Status = State_IAP;
										 }
				       break;
			case State_KEY:
			               Maintask_Status = State_IAP;
				             break;
			case State_IAP:
																	bsp_DelayMS(100);
				             SWD_UpData(); //��ʼִ��SWD����
                 //test();     //��дDP AP�Ĵ�������
				             break;
			case State_Sleep:
				        break;
      default :
                break;						
		}
	}
}

/*
*********************************************************************************************************
*	�� �� ��: PrintfLogo
*	����˵��: ��ӡ�������ƺ����̷�������, ���ϴ����ߺ󣬴�PC���ĳ����ն�������Թ۲���
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void PrintfLogo(void)
{
	/* ���CPU ID */
	{
		/* �ο��ֲ᣺
			32.6.1 MCU device ID code
			33.1 Unique device ID register (96 bits)
		*/
		uint32_t CPU_Sn0, CPU_Sn1, CPU_Sn2;

		CPU_Sn0 = *(__IO uint32_t*)(0x1FFF7A10);
		CPU_Sn1 = *(__IO uint32_t*)(0x1FFF7A10 + 4);
		CPU_Sn2 = *(__IO uint32_t*)(0x1FFF7A10 + 8);

		printf("\r\nCPU : STM32F401RCT6, LQFP64, UID = %08X %08X %08X\r\n"
			, CPU_Sn2, CPU_Sn1, CPU_Sn0);
	}

	printf("\n\r");
	printf("*************************************************************\r\n");
	printf("* ��������   : %s\r\n", EXAMPLE_NAME);	/* ��ӡ�������� */
	printf("* ���̰汾   : %s\r\n", DEMO_VER);		/* ��ӡ���̰汾 */
	//printf("* ��������   : %s\r\n", EXAMPLE_DATE);	/* ��ӡ�������� */

	/* ��ӡST�̼���汾����3���������stm32f10x.h�ļ��� */
	printf("* �̼���汾 : V%d.%d.%d (STM32F4xx_StdPeriph_Driver)\r\n", __STM32F4XX_STDPERIPH_VERSION_MAIN,
			__STM32F4XX_STDPERIPH_VERSION_SUB1,__STM32F4XX_STDPERIPH_VERSION_SUB2);
	printf("* \r\n");	/* ��ӡһ�пո� */

	printf("* Copyright 	�Ͼ���Ϣ���̴�ѧ�����о���Ժ Code Fan\r\n");
	printf("*************************************************************\r\n");
}

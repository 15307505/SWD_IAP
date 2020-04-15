/*
*********************************************************************************************************
*
*	ģ������ : �ַ�������\��ֵת��
*	�ļ����� : bsp_user_lib.c
*	��    �� : V1.1
*	˵    �� : �ṩһЩ���õ�sting��mem���������Լ�Modbus CRC16����
*   �޸ļ�¼ :
*               V1.0  2013-12-05  �װ�
*               V1.1  2014-06-20  ���Ӵ�С������ת������
*********************************************************************************************************
*/

#include "bsp.h"

    // CRC ��λ�ֽ�ֵ��
static const uint8_t s_CRCHi[] = {
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40
} ;
// CRC ��λ�ֽ�ֵ��
const uint8_t s_CRCLo[] = {
	0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06,
	0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD,
	0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
	0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A,
	0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4,
	0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
	0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3,
	0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4,
	0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
	0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29,
	0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED,
	0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
	0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60,
	0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67,
	0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
	0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68,
	0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E,
	0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
	0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71,
	0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92,
	0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
	0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B,
	0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B,
	0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
	0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42,
	0x43, 0x83, 0x41, 0x81, 0x80, 0x40
};

/*
*********************************************************************************************************
*	�� �� ��: str_len
*	����˵��: �����ַ�������.0�ǽ�����
*	��    ��: _str : ������
*	�� �� ֵ: ��
*********************************************************************************************************
*/
int str_len(char *_str)
{
	int len = 0;

	while (*_str++) len++;
	return len;
}

/*
*********************************************************************************************************
*	�� �� ��: str_cpy
*	����˵��: �����ַ���
*	��    ��: tar : Ŀ�껺����
*			 src : Դ������
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void str_cpy(char *_tar, char *_src)
{
	do
	{
		*_tar++ = *_src;
	}
	while (*_src++);
}

/*
*********************************************************************************************************
*	�� �� ��: str_cmp
*	����˵��: �ַ����Ƚ�
*	��    ��: s1 : �ַ���1
*			  s2 : �ַ���2
*	�� �� ֵ: 0 ��ʾ��� ��0��ʾ����
*********************************************************************************************************
*/
int str_cmp(char * s1, char * s2)
{
	while ((*s1!=0) && (*s2!=0) && (*s1==*s2))
	{
		s1++;
		s2++;
	}
	return *s1 - *s2;
}

/*
*********************************************************************************************************
*	�� �� ��: str_copy
*	����˵��: �����ַ���
*	��    ��: tar : Ŀ�껺����
*			 src : Դ������
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void mem_set(char *_tar, char _data, int _len)
{
	while (_len--)
	{
		*_tar++ = _data;
	}
}

/*
*********************************************************************************************************
*	�� �� ��: int_to_ascii
*	����˵��: ������ת��ΪASCII���顣֧�ָ�����
*	��    ��: _Number : ����
*			 _pBuf : Ŀ�껺����, ���ת����Ľ������0�������ַ�����
*			 _len : ASCII�ַ�����, �ַ�������
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void int_to_str(int _iNumber, char *_pBuf, unsigned char _len)
{
	unsigned char i;
	int iTemp;

	if (_iNumber < 0)	/* ���� */
	{
		iTemp = -_iNumber;	/* תΪ���� */
	}
	else
	{
		iTemp = _iNumber;
	}

	mem_set(_pBuf, ' ',_len);

	/* ������ת��ΪASCII�ַ��� */
	for (i = 0; i < _len; i++)
	{
		_pBuf[_len - 1 - i] = (iTemp % 10) + '0';
		iTemp = iTemp / 10;
		if (iTemp == 0)
		{
			break;
		}
	}
	_pBuf[_len] = 0;

	if (_iNumber < 0)	/* ���� */
	{
		for (i = 0; i < _len; i++)
		{
			if ((_pBuf[i] == ' ') && (_pBuf[i + 1] != ' '))
			{
				_pBuf[i] = '-';
				break;
			}
		}
	}
}

/*
*********************************************************************************************************
*	�� �� ��: str_to_int
*	����˵��: ��ASCII���ַ���ת���������� ����С�����Զ�Խ����
*	��    ��: _pStr :��ת����ASCII�봮. �����Զ��ţ�#��0������ 2014-06-20 �޸�Ϊ��0-9���ַ���
*	�� �� ֵ: ����������ֵ
*********************************************************************************************************
*/
int str_to_int(char *_pStr)
{
	unsigned char flag;
	char *p;
	int ulInt;
	unsigned char  i;
	unsigned char  ucTemp;

	p = _pStr;
	if (*p == '-')
	{
		flag = 1;	/* ���� */
		p++;
	}
	else
	{
		flag = 0;
	}

	ulInt = 0;
	for (i = 0; i < 15; i++)
	{
		ucTemp = *p;
		if (ucTemp == '.')	/* ����С���㣬�Զ�����1���ֽ� */
		{
			p++;
			ucTemp = *p;
		}
		if ((ucTemp >= '0') && (ucTemp <= '9'))
		{
			ulInt = ulInt * 10 + (ucTemp - '0');
			p++;
		}
		else
		{
			break;
		}
	}

	if (flag == 1)
	{
		return -ulInt;
	}
	return ulInt;
}


/*
*********************************************************************************************************
*	�� �� ��: BEBufToUint16
*	����˵��: ��2�ֽ�����(���Big Endian���򣬸��ֽ���ǰ)ת��Ϊ16λ����
*	��    ��: _pBuf : ����
*	�� �� ֵ: 16λ����ֵ
*
*   ���(Big Endian)��С��(Little Endian)
*********************************************************************************************************
*/
uint16_t BEBufToUint16(uint8_t *_pBuf)
{
    return (((uint16_t)_pBuf[0] << 8) | _pBuf[1]);
}

/*
*********************************************************************************************************
*	�� �� ��: BEBufToUint16
*	����˵��: ��2�ֽ�����(С��Little Endian�����ֽ���ǰ)ת��Ϊ16λ����
*	��    ��: _pBuf : ����
*	�� �� ֵ: 16λ����ֵ
*********************************************************************************************************
*/
uint16_t LEBufToUint16(uint8_t *_pBuf)
{
    return (((uint16_t)_pBuf[1] << 8) | _pBuf[0]);
}

/*
*********************************************************************************************************
*	�� �� ��: CRC16_Modbus
*	����˵��: ����CRC�� ����ModbusЭ�顣
*	��    ��: _pBuf : ����У�������
*			  _usLen : ���ݳ���
*	�� �� ֵ: 16λ����ֵ�� ����Modbus ���˽�����ֽ��ȴ��ͣ����ֽں��͡�
*
*   ���п��ܵ�CRCֵ����Ԥװ���������鵱�У������㱨������ʱ���Լ򵥵��������ɣ�
*   һ�����������16λCRC�������256�����ܵĸ�λ�ֽڣ���һ�����麬�е�λ�ֽڵ�ֵ��
*   ������������CRC�ķ�ʽ�ṩ�˱ȶԱ��Ļ�������ÿһ�����ַ��������µ�CRC����ķ�����
*
*  ע�⣺�˳����ڲ�ִ�и�/��CRC�ֽڵĽ������˺������ص����Ѿ�����������CRCֵ��Ҳ����˵���ú����ķ���ֵ����ֱ�ӷ���
*        �ڱ������ڷ��ͣ�
*********************************************************************************************************
*/
uint16_t CRC16_Modbus(uint8_t *_pBuf, uint16_t _usLen)
{
	uint8_t ucCRCHi = 0xFF; /* ��CRC�ֽڳ�ʼ�� */
	uint8_t ucCRCLo = 0xFF; /* ��CRC �ֽڳ�ʼ�� */
	uint16_t usIndex;  /* CRCѭ���е����� */

    while (_usLen--)
    {
		usIndex = ucCRCHi ^ *_pBuf++; /* ����CRC */
		ucCRCHi = ucCRCLo ^ s_CRCHi[usIndex];
		ucCRCLo = s_CRCLo[usIndex];
    }
    return ((uint16_t)ucCRCHi << 8 | ucCRCLo);
}


/*
*********************************************************************************************************
*	�� �� ��: CaculTwoPoint
*	����˵��: ����2��ֱ�߷��̣�����Yֵ
*	��    ��:  2����������x������
*	�� �� ֵ: x��Ӧ��yֵ
*********************************************************************************************************
*/
int32_t  CaculTwoPoint(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x)
{
	return y1 + ((int64_t)(y2 - y1) * (x - x1)) / (x2 - x1);
}


unsigned short CRC16_CCITT(unsigned char *puchMsg, unsigned int usDataLen)
{
  unsigned short wCRCin = 0x0000;
  unsigned short wCPoly = 0x1021;
  unsigned char wChar = 0;
  
  while (usDataLen--) 	
  {
        wChar = *(puchMsg++);
        InvertUint8(&wChar,&wChar);
        wCRCin ^= (wChar << 8);
        for(int i = 0;i < 8;i++)
        {
          if(wCRCin & 0x8000)
            wCRCin = (wCRCin << 1) ^ wCPoly;
          else
            wCRCin = wCRCin << 1;
        }
  }
  InvertUint16(&wCRCin,&wCRCin);
  return (wCRCin) ;
}

unsigned short CRC16_CCITT_FALSE(unsigned char *puchMsg, unsigned int usDataLen)
{
  unsigned short wCRCin = 0xFFFF;
  unsigned short wCPoly = 0x1021;
  unsigned char wChar = 0;
  
  while (usDataLen--) 	
  {
        wChar = *(puchMsg++);
        wCRCin ^= (wChar << 8);
        for(int i = 0;i < 8;i++)
        {
          if(wCRCin & 0x8000)
            wCRCin = (wCRCin << 1) ^ wCPoly;
          else
            wCRCin = wCRCin << 1;
        }
  }
  return (wCRCin) ;
}
unsigned short CRC16_XMODEM(unsigned char *puchMsg, unsigned int usDataLen)
{
  unsigned short wCRCin = 0x0000;
  unsigned short wCPoly = 0x1021;
  unsigned char wChar = 0;
  
  while (usDataLen--) 	
  {
        wChar = *(puchMsg++);
        wCRCin ^= (wChar << 8);
        for(int i = 0;i < 8;i++)
        {
          if(wCRCin & 0x8000)
            wCRCin = (wCRCin << 1) ^ wCPoly;
          else
            wCRCin = wCRCin << 1;
        }
  }
  return (wCRCin) ;
}
 
unsigned short CRC16_X25(unsigned char *puchMsg, unsigned int usDataLen)
{
  unsigned short wCRCin = 0xFFFF;
  unsigned short wCPoly = 0x1021;
  unsigned char wChar = 0;
  
  while (usDataLen--) 	
  {
        wChar = *(puchMsg++);
        InvertUint8(&wChar,&wChar);
        wCRCin ^= (wChar << 8);
        for(int i = 0;i < 8;i++)
        {
          if(wCRCin & 0x8000)
            wCRCin = (wCRCin << 1) ^ wCPoly;
          else
            wCRCin = wCRCin << 1;
        }
  }
  InvertUint16(&wCRCin,&wCRCin);
  return (wCRCin^0xFFFF) ;
}
 
unsigned short CRC16_MODBUS(unsigned char *puchMsg, unsigned int usDataLen)
{
  unsigned short wCRCin = 0xFFFF;
  unsigned short wCPoly = 0x8005;
  unsigned char wChar = 0;
  
  while (usDataLen--) 	
  {
        wChar = *(puchMsg++);
        InvertUint8(&wChar,&wChar);
        wCRCin ^= (wChar << 8);
        for(int i = 0;i < 8;i++)
        {
          if(wCRCin & 0x8000)
            wCRCin = (wCRCin << 1) ^ wCPoly;
          else
            wCRCin = wCRCin << 1;
        }
  }
  InvertUint16(&wCRCin,&wCRCin);
  return (wCRCin) ;
}

unsigned short CRC16_IBM(unsigned char *puchMsg, unsigned int usDataLen)
{
  unsigned short wCRCin = 0x0000;
  unsigned short wCPoly = 0x8005;
  unsigned char wChar = 0;
  
  while (usDataLen--) 	
  {
        wChar = *(puchMsg++);
        InvertUint8(&wChar,&wChar);
        wCRCin ^= (wChar << 8);
        for(int i = 0;i < 8;i++)
        {
          if(wCRCin & 0x8000)
            wCRCin = (wCRCin << 1) ^ wCPoly;
          else
            wCRCin = wCRCin << 1;
        }
  }
  InvertUint16(&wCRCin,&wCRCin);
  return (wCRCin) ;
}

unsigned short CRC16_MAXIM(unsigned char *puchMsg, unsigned int usDataLen)
{
  unsigned short wCRCin = 0x0000;
  unsigned short wCPoly = 0x8005;
  unsigned char wChar = 0;
  
  while (usDataLen--) 	
  {
        wChar = *(puchMsg++);
        InvertUint8(&wChar,&wChar);
        wCRCin ^= (wChar << 8);
        for(int i = 0;i < 8;i++)
        {
          if(wCRCin & 0x8000)
            wCRCin = (wCRCin << 1) ^ wCPoly;
          else
            wCRCin = wCRCin << 1;
        }
  }
  InvertUint16(&wCRCin,&wCRCin);
  return (wCRCin^0xFFFF) ;
}

unsigned short CRC16_USB(unsigned char *puchMsg, unsigned int usDataLen)
{
  unsigned short wCRCin = 0xFFFF;
  unsigned short wCPoly = 0x8005;
  unsigned char wChar = 0;
  
  while (usDataLen--) 	
  {
        wChar = *(puchMsg++);
        InvertUint8(&wChar,&wChar);
        wCRCin ^= (wChar << 8);
        for(int i = 0;i < 8;i++)
        {
          if(wCRCin & 0x8000)
            wCRCin = (wCRCin << 1) ^ wCPoly;
          else
            wCRCin = wCRCin << 1;
        }
  }
  InvertUint16(&wCRCin,&wCRCin);
  return (wCRCin^0xFFFF) ;
}

void InvertUint8(unsigned char *dBuf,unsigned char *srcBuf)
{
    int i;
    unsigned char tmp[4];
    tmp[0] = 0;
    for(i=0;i< 8;i++)
    {
      if(srcBuf[0]& (1 << i))
        tmp[0]|=1<<(7-i);
    }
    dBuf[0] = tmp[0];
    
}
void InvertUint16(unsigned short *dBuf,unsigned short *srcBuf)
{
    int i;
    unsigned short tmp[4];
    tmp[0] = 0;
    for(i=0;i< 16;i++)
    {
      if(srcBuf[0]& (1 << i))
        tmp[0]|=1<<(15 - i);
    }
    dBuf[0] = tmp[0];
}
void InvertUint32(unsigned int *dBuf,unsigned int *srcBuf)
{
    int i;
    unsigned int tmp[4];
 
    tmp[0] = 0;
    
    for(i=0;i< 32;i++)
    {
      if(srcBuf[0]& (1 << i))
        tmp[0]|=1<<(15 - i);
    }
    dBuf[0] = tmp[0];
}

//CRC32�㷨:
unsigned int CRC32(unsigned char *puchMsg, unsigned int usDataLen)
{
  int i;
  unsigned int wCRCin = 0xFFFFFFFF;
  unsigned int wCPoly = 0x04C11DB7;
  unsigned int wChar = 0;
  while (usDataLen--) 	
  {
        wChar = *(puchMsg++);
        InvertUint8((unsigned char *)&wChar,(unsigned char *)&wChar);
        wCRCin ^= (wChar << 24);
        for(i = 0;i < 8;i++)
        {
          if(wCRCin & 0x80000000)
            wCRCin = (wCRCin << 1) ^ wCPoly;
          else
            wCRCin = wCRCin << 1;
        }
  }
  InvertUint32(&wCRCin,&wCRCin);
  return (wCRCin ^ 0xFFFFFFFF) ;
}



void LongInt_To_Asc(unsigned long int *data,unsigned char *length,unsigned char *data1)
{ 
  int i,k;
  unsigned long j;
  *length=0;
  j=1000000000;
  k=0;
  for (i=0;i<10;i++)
      {
     /*   if (j==10)
           { if (k==0)
                { *(data1+k)=0x30;
                  k++;
                  *(data1+k)='.';
                  k++;
                  *(data1+k)='0';
                  k++;
                  *(data1+k)='0';
                  k++;
                  }
             else
                { *(data1+k)='.';
                  k++;
                  }
             }*/
        if ((*data/j)!=0)
                { *(data1+k)=(*data)%(j*10)/j+0x30;
                  k++;
                  }
        j=j/10;
        }
  *length=k;
}

void Asc_To_LongInt_2(unsigned char *data,unsigned char length,unsigned long int *data1)
{ unsigned char i,j,k;
  *data1=0;
  j=length-1;
  for (i=0;i<length;i++)
      { if ((i-j)>2)break;
        switch( *(data+i)) ///0123456.891   //j=7   012345678.1  j=9   0123456.891
           { case '0':
             case '1':
             case '2':
             case '3':
             case '4':
             case '5':
             case '6':
             case '7':
             case '8':
             case '9':*data1=(*(data+i)-0x30)+(*data1)*10;break;
             case 0x2e:j=i;break;
             default:*data1=(*data1)*10;break;
             }
        }
  k=i;
  for (i=0;i<3-(k-j);i++)
      { *data1=*data1*10;
        }
  }
void Asc_To_Int(unsigned char *data,unsigned char length,unsigned int *data1)
{ unsigned char i;
  *data1=0;
  for (i=0;i<length;i++)
      { switch( *(data+i))
           { case '0':
             case '1':
             case '2':
             case '3':
             case '4':
             case '5':
             case '6':
             case '7':
             case '8':
             case '9':*data1=(*(data+i)-0x30)+(*data1)*10;break;
             case 0x2e:
             default:*data1=(*data1)*10;break;
             }
        }
  }
void AscBin_To_LongInt(unsigned char *data,unsigned char length,unsigned long int *data1)
{ unsigned char i;
  *data1=0;
  for (i=0;i<length;i++)
      { switch( *(data+i))
           { case '0':
             case '1':
             case '2':
             case '3':
             case '4':
             case '5':
             case '6':
             case '7':
             case '8':
             case '9':*data1=(*(data+i)-0x30)+(*data1)*2;break;
             case 0x2e:
             default:*data1=(*data1)*2;break;
             }
        }
  }
void Asc_To_Char(unsigned char *data,unsigned char length,unsigned char *data1)
{ unsigned char i,j;
  *data1=0;
  j=length-1;
  for (i=0;i<length;i++)
      { if ((i-j)>2)break;
        switch( *(data+i))
           { case '0':
             case '1':
             case '2':
             case '3':
             case '4':
             case '5':
             case '6':
             case '7':
             case '8':
             case '9':*data1=(*(data+i)-0x30)+(*data1)*10;break;
             default:*data1=(*data1)*10;break;
             }
        }
  }
unsigned char Char_To_Asc(unsigned char *data,unsigned char *data1)
{ int i,j,k;
  j=100;
  k=0;
  for (i=0;i<3;i++)
      { if ((*data/j)!=0)
           { *(data1+k)=(*data)%(j*10)/j+0x30;
             k++;
             }
        j=j/10;
        }
  return k;
  }

void Char_To_BinAsc(unsigned char *data,unsigned char *length,unsigned char *data1)
{ 
  int i,j,k;
  *length=0;
  j=0x80;
  k=0;
  for (i=0;i<8;i++)
      { *(data1+k)=(*data)%(j<<1)/j+0x30;
        k++;
        j=j>>1;
        }
  *length=k;
}

unsigned char Int_To_Asc(unsigned int *data,unsigned char *data1)
{ 
	int i,j,k;
  j=10000;
  k=0;
	if(*data == 0)
		return 1;
  for (i=0;i<5;i++)
  { 
	 if ((*data/j)!=0)
   {
		*(data1+k)=(*data)%(j*10)/j+0x30;
    k++;
   }
   j=j/10;
   }
  return k;
}

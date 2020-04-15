/*
*********************************************************************************************************
*
*	模块名称 : 字符串操作\数值转换
*	文件名称 : bsp_user_lib.h
*	版    本 : V1.1
*	说    明 : 头文件
*
*********************************************************************************************************
*/

#ifndef __BSP_USER_LIB_H
#define __BSP_USER_LIB_H

int str_len(char *_str);
void str_cpy(char *_tar, char *_src);
int str_cmp(char * s1, char * s2);
void mem_set(char *_tar, char _data, int _len);

void int_to_str(int _iNumber, char *_pBuf, unsigned char _len);
int str_to_int(char *_pStr);

uint16_t BEBufToUint16(uint8_t *_pBuf);
uint16_t LEBufToUint16(uint8_t *_pBuf);

uint16_t CRC16_Modbus(uint8_t *_pBuf, uint16_t _usLen) ;
int32_t  CaculTwoPoint(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x);
unsigned short CRC16_CCITT(unsigned char *puchMsg, unsigned int usDataLen);
unsigned short CRC16_CCITT_FALSE(unsigned char *puchMsg, unsigned int usDataLen);
unsigned short CRC16_XMODEM(unsigned char *puchMsg, unsigned int usDataLen);
unsigned short CRC16_X25(unsigned char *puchMsg, unsigned int usDataLen);
unsigned short CRC16_MODBUS(unsigned char *puchMsg, unsigned int usDataLen);
unsigned short CRC16_IBM(unsigned char *puchMsg, unsigned int usDataLen);
unsigned short CRC16_MAXIM(unsigned char *puchMsg, unsigned int usDataLen);
unsigned short CRC16_USB(unsigned char *puchMsg, unsigned int usDataLen);
void InvertUint8(unsigned char *dBuf,unsigned char *srcBuf);
void InvertUint16(unsigned short *dBuf,unsigned short *srcBuf);
void InvertUint32(unsigned int *dBuf,unsigned int *srcBuf);
unsigned int CRC32(unsigned char *puchMsg, unsigned int usDataLen);
void LongInt_To_Asc(unsigned long int *data,unsigned char *length,unsigned char *data1);
void Asc_To_LongInt_2(unsigned char *data,unsigned char length,unsigned long int *data1);
void Asc_To_Int(unsigned char *data,unsigned char length,unsigned int *data1);
void AscBin_To_LongInt(unsigned char *data,unsigned char length,unsigned long int *data1);
void Asc_To_Char(unsigned char *data,unsigned char length,unsigned char *data1);
unsigned char Char_To_Asc(unsigned char *data,unsigned char *data1);
void Char_To_BinAsc(unsigned char *data,unsigned char *length,unsigned char *data1);
unsigned char Int_To_Asc(unsigned int *data,unsigned char *data1);

#endif


#ifndef _IAP_H
#define _IAP_H

void IAPData(void);
void IAP_SendCMD(u8 cmd);
void OpenFile(void);
void IAP_SendData(u8 Current_Sector);

typedef struct
{
//  u8 add;
//	u8 fuc;
//	u16 write_add;
//	u16 reg_num;
//	u8 write_len;
//	u8 write_buf[10];
//	u16 write_data;
//	
//	u8 Init_OK;
	u16  Rxd_TimeOut;
	char ErrCnt;
	u16  Current_Sector;
//	u8 Write_OK;
//	u16 Run_State;
//	s32 Read_Position;
//	u16 Run_Mode;
//	u8 Read_Data[10];  // 默认支持读10字节数据
//	
//	u8 Warning_Type;
//	u16 Warning_Cause;
//	u8 Origin_Returned;
//	u8 In_Origin;
//	u8 Run_Start;
//  u8 Arrive;
//	u8 Run_Done;
//	u32 Position[MODBUS_MAX_POSITOIN];
//	s16 Adjust;
//	u8 Next_Position;
//  u8 Current_Position;
//	
//	u16 delay_ms;
	
}_IAP_t;

extern _IAP_t IAP_t;

#endif

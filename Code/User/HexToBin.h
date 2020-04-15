#ifndef HEXTOBIN_H
#define HEXTOBIN_H
 
#define HEX_MAX_LENGTH		521
#define HEX_MIN_LEN		11
 
typedef enum {
	HEX_RES_OK = 0,		      //��ȷ
	HEX_RES_DATA_TOO_LONG,	//����̫��
	HEX_RES_DATA_TOO_SHORT,	//����̫��
	HEX_RES_NO_COLON,		    //�ޱ��
	HEX_RES_TYPE_ERROR,		  //���ͳ�����������
	HEX_RES_LENGTH_ERROR,	  //���ݳ����ֶκ����ݵ��ܳ��Ȳ���Ӧ
	HEX_RES_CHECK_ERROR,	  //У�����
	HEX_RES_HEX_FILE_NOT_EXIST,	//HEX�ļ�������
	HEX_RES_BIN_FILE_PATH_ERROR,//BIN�ļ�·�����ܲ���ȷ
	HEX_RES_WRITE_ERROR,	      //д���ݳ���
	HEX_RES_HEX_FILE_NO_END	    //hex�ļ�û�н�����
} RESULT_STATUS;
 
typedef struct {
	uint8_t len;
	uint8_t	type;
	uint16_t addr;
	uint8_t *data;
} BinFarmat;
 
void Test_Hex2Bin(void);



#endif

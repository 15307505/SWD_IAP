#include "bsp.h"

// 读取十六进制文件（main.hex），并将其按Hex格式规定，对不同格式域进行划分
// 将划分后的格式写入main.txt文件，根据Hex格式，对响应的Flash存储单元赋值
// 将Flash存储单元写入main_formated.txt中
// 请将main.hex放在改程序的统一目录下


#define FLASHVOLUM 1024   //Flash的容量，以K为单位
 
 
void FormatTran(char *dst,const char *src);
int HexToDec(char *src);
void TranHex(char *flashbuffer,char *formatcmd,int flashsize);
void PrintFlash(char *flashbuffer,int flashsize);
void WriteFlashToFile(char *flashbuffer,char *filename,int flashsize);
char *ExtraSubStr(const char *src,int startIndex,int len);

void Test_Hex2Bin(void)
{
//    FILE *fp;
//    FILE *resultfp;
//    int  flashsize=2*FLASHVOLUM*1024*sizeof(char);//flash的存储单元个数
//    char *buffer=malloc(sizeof(char)*100);//存储hex文件的一行内容
//    char *trsbuffer=malloc(sizeof(char)*200);//存储hex文件解析后的内容
//    char *flashbuffer=malloc(flashsize);//存储Flash中的内容
//    if(buffer==NULL||trsbuffer==NULL||flashbuffer==NULL){//申请空间失败
//        printf("Apply for memory failed.!\n");
//        exit(0);
//    }
//    memset(flashbuffer,'F',flashsize);//将Flash初始时全部清1
//    fp=fopen("main.hex","r");
//    resultfp=fopen("main.txt","w");
//    if(resultfp==NULL||fp==NULL){//打开文件失败
//        printf("Open file error.\n");
//        exit(0);
//    }
//    while(fscanf(fp,"%s",buffer)!=EOF){//读取文件所有内容
//        //HextoBin(trsbuffer,buffer);
//        printf("%s\n",buffer);//控制台显示
//        FormatTran(trsbuffer,buffer);
//        //printf("Length:%d\n",strlen(buffer));//显示字符串长度
//        fprintf(resultfp,"%s\n",trsbuffer);//写入文件
//        TranHex(flashbuffer,buffer,flashsize);
//    }
//    PrintFlash(flashbuffer,2*1024);//仅显示前2K的内容
//    WriteFlashToFile(flashbuffer,"main_formated.txt",2*1024);//将前2K的内容写入文件"main_formated.txt"中
//    //释放内存
//    free(buffer);
//    free(trsbuffer);
//    free(flashbuffer);
//    //关闭文件
//    fclose(fp);
//    fclose(resultfp);
 
 
/*  //HexToDex()函数测试代码
    char *src=malloc(sizeof(char)*5);
    int value;
    src[0]='2';src[1]='F';src[2]='1';src[3]='F';src[4]='\0';
    value=HexToDec(src);
    printf("The value of the hex string:%d",value);
*/

    //ExtraSubStr()函数测试代码
//    char *src=":1040000068120020654100006D4100006F41000012";
//    char *substr;
//    substr = ExtraSubStr(src,1,2);
//    printf("SubString:%s\n",substr);
//    printf("Change to Number:%d",HexToDec(substr));

    //ExtraSubStr()函数测试代码
    char *src=":1040000068120020654100006D4100006F41000012";
    char *substr;
		char *subcmd;
		TranHex(src,subcmd,8);
    substr = ExtraSubStr(src,1,2);
    printf("SubString:%s\n",substr);
    printf("Change to Number:%d",HexToDec(substr));
}
 
//按formatcmd字符串描述，以hex文件格式修改flashbuffer中的存储单元
//如 formatcmd=":03000000020030CB"
//则将flahsbuffer中以"0000"开始的连续"03"个内存单元，内容为"020030"
//flashsize:表示flash的存储单元个数
void TranHex(char *flashbuffer,char *formatcmd,int flashsize){
    
    char *eofstring=":00000001FF";
    int startadd;//formatcmd指示的起始地址
    int count;//formatcmd指示的数据单元个数
    int p,q;//索引指针
    if(strcmp(formatcmd,eofstring)==0){//结束标志
        printf("End of file.\n");
        return;
    }
    if(formatcmd[0]!=':'){//不是以":"开始
        printf("Format wrong!\n");
        return;
    }
    if(strlen(formatcmd)<11){//字符串长度小于11
        printf("Format wrong!\n");
        return;
    }
    count=HexToDec(ExtraSubStr(formatcmd,1,2));//获得数据单元长度
    startadd=2*HexToDec(ExtraSubStr(formatcmd,3,4));//获得起始地址
    if(strlen(formatcmd)<11+count*2){//字符串中数据损坏（丢失）
        printf("File has been damaged!\n");
        return;
    }
    for(p=0;p<count*2;p++){
        flashbuffer[startadd+p]=formatcmd[9+p];
    }
}

//格式解析，将一行Hex文件字符串src，按Hex文件组织格式
//划分出不同的块，块之间通过空格隔开
// ":llaaaatt[dd]cc"
// ":"行开始标志；"ll"数据长度；"aaaa"起始地址；"tt"数据类型；"dd"数据；"cc"校验
//解析后为：": ll aaaa tt [dd] cc" 
void FormatTran(char *dst,const char *src)
{

    int srclen=strlen(src);
    //printf("Length of src:%d",srclen);
    int p=0,q=0;//索引，p为src的索引，q为dst索引
    int i;//循环标志
    while(p<=srclen){//循环，对src按格式解析
        if(p==0){
            if(src[p]!=':'){//是否以":"开始
                printf("Format wrong! Please check the format of the file.\n");
                break;
            }
            else{
                dst[q++]=':';dst[q++]=' ';
                p++;continue;
            }
        }
        if(p==1){
            for(i=0;i<2;i++){//提取两位的"ll"
                dst[q++]=src[p++];
            }
            dst[q++]=' ';continue;
        }
        if(p==3){
            for(i=0;i<4;i++){//提取四位的"aaaa"
                dst[q++]=src[p++];
            }
            dst[q++]=' ';continue;
        }
        if(p==7){
            for(i=0;i<2;i++){//提取两位的"tt"
                dst[q++]=src[p++];
            }
            dst[q++]=' ';continue;
        }
        for(i=0;i<2;i++){//将dst的数据位以两个为一组，用空格隔开
                dst[q++]=src[p++];
        }
        dst[q++]=' ';
    }
    dst[q]='\0';//结束标志
}

//将src中的无符号十六进制字符串转为十进制数
//如src="001A"，则返回26
//字符串src需要以'\0'作为结束标志
int HexToDec(char *src){
    
    int value=0,srclen=strlen(src);
    int i;
    for(i=0;i<srclen;i++){
        if(src[i]<='9'&&src[i]>='0'){
            value=value*16+(int)(src[i]-'0');
        }
        else if(src[i]<='f'&&src[i]>='a'){
            value=value*16+(int)(src[i]-'a'+10);
        }else{
            value=value*16+(int)(src[i]-'A'+10);
        }
    }
    return value;//返回转换后的数值
}

//提取子串，从src中提取自startIndex开始的连续len个字节，构成子串 
char *ExtraSubStr(const char *src,int startIndex,int len)
{
    char *substr=malloc(sizeof(char)*(len+1));
    int i=0;
    if(substr==NULL){
        printf("Apply for memory failed.\n");
        return NULL;
    }
    if(strlen(src)<startIndex+len){
        printf("Index exceeds bounds.\n");
        return NULL;
    }
    for(i=0;i<len;i++){//提取子串
        substr[i]=src[startIndex+i];
    }
    substr[len]='\0';//添加字符串末尾标志
    return substr;
}

//flashbuffer:内存单元指针；flashsize：flash存储单元个数，即字节数
//输出Flash中的内容
void PrintFlash(char *flashbuffer,int flashsize){
   
    int rowlen=16;//每行16个字节
    int i,j;
    printf("Flash:%d\n",flashsize);
    for(i=0;i<flashsize;i+=2){
        if((i/2)%(rowlen)==0){
            printf("\n%06X\t",i/2);//内存单元地址输出格式：6位十六进制（A~F大写）
        }
        printf("%c%c ",flashbuffer[i],flashbuffer[i+1]);
    }
}

////将Flash的内容写入文件中
////flashbuffer:内存单元指针；filename:要写入的文件文件名；
////flashsize:写入的长度，以字节计
//void WriteFlashToFile(char *flashbuffer,char *filename,int flashsize){
//    
//    FILE *fp;
//    int i,rowlen=16;
//    if((fp=fopen(filename,"w"))==NULL){
//        printf("Open file error.\n");
//        exit(0);
//    }
//    //开始写入
//    for(i=0;i<flashsize;i+=2){
//        if((i/2)%(rowlen)==0){
//            if(i!=0){//不是第一行，遇到写满16个单元换行
//                fprintf(fp,"\n");
//            }
//            fprintf(fp,"%06X\t",i/2);//内存单元地址输出格式：6位十六进制（A~F大写）
//        }
//        fprintf(fp,"%c%c ",flashbuffer[i],flashbuffer[i+1]);
//    }
//    fclose(fp);
//}

//=============================================================
// 文件名称：	DS18B20.c
// 功能描述：	DS18B20驱动
// 维护记录：	2009-10-15	V1.0    by xgc
//=============================================================
#include "2440addr.h"
#include "2440lib.h"
#include "def.h"
#include "uart.h"
#include "DS18B20.h"

U16 temp;
U8 DS18B20_ID[8] = {0};
extern unsigned int PCLK;

void usDelay(int time)
{
	U32 i,j;
	for (i = time; i > 0; i--)
		for (j = 1; j > 0; j--);
}

//========================================
//函数名称：  void DS18B20_Init(void)
//实现功能：  初始化18B20
//入口参数:   无
//出口返回值: 无
//========================================
void DS18B20_Init(void)
{	
	DQ_OUT;
	DQ_L;
	usDelay(700);
	DQ_H;
	usDelay(4);
	DQ_IN;
	usDelay(100);
	
/*	if(DQ_STAT!=0)	//测试复位是否成功
		Uart_Printf("There are no 18B20 at GPB5! 0x%x\n");
	else
		Uart_Printf("Init 18B20 succeed!\n");
*/	
	usDelay(250);
}
//========================================
//函数名称：  U8 DS18B20_ReadBit(void)
//实现功能：  读取18b20的一位数据
//入口参数:   无
//出口返回值: 读取的一位数据
//========================================
U8 DS18B20_ReadBit(void)    
{
	U8 dat;
	DQ_OUT;
	DQ_L;
	usDelay(2);
	DQ_IN;
	usDelay(10);
	if(DQ_STAT!=0)
		dat=1;
	else 
		dat=0;
	usDelay(50);
	return (dat);
}
//========================================
//函数名称：  U8 DS18B20_ReadByte(void)
//实现功能：  读取一个字节的数据
//入口参数:   无
//出口返回值: 读取的一个字节数据
//========================================
U8 DS18B20_ReadByte(void)   
{
	U8 i,j,dat;
	dat=0;
	for(i=1;i<=8;i++)
	{
		j=DS18B20_ReadBit();
		dat=(j<<7)|(dat>>1);   //读出的数据最低位在最前面，这样刚好一个字节在DAT里
	}
	return(dat);
}

//========================================
//函数名称：  void DS18B20_WriteByte(U8 dat)
//实现功能：  向18B20写一个字节数据
//入口参数:   要写入的数据
//出口返回值: 无
//========================================
void DS18B20_WriteByte(U8 dat) 
{
	U8 j;
	U8 testb;
	DQ_OUT;
	for(j=1;j<=8;j++)
	{
		testb=dat&0x01;
		dat=dat>>1;
		if(testb)     
		{
			DQ_L;
			usDelay(8);
			DQ_H;
			usDelay(50);
		}
		else
		{
			DQ_L;
			usDelay(90);
			DQ_H;
			usDelay(8);
		}
	}
}

//========================================
//函数名称：  void DS18B20_ReadID(void)
//实现功能：  读取18B20的ID号
//入口参数:   无
//出口返回值: 无
//========================================
void DS18B20_ReadID(void)
{
    usDelay(1);
    DS18B20_Init();
    DS18B20_WriteByte(DS18B20_ReadROM);
    DS18B20_ID[0] = DS18B20_ReadByte();
    DS18B20_ID[1] = DS18B20_ReadByte();
    DS18B20_ID[2] = DS18B20_ReadByte();
    DS18B20_ID[3] = DS18B20_ReadByte();
    DS18B20_ID[4] = DS18B20_ReadByte();
    DS18B20_ID[5] = DS18B20_ReadByte();
    DS18B20_ID[6] = DS18B20_ReadByte();
    DS18B20_ID[7] = DS18B20_ReadByte();
}

//========================================
//函数名称：  void DS18B20_Match(void)
//实现功能：  匹配18B20
//入口参数:   无
//出口返回值: 无
//========================================
void DS18B20_Match(void)
{
    DS18B20_WriteByte(DS18B20_MatchROM);
    DS18B20_WriteByte(DS18B20_ID[0]);
    DS18B20_WriteByte(DS18B20_ID[1]);
    DS18B20_WriteByte(DS18B20_ID[2]);
    DS18B20_WriteByte(DS18B20_ID[3]);
    DS18B20_WriteByte(DS18B20_ID[4]);
    DS18B20_WriteByte(DS18B20_ID[5]);
    DS18B20_WriteByte(DS18B20_ID[6]);
    DS18B20_WriteByte(DS18B20_ID[7]);
}

//========================================
//函数名称：  void DS18B20_TmpChange(void)
//实现功能：  18B20开始温度转换
//入口参数:   无
//出口返回值: 无
//========================================
void DS18B20_TmpChange(void)
{
	DS18B20_Init();
	usDelay(2);/*delay(1);*/
	DS18B20_WriteByte(DS18B20_SkipROM);  // address all drivers on bus
	DS18B20_WriteByte(DS18B20_ConvertTemp);  //  initiates a single temperature conversion
}

//========================================
//函数名称：  U16 DS18B20_Temperature(void)
//实现功能：  得到18B20温度值
//入口参数:   无
//出口返回值: 温度值
//========================================
U16 DS18B20_Temperature(void)
{
	float tt;
	U8 a,b;
	DS18B20_Init();
	usDelay(4);/*delay(1);*/
	DS18B20_WriteByte(DS18B20_SkipROM);
	DS18B20_WriteByte(DS18B20_ReadSCR);
	a=DS18B20_ReadByte();
	b=DS18B20_ReadByte();
	temp=b;
	temp<<=8;             //two byte  compose a int variable
	temp=temp|a;
	tt=temp*0.0625;
	temp=tt*10+0.5;
	return temp;
}

//========================================
//函数名称：  void Test_DS18B20(void)
//实现功能：  18B20测试程序
//入口参数:   无
//出口返回值: 无
//========================================
void Test_DS18B20(void)
{
	U16 tmp;

	Uart_Printf("DS18B20 TEST!\n");
	Uart_Printf("Now start temperature change!\n");
	while(1)
	{
		DS18B20_TmpChange();
		Delay(1000);
		tmp =  DS18B20_Temperature();
		Uart_Printf("Temperature is  [%d.%d `C]\n", tmp/10, tmp%10);		
	}
}
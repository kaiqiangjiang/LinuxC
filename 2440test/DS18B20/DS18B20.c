//=============================================================
// �ļ����ƣ�	DS18B20.c
// ����������	DS18B20����
// ά����¼��	2009-10-15	V1.0    by xgc
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
//�������ƣ�  void DS18B20_Init(void)
//ʵ�ֹ��ܣ�  ��ʼ��18B20
//��ڲ���:   ��
//���ڷ���ֵ: ��
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
	
/*	if(DQ_STAT!=0)	//���Ը�λ�Ƿ�ɹ�
		Uart_Printf("There are no 18B20 at GPB5! 0x%x\n");
	else
		Uart_Printf("Init 18B20 succeed!\n");
*/	
	usDelay(250);
}
//========================================
//�������ƣ�  U8 DS18B20_ReadBit(void)
//ʵ�ֹ��ܣ�  ��ȡ18b20��һλ����
//��ڲ���:   ��
//���ڷ���ֵ: ��ȡ��һλ����
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
//�������ƣ�  U8 DS18B20_ReadByte(void)
//ʵ�ֹ��ܣ�  ��ȡһ���ֽڵ�����
//��ڲ���:   ��
//���ڷ���ֵ: ��ȡ��һ���ֽ�����
//========================================
U8 DS18B20_ReadByte(void)   
{
	U8 i,j,dat;
	dat=0;
	for(i=1;i<=8;i++)
	{
		j=DS18B20_ReadBit();
		dat=(j<<7)|(dat>>1);   //�������������λ����ǰ�棬�����պ�һ���ֽ���DAT��
	}
	return(dat);
}

//========================================
//�������ƣ�  void DS18B20_WriteByte(U8 dat)
//ʵ�ֹ��ܣ�  ��18B20дһ���ֽ�����
//��ڲ���:   Ҫд�������
//���ڷ���ֵ: ��
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
//�������ƣ�  void DS18B20_ReadID(void)
//ʵ�ֹ��ܣ�  ��ȡ18B20��ID��
//��ڲ���:   ��
//���ڷ���ֵ: ��
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
//�������ƣ�  void DS18B20_Match(void)
//ʵ�ֹ��ܣ�  ƥ��18B20
//��ڲ���:   ��
//���ڷ���ֵ: ��
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
//�������ƣ�  void DS18B20_TmpChange(void)
//ʵ�ֹ��ܣ�  18B20��ʼ�¶�ת��
//��ڲ���:   ��
//���ڷ���ֵ: ��
//========================================
void DS18B20_TmpChange(void)
{
	DS18B20_Init();
	usDelay(2);/*delay(1);*/
	DS18B20_WriteByte(DS18B20_SkipROM);  // address all drivers on bus
	DS18B20_WriteByte(DS18B20_ConvertTemp);  //  initiates a single temperature conversion
}

//========================================
//�������ƣ�  U16 DS18B20_Temperature(void)
//ʵ�ֹ��ܣ�  �õ�18B20�¶�ֵ
//��ڲ���:   ��
//���ڷ���ֵ: �¶�ֵ
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
//�������ƣ�  void Test_DS18B20(void)
//ʵ�ֹ��ܣ�  18B20���Գ���
//��ڲ���:   ��
//���ڷ���ֵ: ��
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
//======================================================================
//	�������ƣ�	LED.mcp
//	����������	��GPIO�����ĸ�LED
//  IDE������   ADS v1.2
//	����ļ���	main.c
//	Ӳ�����ӣ�	LED1 -- GPF0
//              LED2 -- GPF1
//              LED3 -- GPF2
//              LED4 -- GPF3
//	ά����¼��	2009-9-22	V1.0   by xgc 
//======================================================================

//=============================================================
// �ļ����ƣ�	main.c
// ����������	������������
// ά����¼��	2009-9-22	V1.0
//=============================================================

//====================================================
// ����ͷ�ļ���
//====================================================
#include "2440addr.h"
#include "2440lib.h"
#include "option.h"
#include "def.h"
#include "uart.h"


#define LED1ON    0xFE            //LED1����ֵΪ0xFE(�͵�ƽ����)
#define LED2ON    (LED1ON<<1)     //LED2����ֵΪLED1����1λ
#define LED3ON    (LED1ON<<2)     //LED3����ֵΪLED2����1λ
#define LED4ON    (LED1ON<<3)     //LED4����ֵΪLED3����1λ

extern unsigned int PCLK;

void usDelay(int time)
{
	U32 i,j;
	for (i = time; i > 0; i--)
		for (j = 50; j > 0; j--);
}

void Main(void)
{
	SetSysFclk(FCLK_400M);  //����ϵͳʱ�� 400M     
	ChangeClockDivider(2, 1);      //���÷�Ƶ 1��4��8
	CalcBusClk();           //��������Ƶ
	
	Uart_Select(0);
	Uart_Init(0, 115200);
	Uart_Printf("LED TEST!\n");
	
	rGPFCON = (rGPFCON | 0xFFFF) & 0xFFFFFF55;  //GPF0--GPF3����Ϊoutput       
   	rGPFUP  = rGPFUP & 0xFFF0;                  //ʹ��GPF��������
   	rGPFDAT = 0x0F;						  	 //GPF��4λ��ʼ��Ϊ1
   	while(1)
    {
    
       	Uart_Printf("LED1 ON!\n");      
    	rGPFDAT = LED1ON;                   //����LED1
       	Delay(1000);
       	
       	Uart_Printf("LED2 ON!\n");       
       	rGPFDAT = LED2ON;                   //����LED2
       	Delay(1000);

       	Uart_Printf("LED3 ON!\n");       
       	rGPFDAT = LED3ON;                   //����LED3
       	Delay(1000);

       	Uart_Printf("LED4 ON!\n");       
		rGPFDAT = LED4ON;                   //����LED4
        Delay(1000);           
    }
}
//======================================================================
//	�ļ����ƣ�	LED.c
//	����������	��GPIO�����ĸ�LED
//	Ӳ�����ӣ�	LED1 -- GPF0
//              LED2 -- GPF1
//              LED3 -- GPF2
//              LED4 -- GPF3
//	ά����¼��	2009-9-22	V1.0   by xgc 
//======================================================================

#include "2440addr.h"
#include "2440lib.h"
#include "option.h"
#include "def.h"
#include "uart.h"

#define LED1ON    0xFE            //LED1����ֵΪ0xFE(�͵�ƽ����)
#define LED2ON    (LED1ON<<1)     //LED2����ֵΪLED1����1λ
#define LED3ON    (LED1ON<<2)     //LED3����ֵΪLED2����1λ
#define LED4ON    (LED1ON<<3)     //LED4����ֵΪLED3����1λ

void Test_LED(void)
{
	int i;
	Uart_Printf("\nLED TEST!\n");
	
	rGPFCON = (rGPFCON | 0xFFFF) & 0xFFFFFF55;  //GPF0--GPF3����Ϊoutput       
   	rGPFUP  = rGPFUP & 0xFFF0;                  //ʹ��GPF��������
   	rGPFDAT = 0x0F;						  	 //GPF��4λ��ʼ��Ϊ1
   	for(i = 0; i < 3; i++)
    {
       	Uart_Printf("LED1 ON!\n");      
    	rGPFDAT = LED1ON;                   //����LED1
       	Delay(500);
       	
       	Uart_Printf("LED2 ON!\n");       
       	rGPFDAT = LED2ON;                   //����LED2
       	Delay(500);

       	Uart_Printf("LED3 ON!\n");       
       	rGPFDAT = LED3ON;                   //����LED3
       	Delay(500);

       	Uart_Printf("LED4 ON!\n");       
		rGPFDAT = LED4ON;                   //����LED4
       	Delay(500);           
    }
}
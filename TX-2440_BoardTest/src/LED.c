//======================================================================
//	文件名称：	LED.c
//	功能描述：	用GPIO控制四个LED
//	硬件连接：	LED1 -- GPF0
//              LED2 -- GPF1
//              LED3 -- GPF2
//              LED4 -- GPF3
//	维护记录：	2009-9-22	V1.0   by xgc 
//======================================================================

#include "2440addr.h"
#include "2440lib.h"
#include "option.h"
#include "def.h"
#include "uart.h"

#define LED1ON    0xFE            //LED1点亮值为0xFE(低电平点亮)
#define LED2ON    (LED1ON<<1)     //LED2点亮值为LED1左移1位
#define LED3ON    (LED1ON<<2)     //LED3点亮值为LED2左移1位
#define LED4ON    (LED1ON<<3)     //LED4点亮值为LED3左移1位

void Test_LED(void)
{
	int i;
	Uart_Printf("\nLED TEST!\n");
	
	rGPFCON = (rGPFCON | 0xFFFF) & 0xFFFFFF55;  //GPF0--GPF3设置为output       
   	rGPFUP  = rGPFUP & 0xFFF0;                  //使能GPF上拉电阻
   	rGPFDAT = 0x0F;						  	 //GPF低4位初始化为1
   	for(i = 0; i < 3; i++)
    {
       	Uart_Printf("LED1 ON!\n");      
    	rGPFDAT = LED1ON;                   //点亮LED1
       	Delay(500);
       	
       	Uart_Printf("LED2 ON!\n");       
       	rGPFDAT = LED2ON;                   //点亮LED2
       	Delay(500);

       	Uart_Printf("LED3 ON!\n");       
       	rGPFDAT = LED3ON;                   //点亮LED3
       	Delay(500);

       	Uart_Printf("LED4 ON!\n");       
		rGPFDAT = LED4ON;                   //点亮LED4
       	Delay(500);           
    }
}
//======================================================================
//	工程名称：	LED.mcp
//	功能描述：	用GPIO控制四个LED
//  IDE环境：   ADS v1.2
//	组成文件：	main.c
//	硬件连接：	LED1 -- GPF0
//              LED2 -- GPF1
//              LED3 -- GPF2
//              LED4 -- GPF3
//	维护记录：	2009-9-22	V1.0   by xgc 
//======================================================================

//=============================================================
// 文件名称：	main.c
// 功能描述：	定义了主函数
// 维护记录：	2009-9-22	V1.0
//=============================================================

//====================================================
// 包含头文件区
//====================================================
#include "2440addr.h"
#include "2440lib.h"
#include "option.h"
#include "def.h"
#include "uart.h"


#define LED1ON    0xFE            //LED1点亮值为0xFE(低电平点亮)
#define LED2ON    (LED1ON<<1)     //LED2点亮值为LED1左移1位
#define LED3ON    (LED1ON<<2)     //LED3点亮值为LED2左移1位
#define LED4ON    (LED1ON<<3)     //LED4点亮值为LED3左移1位

extern unsigned int PCLK;

void usDelay(int time)
{
	U32 i,j;
	for (i = time; i > 0; i--)
		for (j = 50; j > 0; j--);
}

void Main(void)
{
	SetSysFclk(FCLK_400M);  //设置系统时钟 400M     
	ChangeClockDivider(2, 1);      //设置分频 1：4：8
	CalcBusClk();           //计算总线频
	
	Uart_Select(0);
	Uart_Init(0, 115200);
	Uart_Printf("LED TEST!\n");
	
	rGPFCON = (rGPFCON | 0xFFFF) & 0xFFFFFF55;  //GPF0--GPF3设置为output       
   	rGPFUP  = rGPFUP & 0xFFF0;                  //使能GPF上拉电阻
   	rGPFDAT = 0x0F;						  	 //GPF低4位初始化为1
   	while(1)
    {
    
       	Uart_Printf("LED1 ON!\n");      
    	rGPFDAT = LED1ON;                   //点亮LED1
       	Delay(1000);
       	
       	Uart_Printf("LED2 ON!\n");       
       	rGPFDAT = LED2ON;                   //点亮LED2
       	Delay(1000);

       	Uart_Printf("LED3 ON!\n");       
       	rGPFDAT = LED3ON;                   //点亮LED3
       	Delay(1000);

       	Uart_Printf("LED4 ON!\n");       
		rGPFDAT = LED4ON;                   //点亮LED4
        Delay(1000);           
    }
}
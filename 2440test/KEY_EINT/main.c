//======================================================================
//	工程名称：	KEY_EINT
//	功能描述：	用外部中断4-7(4个按键)，控制四个LED的亮灭
//              采用下降沿触发
//  IDE环境：   ADS v1.2
//	涉及的库：	无
//	组成文件：	main.c
//	硬件连接：
//	维护记录：	2009-9-22	V1.0   by xgc
//======================================================================

#include "def.h" 
#include "2440addr.h"
#include "2440lib.h"
#include "string.h" 
#include "uart.h"

#define LED1ON    0xFE 
#define LED2ON    0xFD
#define LED3ON    0xFB
#define LED4ON    0xF7
#define LEDOFF    0xFF            //LED熄灭值为0xFF

void __irq EintHandler(void);

void Main(void)
{

	memcpy((unsigned char *)0x0,(unsigned char *)0x30000000,0x1000);
    
	SetSysFclk(FCLK_400M);  //设置系统时钟 400M     
	ChangeClockDivider(2, 1);      //设置分频 1：4：8
	CalcBusClk();           //计算总线频
	
	Uart_Select(0);
	Uart_Init(0,115200);
	Uart_Printf("KEY EINT TEST\n");
	Uart_Printf("Use Eint 4-7 falling edge\n"); 

    rGPFCON = (rGPFCON|0xFFFF)&0xFFFFaa55; 	//GPF4-7设置为EINT4-7,GPF0-3为输出
    rGPFUP &= 0xFF00;                     	//打开上拉功能
    rGPFDAT |= 0xF; 
 	
    rEXTINT0 &= ~(7<<16 | 7<<20 | 7<<24 | 7<<28);	
	rEXTINT0 |= (2<<16 | 2<<20 | 2<<24 | 2<<28) ;  //设置外部中断4_7下降沿触发
	
	rEINTPEND |= (1<<4|1<<5|1<<6|1<<7);							//clear eint 4
	rEINTMASK &= ~(1<<4|1<<5|1<<6|1<<7);						//enable eint 4
	ClearPending(BIT_EINT4_7);
  	pISR_EINT4_7=(unsigned)EintHandler; 	//外部中断4_7中断服务子程序入口地址
	EnableIrq(BIT_EINT4_7);	
	
    while(1);
     
 }
 
 //====================================================
// 语法格式： void __irq Eint0Handler(void)
// 功能描述: 点亮或者熄灭LED5-8  中断函数
// 入口参数: 无
// 出口参数: 无
//====================================================
 
void __irq EintHandler(void)
{
    
	if(rINTPND==BIT_EINT4_7)
	{
		
		ClearPending(BIT_EINT4_7);
		
		if(rEINTPEND&(1<<4))
		{
			Uart_Printf("eint 4\n");
			rGPFDAT = LED1ON;
			Delay(500);
			rGPFDAT = LEDOFF;
			rEINTPEND |= 1<< 4;
		}
		if(rEINTPEND&(1<<5))
		{
			Uart_Printf("eint 5\n");
			rGPFDAT = LED2ON;
			Delay(500);
			rGPFDAT = LEDOFF;
			rEINTPEND |= 1<< 5;
		}
		if(rEINTPEND&(1<<6))
		{
			Uart_Printf("eint 6\n");
			rGPFDAT = LED3ON;
			Delay(500);
			rGPFDAT = LEDOFF;
			rEINTPEND |= 1<< 6;
		}
		if(rEINTPEND&(1<<7))
		{
			Uart_Printf("eint 7\n");
			rGPFDAT = LED4ON;
			Delay(500);
			rGPFDAT = LEDOFF;
			rEINTPEND |= 1<< 7;
		}
	}

}
 
    
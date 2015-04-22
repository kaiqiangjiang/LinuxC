//======================================================================
//	工程名称：	TouchPanel.mcp
//	功能描述：	触摸笔按下的地方会有白色实心圆显示
//  IDE环境：   ADS1.2
//	组成文件：	main.c, 
//              TouchPanel.c   TouchPanel.h
//	硬件连接：	
//	维护记录：	2009-08-18	V1.0   by xgc
//				2009-10-28  V1.1   by xgc
//======================================================================
#include "TouchPanel.h"
#include "2440addr.h"
#include "2440lib.h"
#include "uart.h"
#include <string.h>

void Main()
{	
	memcpy((unsigned char *)0x0,(unsigned char *)0x30000000,0x1000);
    
	SetSysFclk(FCLK_400M);  //设置系统时钟 400M     
	ChangeClockDivider(2, 1);      //设置分频 1：4：8
	CalcBusClk();           //计算总线频
	
	Uart_Select(0);    
	Uart_Init(0, 115200); 
    Uart_Printf("\n ---触摸屏测试程序---\n");
	
	Test_TouchPanel();
}
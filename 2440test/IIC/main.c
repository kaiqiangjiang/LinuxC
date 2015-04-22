//======================================================================
//	工程名称：	IIC.mcp
//	功能描述：	通过超级终端可以看到IIC传输状态及其结果
//              
//  IDE环境：   ADS v1.2
//	组成文件：	main.c, IIC.c IIC.h 
//	硬件连接：	使用AT24C04
//	维护记录：	2009-8-26	V1.0   by xgc  
//======================================================================

#include "2440lib.h"
#include "2440addr.h"
#include "IIC.h"
#include "uart.h"
#include <string.h>

void Main(void)
{
 	memcpy((unsigned char *)0x0,(unsigned char *)0x30000000,0x1000);
 	
	SetSysFclk(FCLK_400M);         //设置系统时钟 400M
	ChangeClockDivider(2, 1);      //设置分频 1：4：8
	CalcBusClk();           //计算总线频

	Uart_Select(0);
	Uart_Init(0, 115200);	
	Test_Iic();
//	Test_Iic2();
	while(1);
}


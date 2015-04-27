//======================================================================
//	工程名称：	TFT.mcp
//	功能描述：	在LCD上画点，线，圆矩形，显示图片
//  IDE环境：   ADS1.2
//	组成文件：	main.c, 
//				TFT_API.c TFT_API.h TFT_Config.h
//				TFT_Driver.c TFT_Driver.h  TypeDef.h
//	硬件连接：	
//	维护记录：	2009-08-18	V1.0   by xgc
//              2009-10-28  V1.1   by xgc
//======================================================================
#include "TFT_API.h"
#include "2440addr.h"
#include "2440lib.h"
#include "uart.h"
#include <string.h>

void Main()
{
	memcpy((unsigned char *)0x0,(unsigned char *)0x30000000,0x1000);
	
	SetSysFclk(FCLK_400M);     //设置系统时钟 400M
	ChangeClockDivider(2, 1);  //设置分频 1：4：8
	CalcBusClk();              //计算总线频
	
	Uart_Select(0);
	Uart_Init(0, 115200);
	Uart_Printf("\nTFT TEST(320x240)\n");
	
	Test_TFT();
}
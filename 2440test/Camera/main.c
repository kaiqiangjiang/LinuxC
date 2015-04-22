//======================================================================
//	工程名称：	Camera.mcp
//	功能描述：	OV9650摄像头测试              
//  IDE环境：   ADS v1.2
//	组成文件：	camtest.c OV9650Test.c camproset.c camtest.h OV9650Test.h
//              camproset.h camdata.h camdef.h main.c TFT_API.c TFT_API.h 
//	硬件连接：	将摄像头模组插在CAMERA插座上，连接好LCD
//	维护记录：	2009-9-17	V1.0   by xgc  
//======================================================================

#include "2440lib.h"
#include "2440addr.h"
#include "camtest.h"
#include "TFT_API.h"
#include "uart.h"
#include <string.h>

void Main(void)
{
 	memcpy((unsigned char *)0x0,(unsigned char *)0x30000000,0x1000);
 	
	SetSysFclk(FCLK_400M);         //设置系统时钟 220M
	ChangeClockDivider(2, 1);      //设置分频 1：2：4
	CalcBusClk();
	
	Uart_Select(0);
	Uart_Init(0, 115200);
	TFT_Init();
	Camera_Test();
	while(1);
}
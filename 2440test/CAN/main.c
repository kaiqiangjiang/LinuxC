//======================================================================
//	工程名称：  CAN.mcp
//	功能描述：	测试CAN总线收发器MCP2510，用回环模式，自发自收
//  IDE环境：   ADS v1.2
//	涉及的库：	无
//	组成文件：	main.c	MCP2510.c  MCP2510.h
//	硬件连接：
//              MCP2510_CS      GPB7   ( nSS0 )   
//              MCP2510_SI      GPE12  ( SPIMOSI0 )   
//              MCP2510_SO      GPE11  ( SPIMISO0 )   
//              MCP2510_SCK     GPE13  ( SPICLK0 )   
//              MCP2510_INT     GPG0   ( EINT8 ) 	
//	维护记录：	2009-08-14	v1.0     by xgc
//				2009-10-28  v1.1     by xgc
//======================================================================

#include "2440addr.h"
#include "2440lib.h"
#include "MCP2510.h"
#include "uart.h"
#include <string.h>
extern unsigned int PCLK;
void Main(void)
{
 	memcpy((unsigned char *)0x0,(unsigned char *)0x30000000,0x1000);
 	
	SetSysFclk(FCLK_400M);         //设置系统时钟 400M
	ChangeClockDivider(2, 1);      //设置分频 1：4：8
	CalcBusClk();           //计算总线频

	Uart_Select(0);
	Uart_Init(0, 115200);	
	Test_MCP2510();
	while(1);	
}
//======================================================================
//	工程名称：	NandFlash
//	功能描述：	NandFlash基本操作，包括：读，写，擦除，检查坏块
//  IDE环境：   ADS v1.2
//	组成文件：	main.c, Nand.c Nand.h
//	硬件连接：	
//	维护记录：	2009-08-15	V1.0 by xgc
//              2009-10-14  V1.1 by xgc 支持大页Nand,K9F2G08(256M) 
//======================================================================

//=============================================================
// 文件名称：	main.c
// 功能描述：	定义了主函数
// 维护记录：	2009-08-15	V1.0 by xgc
//              2009-10-14  V1.1 by xgc
//=============================================================

//====================================================
// 包含头文件区
//====================================================
 
#include "2440addr.h"
#include "2440lib.h" 
#include "def.h"
#include "Nand.h"
#include "uart.h"
#include <string.h>
//====================================================
// 语法格式：void Main(void)
// 功能描述: K9F1208U0M nand flash 测试
// 入口参数: 无
// 出口参数: 无
//====================================================

void Main(void)
{
	memcpy((unsigned char *)0x0,(unsigned char *)0x30000000,0x1000);
	
	SetSysFclk(FCLK_400M);  //设置系统时钟 400M     
	ChangeClockDivider(2, 1);      //设置分频 1：4：8
	CalcBusClk();           //计算总线频
	
	Uart_Select(0);
	Uart_Init(0, 115200);
	
	Test_Nand();
	while(1);
}
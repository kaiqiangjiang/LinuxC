//======================================================================
//	�������ƣ�	TFT.mcp
//	����������	��LCD�ϻ��㣬�ߣ�Բ���Σ���ʾͼƬ
//  IDE������   ADS1.2
//	����ļ���	main.c, 
//				TFT_API.c TFT_API.h TFT_Config.h
//				TFT_Driver.c TFT_Driver.h  TypeDef.h
//	Ӳ�����ӣ�	
//	ά����¼��	2009-08-18	V1.0   by xgc
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
	
	SetSysFclk(FCLK_400M);     //����ϵͳʱ�� 400M
	ChangeClockDivider(2, 1);  //���÷�Ƶ 1��4��8
	CalcBusClk();              //��������Ƶ
	
	Uart_Select(0);
	Uart_Init(0, 115200);
	Uart_Printf("\nTFT TEST(320x240)\n");
	
	Test_TFT();
}
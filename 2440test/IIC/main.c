//======================================================================
//	�������ƣ�	IIC.mcp
//	����������	ͨ�������ն˿��Կ���IIC����״̬������
//              
//  IDE������   ADS v1.2
//	����ļ���	main.c, IIC.c IIC.h 
//	Ӳ�����ӣ�	ʹ��AT24C04
//	ά����¼��	2009-8-26	V1.0   by xgc  
//======================================================================

#include "2440lib.h"
#include "2440addr.h"
#include "IIC.h"
#include "uart.h"
#include <string.h>

void Main(void)
{
 	memcpy((unsigned char *)0x0,(unsigned char *)0x30000000,0x1000);
 	
	SetSysFclk(FCLK_400M);         //����ϵͳʱ�� 400M
	ChangeClockDivider(2, 1);      //���÷�Ƶ 1��4��8
	CalcBusClk();           //��������Ƶ

	Uart_Select(0);
	Uart_Init(0, 115200);	
	Test_Iic();
//	Test_Iic2();
	while(1);
}


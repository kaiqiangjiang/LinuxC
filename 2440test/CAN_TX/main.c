//======================================================================
//	�������ƣ�  CAN_TX.mcp
//	����������	����CAN�����շ���MCP2510�ķ��͹���
//              �����鿪�����ϵ�CAN�������ӣ����в���
//  IDE������   ADS v1.2
//	�漰�Ŀ⣺	��
//	����ļ���	main.c	MCP2510.c  MCP2510.h
//	Ӳ�����ӣ�
//              MCP2510_CS      GPB7   ( nSS0 )   
//              MCP2510_SI      GPE12  ( SPIMOSI0 )   
//              MCP2510_SO      GPE11  ( SPIMISO0 )   
//              MCP2510_SCK     GPE13  ( SPICLK0 )   
//              MCP2510_INT     GPG0   ( EINT8 ) 	
//	ά����¼��	2009-08-14	v1.0     by xgc
//				2009-10-28  v1.1     by xgc
//======================================================================

#include "2440addr.h"
#include "2440lib.h"
#include "MCP2510.h"
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
	CAN_Test_TX();
	while(1);	
}
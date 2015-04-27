//======================================================================
//	�������ƣ�	IIS.mcp
//	����������	UDA1341ʵ��¼����
//  IDE������   ADS v1.2
//  ����������  �ڳ����ն��ϣ���'1'��ѭ���������֣����������ͣ����
//              �ٰ�������������ţ���ESC�˳���
//              ��'1'������¼��ģʽ�����������ʼ¼����¼��������
//              �����������ʼѭ������¼������ESC�˳���
//	����ļ���	main.c, IIS.c IIS.h WAV.c
//	Ӳ�����ӣ�  ʹ��UDA1341	
//	ά����¼��	2009-8-31	V1.0   by xgc
//======================================================================

//=============================================================
// �ļ����ƣ�	main.c
// ����������	������������
// ά����¼��	2009-8-31	V1.0   by xgc
//=============================================================

#include "2440lib.h"
#include "2440addr.h"
#include "IIS.h"
#include "uart.h"
#include "UDA1341.h"
#include <string.h>

void Main(void)
{
	char Key;
	char m_flag = 0;

 	memcpy((unsigned char *)0x0,(unsigned char *)0x30000000,0x1000);
 	
	SetSysFclk(FCLK_220M);         //����ϵͳʱ�� 220M
	ChangeClockDivider(1, 1);      //���÷�Ƶ 1��2��4
	CalcBusClk();           //��������Ƶ
	
	Uart_Select(0);
	Uart_Init(0,115200);		 	//������0����������Ϊ115200

    Uart_Printf("\n\n To enter 1 means Play  , To enter 2 means Record!\n ") ;
    Uart_Printf("Please select: ") ;
	while(1)
	{
	
		Key = Uart_GetKey();
		switch(Key)
		{
	  		case '1':
	  				PlayMusicTest();
	  				m_flag = 1;
	  				break;
	  		case '2':
	  				RecordTest();
	  				m_flag = 1;
	  				break;
	  	  	default:
	  			break;
		}
		if( m_flag ==1 )
		{
		
			Uart_Printf("\n\n To enter 1 means Play  , To enter 2 means Record!\n ") ;
    		Uart_Printf("Please select: ") ;
			m_flag = 0;
		}
	}                      
}
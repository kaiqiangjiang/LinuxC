//======================================================================
//	�������ƣ�	KEY.mcp
//	����������	��������LED(��ѯ��)
//  IDE������   ADS v1.2
//	����ļ���	main.c
//	Ӳ�����ӣ�	KEY1 -- GPF4
//              KEY2 -- GPF5
//              KEY3 -- GPF6
//              KEY4 -- GPF7
//	ά����¼��	2009-9-22	V1.0   by xgc 
//======================================================================

//=============================================================
// �ļ����ƣ�	main.c
// ����������	������������
// ά����¼��	2009-9-22	V1.0
//=============================================================

//====================================================
// ����ͷ�ļ���
//====================================================
#include "2440addr.h"
#include "2440lib.h"
#include "option.h"
#include "def.h"
//#include "uart.h"

#define  KEY1     0xE0
#define  KEY2     0xD0
#define  KEY3     0xB0
#define  KEY4     0x70

#define LED1ON    0xFE 
#define LED2ON    0xFD
#define LED3ON    0xFB
#define LED4ON    0xF7

#define LEDOFF    0xFF            //LEDϨ��ֵΪ0xFF


unsigned int KeyScan(void)
{
	unsigned int uiData;
	unsigned int uiTemp;
	uiData = rGPFDAT;					//��ȡGPF�˿�����
	uiData = uiData&0xF0;				//��ȡGPF4~GPF7��Чֵ
	if(uiData!=0xF0)					//��0xF0���ʾ�м�����
	{
		Delay(10);						//��ʱ����
		uiTemp = rGPFDAT;
		uiTemp = uiTemp&0xF0;		    
		if(uiData == uiTemp)
			return uiData;              //���ؼ�ֵ	
	}
	return 0;	                  		 
}

void Main(void)
{
	unsigned int uiKey;
	SetSysFclk(FCLK_400M);  //����ϵͳʱ�� 400M     
	ChangeClockDivider(2, 1);      //���÷�Ƶ 1��4��8
	CalcBusClk();           //��������Ƶ
	
//	Uart_Select(0);
//	Uart_Init(0, 115200);
//	Uart_Printf("KEY TEST!\n");
	
	rGPFCON = (rGPFCON | 0xFFFF) & 0xFFFF0055;  //GPF0--GPF3����Ϊoutput,GPF4--GPF7����Ϊinput       
   	rGPFUP  = rGPFUP & 0xFF00;                  //ʹ��GPF��������
   	rGPFDAT = 0xFF;						  	 //GPF��ʼ��Ϊ1
   	while(1)
    {
    	uiKey = KeyScan();             		   //ȡ��ֵ
      	switch(uiKey)
      	{
      		case KEY1:					 	 //KEY1���£������LED1	
				rGPFDAT = LED1ON;
				Delay(500);
				rGPFDAT = LEDOFF;
				break;
			case KEY2:						//KEY2���£������LED2	
				rGPFDAT = LED2ON;
				Delay(500);
				rGPFDAT = LEDOFF;
				break;	
			case KEY3:						//KEY3���£������LED3
				rGPFDAT = LED3ON;
				Delay(500);
				rGPFDAT = LEDOFF;	  
		  		break;
		    case KEY4:						//KEY4���£������LED4
		   		rGPFDAT = LED4ON;
		   		Delay(500);
				rGPFDAT = LEDOFF;
		  		break;
		  	default:
		  		break;
      	}             
    }
}
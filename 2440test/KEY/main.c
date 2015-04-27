//======================================================================
//	工程名称：	KEY.mcp
//	功能描述：	按键控制LED(查询法)
//  IDE环境：   ADS v1.2
//	组成文件：	main.c
//	硬件连接：	KEY1 -- GPF4
//              KEY2 -- GPF5
//              KEY3 -- GPF6
//              KEY4 -- GPF7
//	维护记录：	2009-9-22	V1.0   by xgc 
//======================================================================

//=============================================================
// 文件名称：	main.c
// 功能描述：	定义了主函数
// 维护记录：	2009-9-22	V1.0
//=============================================================

//====================================================
// 包含头文件区
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

#define LEDOFF    0xFF            //LED熄灭值为0xFF


unsigned int KeyScan(void)
{
	unsigned int uiData;
	unsigned int uiTemp;
	uiData = rGPFDAT;					//读取GPF端口输入
	uiData = uiData&0xF0;				//仅取GPF4~GPF7有效值
	if(uiData!=0xF0)					//非0xF0则表示有键按下
	{
		Delay(10);						//延时消抖
		uiTemp = rGPFDAT;
		uiTemp = uiTemp&0xF0;		    
		if(uiData == uiTemp)
			return uiData;              //返回键值	
	}
	return 0;	                  		 
}

void Main(void)
{
	unsigned int uiKey;
	SetSysFclk(FCLK_400M);  //设置系统时钟 400M     
	ChangeClockDivider(2, 1);      //设置分频 1：4：8
	CalcBusClk();           //计算总线频
	
//	Uart_Select(0);
//	Uart_Init(0, 115200);
//	Uart_Printf("KEY TEST!\n");
	
	rGPFCON = (rGPFCON | 0xFFFF) & 0xFFFF0055;  //GPF0--GPF3设置为output,GPF4--GPF7设置为input       
   	rGPFUP  = rGPFUP & 0xFF00;                  //使能GPF上拉电阻
   	rGPFDAT = 0xFF;						  	 //GPF初始化为1
   	while(1)
    {
    	uiKey = KeyScan();             		   //取键值
      	switch(uiKey)
      	{
      		case KEY1:					 	 //KEY1按下，则点亮LED1	
				rGPFDAT = LED1ON;
				Delay(500);
				rGPFDAT = LEDOFF;
				break;
			case KEY2:						//KEY2按下，则点亮LED2	
				rGPFDAT = LED2ON;
				Delay(500);
				rGPFDAT = LEDOFF;
				break;	
			case KEY3:						//KEY3按下，则点亮LED3
				rGPFDAT = LED3ON;
				Delay(500);
				rGPFDAT = LEDOFF;	  
		  		break;
		    case KEY4:						//KEY4按下，则点亮LED4
		   		rGPFDAT = LED4ON;
		   		Delay(500);
				rGPFDAT = LEDOFF;
		  		break;
		  	default:
		  		break;
      	}             
    }
}
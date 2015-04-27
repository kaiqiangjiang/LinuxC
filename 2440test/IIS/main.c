//======================================================================
//	工程名称：	IIS.mcp
//	功能描述：	UDA1341实现录放音
//  IDE环境：   ADS v1.2
//  程序描述：  在超级终端上，按'1'：循环播放音乐，按任意键暂停播放
//              再按任意键继续播放，按ESC退出。
//              按'1'，进入录音模式，按任意键开始录音，录音结束后
//              按任意键，开始循环播放录音，按ESC退出。
//	组成文件：	main.c, IIS.c IIS.h WAV.c
//	硬件连接：  使用UDA1341	
//	维护记录：	2009-8-31	V1.0   by xgc
//======================================================================

//=============================================================
// 文件名称：	main.c
// 功能描述：	定义了主函数
// 维护记录：	2009-8-31	V1.0   by xgc
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
 	
	SetSysFclk(FCLK_220M);         //设置系统时钟 220M
	ChangeClockDivider(1, 1);      //设置分频 1：2：4
	CalcBusClk();           //计算总线频
	
	Uart_Select(0);
	Uart_Init(0,115200);		 	//将串口0波特率设置为115200

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
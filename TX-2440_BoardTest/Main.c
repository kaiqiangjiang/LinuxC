//======================================================================
//	工程名称：	TX-2440_BoardTest.mcp
//	功能描述：	TX-2440A 开发板整板测试程序
//              1. LED          2. KEY
//              3. ADC          4. BUZZER
//              5. RTC          6. IIC
//              7. IIS Play     8. IIS Record
//              9. TFT          10.TouchTanel
//              11.DS18B20      12.Camera
//              13.CAN Bus      14.IRDA_TX
//              15.IRDA_RX
//  IDE环境：   ADS1.2
//	维护记录：	2009-11-01   V1.0  by  xgc
//              2010-01-27   V1.1  by  xgc
//======================================================================


#include "def.h"
#include "option.h"
#include "2440addr.h"
#include "2440lib.h"
#include "2440slib.h"
#include "uart.h"
#include <stdlib.h>
#include <string.h>


extern void Test_LED(void);
extern void Test_KEY(void);
extern void Test_ADC(void);
extern void Test_PWM(void);
extern void Test_IIC(void);
extern void Test_RTC(void);
extern void PlayMusicTest(void);
extern void RecordTest(void);
extern void Test_TFT(void);
extern void Test_TouchPanel(void);
extern void Test_DS18B20(void);
extern void Camera_Test(void);
extern void Test_MCP2510(void);
extern void Test_IrDA_Tx(void);
extern void Test_IrDA_Rx(void);
extern void TFT_Init(void);


void Temp_function() { Uart_Printf("\nTX-2440A Board Test!!\n"); }

struct {
	void (*fun)(void);
	char *tip;
}CmdTip[] = 
{
	{ Temp_function, "Please input 1-16 to select test" } ,
	{ Test_LED, "LED" } ,
	{ Test_KEY, "KEY" } ,
	{ Test_ADC, "ADC" } ,
	{ Test_PWM, "BUZZER" } ,
	{ Test_IIC, "IIC EEPROM" } ,
	{ Test_RTC, "RTC Display" } ,
	{ PlayMusicTest, "IIS Play Music" } ,
	{ RecordTest, "IIS Record" } ,
	{ Test_TFT, "LCD Display" } ,
	{ Test_TouchPanel, "TouchPanel" } ,
	{ Test_DS18B20, "DS18B20"},
	{ Camera_Test, "CMOS Camera"},
	{ Test_MCP2510, "CAN Bus"},
	{ Test_IrDA_Tx, "IrDA TX"},
	{ Test_IrDA_Rx, "IrDA RX"},
	{ 0, 0}						
};

void Main(void)
{
	int i;
	memcpy((unsigned char *)0x0,(unsigned char *)0x30000000,0x1000);

	SetSysFclk(FCLK_400M);  //设置系统时钟 400M     
	ChangeClockDivider(2, 1);      //设置分频 1：4：8
	CalcBusClk();           //计算总线频

	Uart_Select(0);
    Uart_Init(0, 115200);
    
    TFT_Init();
    
    Uart_SendByte('\n');
	Uart_Printf("<***************************************>\n");
	Uart_Printf("          TX-2440A  整板测试程序\n");
	Uart_Printf("         按数字键选择要测试的项目\n");
	Uart_Printf("         按 ESC键退出当前测试项目\n");
	Uart_Printf("             WWW.TXMCU.COM\n");
	Uart_Printf("<***************************************>\n");
	
	while(1)
	{
		U8 idx;
		
		Uart_Printf("\nPlease select function : \n");	
		for(i=0; CmdTip[i].fun!=0; i++)
			Uart_Printf("%d : %s\n", i, CmdTip[i].tip);
		idx = Uart_GetIntNum_GJ() ;	
		if(idx<i)
		{
			(*CmdTip[idx].fun)();
			Delay(20);
			Uart_Init(0,115200 );
		}	
	
	}	
}
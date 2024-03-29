//=========================================================================
//  工程名称：	RTC.mcp
//  文件名称：  main.c
//  功能描述： 	通过超级终端可以看到每秒刷新的实时时钟，同时LED1按秒闪烁，
//              具有闹钟功能，闹钟叫醒时，LED2被点亮，同时超级终端有字符串提示。
//  组成文件：	main.c uart.c 2440lib.c 2440init.s 2440slib.s 
//    头文件：  2440addr.h def.h option.h 2440lib.h 2440slib.h uart.h
//  硬件连接：			
//  维护记录：	2009-08-14  v1.0	by xgc
//              2009-10-15  v1.1    by xgc
//=========================================================================

#include "2440addr.h"
#include "2440lib.h"
#include "def.h"
#include "uart.h"
#include <stdio.h>
#include <string.h>

//====================================================
// 常量定义区
//====================================================

void __irq RTC_tickHandler(void);
void __irq RTC_alarmHandler(void);

//====================================================
// 变量定义区
//====================================================
U8 led_index;               //用于秒刷新标识
U8 alarmflag = 0;            //闹钟报警标识

typedef struct ST_DATE{      //表示日期时间的数据结构
	short year;
	U8 month;
	U8 day;
	U8 week_day;
	U8 hour;
	U8 minute;
	U8 second;
	}st_date;
char *date[7] = { "SUN","MON", "TUES", "WED", "THURS","FRI", "SAT" } ; //星期几 

//====================================================
// 函数定义区
//====================================================
//====================================================
// 语法格式：void RTC_setdate(st_date *p_date)
// 功能描述: 设置实时时钟当前日期、 时间
// 入口参数: 设置日期时间
// 出口参数: 无
//====================================================
void RTC_setdate(st_date *p_date)
{
	rRTCCON = 0x01;	             //RTC读写使能，选择BCD时钟、计数器，无复位，1/32768
	rBCDYEAR = p_date->year;
	rBCDMON = p_date->month;
	rBCDDAY = p_date->day;
	rBCDDATE = p_date->week_day;
	rBCDHOUR = p_date->hour;
	rBCDMIN = p_date->minute;
	rBCDSEC = p_date->second;
	rRTCCON = 0x00;             //RTC读写禁止，选择BCD时钟、计数器，无复位，1/32768
}

//====================================================
// 语法格式：void RTC_getdate(st_date *p_date)
// 功能描述: 获取实时时钟当前日期、 时间
// 入口参数: 返回日期时间的指针
// 出口参数: 无
//====================================================
void RTC_getdate(st_date *p_date)
{
	rRTCCON = 0x01;             //RTC读写使能，选择BCD时钟、计数器，无复位，1/32768
	p_date->year = rBCDYEAR;
	p_date->month = rBCDMON;
	p_date->day = rBCDDAY;
	p_date->week_day = rBCDDATE;
	p_date->hour = rBCDHOUR;
	p_date->minute = rBCDMIN;
	p_date->second = rBCDSEC;
	rRTCCON = 0x00;             //RTC读写禁止，选择BCD时钟、计数器，无复位，1/32768
}

//====================================================
// 语法格式：void RTC_tickIRQ_init(U8 tick)
// 功能描述: TICK 中断初始化
// 入口参数: tick初始值
// 出口参数: 无
//====================================================

void RTC_tickIRQ_init(U8 tick)
{
   	pISR_TICK = (unsigned)RTC_tickHandler;	     //中断函数的入口地址
	EnableIrq(BIT_TICK);//open RTC TICK INTERRUPT
   	rRTCCON = 0x00;                               //屏蔽RTC读写
   	rTICNT = (tick&0x7f)|0x80;	//设置tick计数器 和中断使能   (1+n)/128
}

//====================================================
// 语法格式：void RTC_alarm_setdate(st_date *p_date,U8 mode)
// 功能描述: 设置闹钟日期、 时间
// 入口参数: 设置日期时间及其闹钟唤醒模式
// 出口参数: 无
//====================================================
void RTC_alarm_setdate(st_date *p_date,U8 mode)
{
	rRTCCON = 0x01;
	rALMYEAR = p_date->year;
	rALMMON = p_date->month;
	rALMDAY = p_date->day;	
	rALMHOUR = p_date->hour;
	rALMMIN = p_date->minute;
	rALMSEC = p_date->second;
	rRTCALM = mode;
	rRTCCON = 0x00;                
	pISR_RTC = (unsigned)RTC_alarmHandler;	
    EnableIrq(BIT_RTC); //open RTC alarm  INTERRUPT
}

//====================================================
// 语法格式：void Main(void)
// 功能描述: 设置实时时钟及闹钟时间，初始化RTC和alarm中断
// 入口参数: 
// 出口参数: 无
//====================================================

void Main(void)
{
    st_date m_date;
    U8 old_index;

	memcpy((unsigned char *)0x0,(unsigned char *)0x30000000,0x1000);
    
	SetSysFclk(FCLK_400M);  //设置系统时钟 400M     
	ChangeClockDivider(2, 1);      //设置分频 1：4：8
	CalcBusClk();           //计算总线频

		
	rGPFCON = (rGPFCON | 0xFFFF) & 0xFFFFFF55;  //GPF0--GPF3设置为output       
   	rGPFUP  = rGPFUP & 0xFFF0;                  //使能GPF上拉电阻
   	rGPFDAT = 0x0F;						  	 //GPF低4位初始化为1

	Uart_Select(0);    
	Uart_Init(0, 115200);   //设置端口 波特率115200  无数据流检测   数据位= 8位 
    
    Uart_Printf("\n ---实时时钟测试程序---\n");
   

                               //设置当前日期 时钟
    m_date.year =  0x09;    
    m_date.month = 0x10;
    m_date.day = 0x15;
    m_date.week_day = 0x04;
    m_date.hour = 0x13;
    m_date.minute = 0x32;
    m_date.second = 0x30;
    
    RTC_setdate(&m_date);     //设置实时时钟
    
    m_date.second = 0x10;     //修改alarm秒数
    
    RTC_alarm_setdate(&m_date,0x41);//设置告警的时间及方式，0x41表示使能RTC告警，以及使能秒时钟报警
    
    RTC_tickIRQ_init(127);          // 设置1秒钟tick一次
    
  
    while(1)
    {
    	if(old_index != led_index)
    	{   						   //秒是否刷新
    		RTC_getdate(&m_date);      //获取当前日期时间
    		m_date.year += 0x2000;   
    		old_index = led_index;     //更新标识
    		Uart_Printf("RTC TIME : %04x/%02x/%02x  %s - %02x:%02x:%02x\r",m_date.year,m_date.month,m_date.day,date[m_date.week_day],m_date.hour,m_date.minute,m_date.second);
    	}
    	
    	if(alarmflag)
    	{                			  //是否报警
    	 	alarmflag = 0;               //更新标识
    	 	Uart_Printf("\nRTC ALARM  %02x:%02x:%02x \n",m_date.hour,m_date.minute,m_date.second);
        }
    }
}

//=================================================================
// 语法格式：void __irq RTC_tickHandler(void)
// 功能描述: 实时时钟tick中断函数，更新index标识，设置端口（LED1闪烁）
// 入口参数: 
// 出口参数: 无
//==================================================================
void __irq RTC_tickHandler(void)
{
    rGPFDAT = ~(rGPFDAT^0xe);     //刷新LED1
    led_index ++;
    ClearPending(BIT_TICK);
} 

//====================================================
// 语法格式：void __irq RTC_alarmHandler(void)
// 功能描述: alarm中断函数，设置端口(LED2被点亮）
// 入口参数: 
// 出口参数: 无
//==================================================== 
void __irq RTC_alarmHandler(void)
{
   rGPFDAT = (rGPFDAT^0x2);      //刷新LED2
   alarmflag = 1;
   ClearPending(BIT_RTC);
}

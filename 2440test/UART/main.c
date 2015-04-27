//=========================================================================
//  工程名称：	UART.mcp
//  文件名称：  main.c
//  功能描述： 	通过超级终端完成PC和S3C2440的数据传输,利用超级终端输入需要发送的字符，回车后，字符会再发送回来显示。
//  组成文件：	main.c 2440lib.c 2440init.s 2440slib.s 
//    头文件：  2440addr.h def.h option.h 2440lib.h 2440slib.h
//  程序分析：	采用宏定义的方法实现LED循环点亮
//  硬件连接：	用串口线将开发板和PC机串口相连		
//  维护记录：	2009-8-14 v1.0		by xgc
//=========================================================================

#include "2440addr.h"
#include "2440lib.h"
#include "2440slib.h"
#include "option.h"

#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

//====================================================
// 函数定义区
//====================================================
extern void Delay(int time);
void myUart_Select(int ch);
void myUart_Init(int whichuart, int baud);
void myUart_SendByte(char ch);
char myUart_ReceiveByte(void);
void myUart_Send (char *str);
void myUart_receive(char *string);
void myUart_Printf(char *fmt,...);

extern unsigned int PCLK;
static int UartNum=0;
char *string;

//====================================================
// 语法格式：int Main（void）
// 功能描述: 发送并接收字符串，测试UART通信
// 入口参数: 字符串指针
// 出口参数: 无
//======================================================================

int Main(void)
{    
 
    SetSysFclk(FCLK_400M);
    ChangeClockDivider(2, 1);
	CalcBusClk();

	myUart_Select(1);    
    myUart_Init(0,115200);
	while(1)
	{	
	    myUart_Printf("Please Input a string:\n");
	    myUart_receive(string);   
	    Delay(50);
	    myUart_Send(string);
	    myUart_Send("\n");
    }	
}

void myUart_Select(int ch)
{
    UartNum = ch;
}

//====================================================
// 语法格式：void myUart_Init(int whichuart, int baud)
// 功能描述: 对Uart进行初始化，以所需要的波特率为输入参数
// 入口参数: UART端口号  波特率
// 出口参数: 无
//======================================================================

void myUart_Init(int pclk, int baud)
{
    if (pclk == 0)
    	pclk = PCLK;
    if(UartNum == 0)        //判断是否使用UART0     
    {         
		rGPHCON = rGPHCON & (~(0xffff));   //UART0: RXD0<==>GPH3  TXD0<==>GPH2
		rGPHCON = rGPHCON | (0xaaa0) ;     //设置GPH端口为UART口
    	rGPHUP  = 0x0;                     //使能上拉功能
	
		rUFCON0=0x00;   // 不使用FIFO  
        rUMCON0=0x00;   //不使用自动流控制
        rULCON0=0x03;   //不采用红外线传输模式，无奇偶校验位，1个停止位，8个数据位
        rUCON0=0x245;   //发送中断为电平方式，接收中断为边沿方式，禁止超时中断，允许产生错误状态中断，禁止回送模式，禁止中止		
                        //信号，传输模式为中断请求模式，接收模式也为中断请求模式。
        rUBRDIV0=( (int)(pclk/16./baud+0.5) -1 );  //根据波特率计算UBRDIV0的值
	 	Delay(10);
     }
     else if(UartNum == 1)
     {
		rGPHCON = rGPHCON & (~(0xffff)) ; //UART1: RXD1<==>GPH5  TXD1<==>GPH4
		rGPHCON = rGPHCON | (0xaaa0) ;    //设置GPH端口为UART口
    	rGPHUP  = 0x0;                    // 使能上拉功能
	
		rUFCON1=0x0;    
        rUMCON1=0x0;   
        rULCON1=0x3; 
        rUCON1=0x245; 
        rUBRDIV1=( (int)(pclk/16./baud+0.5) -1 ); 
        Delay(10);
     }
}

//====================================================
// 语法格式：void myUart_SendByte(char ch)
// 功能描述: 发送字节数据
// 入口参数: 发送的字节数据       
// 出口参数: 无
//====================================================================

void myUart_SendByte(char ch)
{
	if (UartNum ==0)
    {
		if(ch=='\n')
		{
		    while(!(rUTRSTAT0 & 0x2));  //等待，直到发送缓冲区为空
//		    Delay(10);	            //超级中断的响应速度较慢 
		    WrUTXH0('\r');              //发送回车符
		}
		while(!(rUTRSTAT0 & 0x2));      //等待，直到发送缓冲区为空
//		Delay(10);
		WrUTXH0(ch);                    //发送字符
   	}
	else
    {
		if(ch=='\n')
	    {
		    while(!(rUTRSTAT1 & 0x2));   //等待，直到发送缓冲区为空
//		    Delay(10);	                 //等待
		    rUTXH1='\r';
	    }
	    while(!(rUTRSTAT1 & 0x2));  //Wait until THR is empty.
//	    Delay(10);
	    WrUTXH1(ch);
    }	
}

//====================================================
// 语法格式：char myUart_ReceiveByte(void)
// 功能描述: 接收字节数据
// 入口参数: 无
// 出口参数: 接收的字节数据
//====================================================================

char myUart_ReceiveByte(void)
{
    if(UartNum==0)
    {       
        while(!(rUTRSTAT0 & 0x1)); //等待接收数据
        return RdURXH0();
    }
    else if(UartNum==1)
    {       
        while(!(rUTRSTAT1 & 0x1)); //等待接收数据
        return RdURXH1();
    }
    return 0;
}

//====================================================
// 语法格式：void myUart_Send (char *str)
// 功能描述: 发送字符串
// 入口参数: 字符串指针
// 出口参数: 无
//====================================================================
void myUart_Send (char *str)
{
    myUart_Init(0,115200);
	while (*str)
	myUart_SendByte(*str++);
}	


//====================================================
// 语法格式：void myUart_receive(char *string)
// 功能描述: 接收字符串
// 入口参数: 字符串指针
// 出口参数: 无
//===================================================================
void myUart_receive(char *string)
{
	 char *string2 ;
     char c;
     string2 = string;
     myUart_Init(0,115200);
     while((c = myUart_ReceiveByte())!='\r')
     {
        if(c=='\b')
        {
            if( (int)string2 < (int)string )
            {
                printf("\b \b");
                string--;
            }
        }
        else 
        {
            *string++ = c;            
            myUart_SendByte(c);
        }
     }
     *string='\0';
     myUart_SendByte('\n');    
     
}

void myUart_Printf(char *fmt,...)
{
    va_list ap;
    char string[256];

    va_start(ap,fmt);
    vsprintf(string,fmt,ap);
    myUart_Send(string);
    va_end(ap);
}

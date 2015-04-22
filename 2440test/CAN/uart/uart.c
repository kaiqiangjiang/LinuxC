//=============================================================
// 文件名称：	uart.c
// 功能描述：	UART相关函数
// 维护记录：	2009-8-14	V1.0    by xgc
//=============================================================

#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#include "option.h"
#include "2440addr.h"  
#include "2440lib.h"

extern unsigned int PCLK;
static int UartNum=0;
char *string;


void Uart_Select(int ch)
{
    UartNum = ch;
}

//====================================================
// 语法格式：void Uart_Init(int whichuart, int baud)
// 功能描述: 对Uart进行初始化，以所需要的波特率为输入参数
// 入口参数: UART端口号  波特率
// 出口参数: 无
//======================================================================
void Uart_Init(int pclk, int baud)
{
   if(pclk == 0)
       pclk = PCLK;

   if(UartNum==0)        //判断是否使用UART0     
   {
   		rGPHCON = rGPHCON & (~(0xffff));   //UART0: RXD0<==>GPH3  TXD0<==>GPH2
		rGPHCON = rGPHCON | (0xaaa0) ;     //设置GPH端口为UART口
    	rGPHUP  = 0x0;                     //使能上拉功能
	
		rUFCON0=0x0;   // 不使用FIFO  
        rUMCON0=0x0;   //不使用自动流控制
        rULCON0=0x3;   //不采用红外线传输模式，无奇偶校验位，1个停止位，8个数据位
        rUCON0=0x245;   //发送中断为电平方式，接收中断为边沿方式，禁止超时中断，允许产生错误状态中断，禁止回送模式，禁止中止		
                        //信号，传输模式为中断请求模式，接收模式也为中断请求模式。
    	rUBRDIV0=( (int)(pclk/16./baud+0.5) -1 ); //根据波特率计算UBRDIV0的值
	 	Delay(10);

    }
    else if(UartNum==1)
    {        
		rGPHCON = rGPHCON & (~(0xffff)) ; //UART1: RXD1<==>GPH5  TXD1<==>GPH4
		rGPHCON = rGPHCON | (0xaaa0) ;    //设置GPH端口为UART口
    	rGPHUP  = 0x0;                    // 使能上拉功能
	
		rUFCON1=0x0;    
        rUMCON1=0x0;   
        rULCON1=0x3; 
        rUCON1=0x245; 
        rUBRDIV1=((int)(pclk/(baud*16))-1);
        Delay(10);
    }
    else if(UartNum==2)
    {        
		rGPHCON = rGPHCON & (~(0xffff)) ; //UART1: RXD2<==>GPH7  TXD2<==>GPH6
		rGPHCON = rGPHCON | (0xaaa0) ;    //设置GPH端口为UART口
    	rGPHUP  = 0x0;                    // 使能上拉功能
	
		rUFCON2=0x0;    
        rUMCON2=0x0;   
        rULCON2=0x3; 
        rUCON2=0x245; 
        rUBRDIV2=((int)(pclk/(baud*16))-1);
        Delay(10);
    }
}


//====================================================
// 语法格式：void Uart_TxEmpty(int ch)
// 功能描述: 
// 入口参数: 串口号
// 出口参数: 无
//====================================================================
void Uart_TxEmpty(int ch)
{
    if(ch==0)
        while(!(rUTRSTAT0 & 0x4)); //等待发送缓冲区为空
          
    else if(ch==1)
        while(!(rUTRSTAT1 & 0x4));
        
    else if(ch==2)
        while(!(rUTRSTAT2 & 0x4));
}


//====================================================
// 语法格式：void Uart_SendByte(char ch)
// 功能描述: 发送字节数据
// 入口参数: 发送的字节数据       
// 出口参数: 无
//====================================================================
void Uart_SendByte(char ch)
{
	if (UartNum == 0)
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
	else if (UartNum == 1)
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
    else if (UartNum == 2)
    {
		if(ch=='\n')
	    {
		    while(!(rUTRSTAT2 & 0x2));   //等待，直到发送缓冲区为空
//		    Delay(10);	                 //等待
		    rUTXH2='\r';
	    }
	    while(!(rUTRSTAT2 & 0x2));  //Wait until THR is empty.
//	    Delay(10);
	    WrUTXH2(ch);
    }		
}


//====================================================
// 语法格式：char Uart_ReceiveByte(void)
// 功能描述: 接收字节数据
// 入口参数: 无
// 出口参数: 接收的字节数据
//====================================================================
char Uart_ReceiveByte(void)
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
    else if(UartNum==2)
    {
        while(!(rUTRSTAT2 & 0x1));
        return RdURXH2();
    }
    return 0;
}


//====================================================
// 语法格式：char Uart_Getch(void)
// 功能描述: 接收一个字符，和Uart_ReceiveByte一样
// 入口参数: 无
// 出口参数: 接收的字节数据
//====================================================================
char Uart_Getch(void)
{
    if(UartNum==0)
    {       
        while(!(rUTRSTAT0 & 0x1));
        return RdURXH0();
    }
    else if(UartNum==1)
    {       
        while(!(rUTRSTAT1 & 0x1));
        return RdURXH1();
    }
    else if(UartNum==2)
    {
        while(!(rUTRSTAT2 & 0x1));
        return RdURXH2();
    }
    
    return 0 ;
}


//====================================================
// 语法格式：char Uart_GetKey(void)
// 功能描述: 得到键值，和Uart_ReceiveByte一样
// 入口参数: 无
// 出口参数: 接收的字节数据
//====================================================================
char Uart_GetKey(void)
{
    if(UartNum==0)
    {       
        if(rUTRSTAT0 & 0x1)    //Receive data ready
            return RdURXH0();
        else
            return 0;
    }
    else if(UartNum==1)
    {
        if(rUTRSTAT1 & 0x1)    //Receive data ready
            return RdURXH1();
        else
            return 0;
    }
    else if(UartNum==2)
    {       
        if(rUTRSTAT2 & 0x1)    //Receive data ready
            return RdURXH2();
        else
            return 0;
    }    

	return 0 ;
}


//====================================================
// 语法格式：void Uart_Send (char *str)
// 功能描述: 发送字符串
// 入口参数: 字符串指针
// 出口参数: 无
//====================================================================
void Uart_Send (char *str)
{
	while (*str)
		Uart_SendByte(*str++);
}	


//====================================================
// 语法格式：void Uart_receive(char *string)
// 功能描述: 接收字符串
// 入口参数: 字符串指针
// 出口参数: 无
//===================================================================
void Uart_receive(char *string)
{
	 char *string2 ;
     char c;
     string2 = string;
     while((c = Uart_ReceiveByte())!='\r')
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
            Uart_SendByte(c);
        }
     }
     *string='\0';
     Uart_SendByte('\n');    
     
}


//====================================================
// 语法格式：int Uart_GetIntNum(void)
// 功能描述: 得到整型数
// 入口参数: 无
// 出口参数: 整型数
//====================================================================
int Uart_GetIntNum(void)
{
    char str[30];
    char *string = str;
    int base     = 10;
    int minus    = 0;
    int result   = 0;
    int lastIndex;    
    int i;
    
   	Uart_receive(string);
    
    if(string[0]=='-')
    {
        minus = 1;
        string++;
    }
    
    if(string[0]=='0' && (string[1]=='x' || string[1]=='X'))
    {
        base    = 16;
        string += 2;
    }
    
    lastIndex = strlen(string) - 1;
    
    if(lastIndex<0)
        return -1;
    
    if(string[lastIndex]=='h' || string[lastIndex]=='H' )
    {
        base = 16;
        string[lastIndex] = 0;
        lastIndex--;
    }

    if(base==10)
    {
        result = atoi(string);
        result = minus ? (-1*result):result;
    }
    else
    {
        for(i=0;i<=lastIndex;i++)
        {
            if(isalpha(string[i]))
            {
                if(isupper(string[i]))
                    result = (result<<4) + string[i] - 'A' + 10;
                else
                    result = (result<<4) + string[i] - 'a' + 10;
            }
            else
                result = (result<<4) + string[i] - '0';
        }
        result = minus ? (-1*result):result;
    }
    return result;
}

int Uart_GetIntNum_GJ(void)
{
    char string[16] ;
    char *p_string = string ;
    char c;
    int i = 0 ;
    int data = 0 ;

    while(   ( c = Uart_Getch()) != '\r'  )
    {
		if(c=='\b')		p_string--;
		else		*p_string++=c;
		
		Uart_SendByte( c ) ;
    }

    *p_string = '\0';

	i = 0 ;
	while( string[i] != '\0' )
	{
		data = data * 10 ;
		if( string[i]<'0'||string[i]>'9' )
			return -1 ;
		data = data + ( string[i]-'0' ) ;
		i++ ;		
	}	
	
	return data ;
}


//====================================================
// 语法格式：void Uart_Printf(char *fmt,...)
// 功能描述: 按格式输出字符串
// 入口参数: printf格式的字符串
// 出口参数: 无
//===================================================================

void Uart_Printf(char *fmt,...)
{
    va_list ap;
    char string[256];

    va_start(ap,fmt);
    vsprintf(string,fmt,ap);
    Uart_Send(string);
    va_end(ap);
}
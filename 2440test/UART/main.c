//=========================================================================
//  �������ƣ�	UART.mcp
//  �ļ����ƣ�  main.c
//  ���������� 	ͨ�������ն����PC��S3C2440�����ݴ���,���ó����ն�������Ҫ���͵��ַ����س����ַ����ٷ��ͻ�����ʾ��
//  ����ļ���	main.c 2440lib.c 2440init.s 2440slib.s 
//    ͷ�ļ���  2440addr.h def.h option.h 2440lib.h 2440slib.h
//  ���������	���ú궨��ķ���ʵ��LEDѭ������
//  Ӳ�����ӣ�	�ô����߽��������PC����������		
//  ά����¼��	2009-8-14 v1.0		by xgc
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
// ����������
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
// �﷨��ʽ��int Main��void��
// ��������: ���Ͳ������ַ���������UARTͨ��
// ��ڲ���: �ַ���ָ��
// ���ڲ���: ��
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
// �﷨��ʽ��void myUart_Init(int whichuart, int baud)
// ��������: ��Uart���г�ʼ����������Ҫ�Ĳ�����Ϊ�������
// ��ڲ���: UART�˿ں�  ������
// ���ڲ���: ��
//======================================================================

void myUart_Init(int pclk, int baud)
{
    if (pclk == 0)
    	pclk = PCLK;
    if(UartNum == 0)        //�ж��Ƿ�ʹ��UART0     
    {         
		rGPHCON = rGPHCON & (~(0xffff));   //UART0: RXD0<==>GPH3  TXD0<==>GPH2
		rGPHCON = rGPHCON | (0xaaa0) ;     //����GPH�˿�ΪUART��
    	rGPHUP  = 0x0;                     //ʹ����������
	
		rUFCON0=0x00;   // ��ʹ��FIFO  
        rUMCON0=0x00;   //��ʹ���Զ�������
        rULCON0=0x03;   //�����ú����ߴ���ģʽ������żУ��λ��1��ֹͣλ��8������λ
        rUCON0=0x245;   //�����ж�Ϊ��ƽ��ʽ�������ж�Ϊ���ط�ʽ����ֹ��ʱ�жϣ������������״̬�жϣ���ֹ����ģʽ����ֹ��ֹ		
                        //�źţ�����ģʽΪ�ж�����ģʽ������ģʽҲΪ�ж�����ģʽ��
        rUBRDIV0=( (int)(pclk/16./baud+0.5) -1 );  //���ݲ����ʼ���UBRDIV0��ֵ
	 	Delay(10);
     }
     else if(UartNum == 1)
     {
		rGPHCON = rGPHCON & (~(0xffff)) ; //UART1: RXD1<==>GPH5  TXD1<==>GPH4
		rGPHCON = rGPHCON | (0xaaa0) ;    //����GPH�˿�ΪUART��
    	rGPHUP  = 0x0;                    // ʹ����������
	
		rUFCON1=0x0;    
        rUMCON1=0x0;   
        rULCON1=0x3; 
        rUCON1=0x245; 
        rUBRDIV1=( (int)(pclk/16./baud+0.5) -1 ); 
        Delay(10);
     }
}

//====================================================
// �﷨��ʽ��void myUart_SendByte(char ch)
// ��������: �����ֽ�����
// ��ڲ���: ���͵��ֽ�����       
// ���ڲ���: ��
//====================================================================

void myUart_SendByte(char ch)
{
	if (UartNum ==0)
    {
		if(ch=='\n')
		{
		    while(!(rUTRSTAT0 & 0x2));  //�ȴ���ֱ�����ͻ�����Ϊ��
//		    Delay(10);	            //�����жϵ���Ӧ�ٶȽ��� 
		    WrUTXH0('\r');              //���ͻس���
		}
		while(!(rUTRSTAT0 & 0x2));      //�ȴ���ֱ�����ͻ�����Ϊ��
//		Delay(10);
		WrUTXH0(ch);                    //�����ַ�
   	}
	else
    {
		if(ch=='\n')
	    {
		    while(!(rUTRSTAT1 & 0x2));   //�ȴ���ֱ�����ͻ�����Ϊ��
//		    Delay(10);	                 //�ȴ�
		    rUTXH1='\r';
	    }
	    while(!(rUTRSTAT1 & 0x2));  //Wait until THR is empty.
//	    Delay(10);
	    WrUTXH1(ch);
    }	
}

//====================================================
// �﷨��ʽ��char myUart_ReceiveByte(void)
// ��������: �����ֽ�����
// ��ڲ���: ��
// ���ڲ���: ���յ��ֽ�����
//====================================================================

char myUart_ReceiveByte(void)
{
    if(UartNum==0)
    {       
        while(!(rUTRSTAT0 & 0x1)); //�ȴ���������
        return RdURXH0();
    }
    else if(UartNum==1)
    {       
        while(!(rUTRSTAT1 & 0x1)); //�ȴ���������
        return RdURXH1();
    }
    return 0;
}

//====================================================
// �﷨��ʽ��void myUart_Send (char *str)
// ��������: �����ַ���
// ��ڲ���: �ַ���ָ��
// ���ڲ���: ��
//====================================================================
void myUart_Send (char *str)
{
    myUart_Init(0,115200);
	while (*str)
	myUart_SendByte(*str++);
}	


//====================================================
// �﷨��ʽ��void myUart_receive(char *string)
// ��������: �����ַ���
// ��ڲ���: �ַ���ָ��
// ���ڲ���: ��
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

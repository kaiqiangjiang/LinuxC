//=============================================================
// �ļ����ƣ�	uart.c
// ����������	UART��غ���
// ά����¼��	2009-8-14	V1.0    by xgc
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
// �﷨��ʽ��void Uart_Init(int whichuart, int baud)
// ��������: ��Uart���г�ʼ����������Ҫ�Ĳ�����Ϊ�������
// ��ڲ���: UART�˿ں�  ������
// ���ڲ���: ��
//======================================================================
void Uart_Init(int pclk, int baud)
{
   if(pclk == 0)
       pclk = PCLK;

   if(UartNum==0)        //�ж��Ƿ�ʹ��UART0     
   {
   		rGPHCON = rGPHCON & (~(0xffff));   //UART0: RXD0<==>GPH3  TXD0<==>GPH2
		rGPHCON = rGPHCON | (0xaaa0) ;     //����GPH�˿�ΪUART��
    	rGPHUP  = 0x0;                     //ʹ����������
	
		rUFCON0=0x0;   // ��ʹ��FIFO  
        rUMCON0=0x0;   //��ʹ���Զ�������
        rULCON0=0x3;   //�����ú����ߴ���ģʽ������żУ��λ��1��ֹͣλ��8������λ
        rUCON0=0x245;   //�����ж�Ϊ��ƽ��ʽ�������ж�Ϊ���ط�ʽ����ֹ��ʱ�жϣ������������״̬�жϣ���ֹ����ģʽ����ֹ��ֹ		
                        //�źţ�����ģʽΪ�ж�����ģʽ������ģʽҲΪ�ж�����ģʽ��
    	rUBRDIV0=( (int)(pclk/16./baud+0.5) -1 ); //���ݲ����ʼ���UBRDIV0��ֵ
	 	Delay(10);

    }
    else if(UartNum==1)
    {        
		rGPHCON = rGPHCON & (~(0xffff)) ; //UART1: RXD1<==>GPH5  TXD1<==>GPH4
		rGPHCON = rGPHCON | (0xaaa0) ;    //����GPH�˿�ΪUART��
    	rGPHUP  = 0x0;                    // ʹ����������
	
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
		rGPHCON = rGPHCON | (0xaaa0) ;    //����GPH�˿�ΪUART��
    	rGPHUP  = 0x0;                    // ʹ����������
	
		rUFCON2=0x0;    
        rUMCON2=0x0;   
        rULCON2=0x3; 
        rUCON2=0x245; 
        rUBRDIV2=((int)(pclk/(baud*16))-1);
        Delay(10);
    }
}


//====================================================
// �﷨��ʽ��void Uart_TxEmpty(int ch)
// ��������: 
// ��ڲ���: ���ں�
// ���ڲ���: ��
//====================================================================
void Uart_TxEmpty(int ch)
{
    if(ch==0)
        while(!(rUTRSTAT0 & 0x4)); //�ȴ����ͻ�����Ϊ��
          
    else if(ch==1)
        while(!(rUTRSTAT1 & 0x4));
        
    else if(ch==2)
        while(!(rUTRSTAT2 & 0x4));
}


//====================================================
// �﷨��ʽ��void Uart_SendByte(char ch)
// ��������: �����ֽ�����
// ��ڲ���: ���͵��ֽ�����       
// ���ڲ���: ��
//====================================================================
void Uart_SendByte(char ch)
{
	if (UartNum == 0)
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
	else if (UartNum == 1)
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
    else if (UartNum == 2)
    {
		if(ch=='\n')
	    {
		    while(!(rUTRSTAT2 & 0x2));   //�ȴ���ֱ�����ͻ�����Ϊ��
//		    Delay(10);	                 //�ȴ�
		    rUTXH2='\r';
	    }
	    while(!(rUTRSTAT2 & 0x2));  //Wait until THR is empty.
//	    Delay(10);
	    WrUTXH2(ch);
    }		
}


//====================================================
// �﷨��ʽ��char Uart_ReceiveByte(void)
// ��������: �����ֽ�����
// ��ڲ���: ��
// ���ڲ���: ���յ��ֽ�����
//====================================================================
char Uart_ReceiveByte(void)
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
    else if(UartNum==2)
    {
        while(!(rUTRSTAT2 & 0x1));
        return RdURXH2();
    }
    return 0;
}


//====================================================
// �﷨��ʽ��char Uart_Getch(void)
// ��������: ����һ���ַ�����Uart_ReceiveByteһ��
// ��ڲ���: ��
// ���ڲ���: ���յ��ֽ�����
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
// �﷨��ʽ��char Uart_GetKey(void)
// ��������: �õ���ֵ����Uart_ReceiveByteһ��
// ��ڲ���: ��
// ���ڲ���: ���յ��ֽ�����
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
// �﷨��ʽ��void Uart_Send (char *str)
// ��������: �����ַ���
// ��ڲ���: �ַ���ָ��
// ���ڲ���: ��
//====================================================================
void Uart_Send (char *str)
{
	while (*str)
		Uart_SendByte(*str++);
}	


//====================================================
// �﷨��ʽ��void Uart_receive(char *string)
// ��������: �����ַ���
// ��ڲ���: �ַ���ָ��
// ���ڲ���: ��
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
// �﷨��ʽ��int Uart_GetIntNum(void)
// ��������: �õ�������
// ��ڲ���: ��
// ���ڲ���: ������
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
// �﷨��ʽ��void Uart_Printf(char *fmt,...)
// ��������: ����ʽ����ַ���
// ��ڲ���: printf��ʽ���ַ���
// ���ڲ���: ��
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
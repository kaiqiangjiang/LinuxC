#include <string.h>
#include <stdlib.h>
#include "2440addr.h"
#include "2440lib.h"
#include "def.h"
#include "IrDA.h"
#include "uart.h"

#define IrDA_BUFLEN 0x100

#define UART2_CH0   0
#define UART2_CH1	1

#define LEDOFF    rGPFDAT = 0xFF
#define LED1ON    rGPFDAT = 0xFE
#define LED2ON    rGPFDAT = 0xFD
#define LED3ON    rGPFDAT = 0xFB

void IrDA_Port_Set(void);
void IrDA_Port_Return(void);

void __irq IrDA_TxInt(void);
void __irq IrDA_RxOrErr(void);
void __sub_IrDA_RxInt(void);
void __sub_IrDA_RxErrInt(void);

volatile U8 *IrDAdataPt;
volatile U8 *IrDAdataFl;
volatile U32 IrDA_cnt = 0,IrDA_end,IrDA_err,IrDA_BAUD;
volatile U32 sirda_rGPBCON,sirda_rGPBDAT,sirda_rGPBUP;
volatile U32 sirda_rGPHCON,sirda_rGPHDAT,sirda_rGPHUP;
volatile U32 sirda_ULCON2,sirda_UCON2,sirda_UFCON2,sirda_UMCON2,sirda_UBRDIV2;

extern unsigned int PCLK;

void Select_Device(char Device)
{
	rGPHCON |= 1<<(UART2_CH1<<1);
	rGPHCON &= ~(1<<(UART2_CH1<<1)+1);
	rGPHCON |= 1<<(UART2_CH0<<1);
	rGPHCON &= ~(1<<(UART2_CH0<<1)+1);	 //设置为输出口
	
	rGPHUP &=(~(0x03));
	rGPHDAT	=(rGPHDAT&0xfffffffc)|(Device);																	
}

void IrDA_Port_Set(void)
{
    //Push UART GPIO port configuration
    sirda_rGPBCON=rGPBCON; 
    sirda_rGPBDAT=rGPBDAT;
    sirda_rGPBUP=rGPBUP;
    sirda_rGPHCON=rGPHCON; 
    sirda_rGPHDAT=rGPHDAT;
    sirda_rGPHUP=rGPHUP;
    //Configure IrDA port
    rGPBDAT|=(1<<1);	
    rGPBCON&=0x3ffff3;
    rGPBCON|=(1<<2);	// Output(nIrDATXDEN)
    rGPBUP|=(1<<1);	//Uart port pull-up disable
    rGPHCON&=0x3c0fff;
    rGPHCON|=0x2a000;	// TXD2,RXD2
    rGPHUP|=0x1c0;	//Uart port pull-up disable
    //Push Uart control registers 
    sirda_ULCON2=rULCON2;		
    sirda_UCON2 =rUCON2;		
    sirda_UFCON2=rUFCON2;		
    sirda_UMCON2=rUMCON2;
    sirda_UBRDIV2=rUBRDIV2;
}       
        
void IrDA_Port_Return(void)
{       
    //Pop UART GPIO port configuration
    rGPBCON=sirda_rGPBCON;
    rGPBDAT=sirda_rGPBDAT;
    rGPBUP =sirda_rGPBUP;
    rGPHCON=sirda_rGPHCON;
    rGPHDAT=sirda_rGPHDAT;
    rGPHUP =sirda_rGPHUP;
    //Pop Uart control registers 
    rULCON2=sirda_ULCON2;		
    rUCON2 =sirda_UCON2;	
    rUFCON2=sirda_UFCON2;		
    rUMCON2=sirda_UMCON2;
    rUBRDIV2=sirda_UBRDIV2;
}

void __irq IrDA_TxInt(void)
{
    rINTSUBMSK|=(BIT_SUB_RXD2|BIT_SUB_TXD2|BIT_SUB_ERR2);
    if(IrDA_cnt < (IrDA_BUFLEN))
    {
    	LED1ON;
    	Delay(10);
		Uart_Printf("%d,",*IrDAdataPt);
		WrUTXH2(*IrDAdataPt++);
		IrDA_cnt++;
		LEDOFF;
		Delay(40);
		ClearPending(BIT_UART2);
        rSUBSRCPND=(BIT_SUB_TXD2);
        rINTSUBMSK&=~(BIT_SUB_TXD2);
    }
    else
    {
		IrDA_end=1;
		while(rUFSTAT2 & 0x2f0);	//Until FIFO is empty
		while(!(rUTRSTAT2 & 0x4));	//Until Tx shifter is empty
	        ClearPending(BIT_UART2);
		rINTMSK|=BIT_UART2;
    }

}

void __irq IrDA_RxOrErr(void)
{
    rINTSUBMSK|=(BIT_SUB_RXD2|BIT_SUB_TXD2|BIT_SUB_ERR2);	// Just for the safety
    if(rSUBSRCPND&BIT_SUB_RXD2) __sub_IrDA_RxInt();
    else __sub_IrDA_RxErrInt();
    rSUBSRCPND=(BIT_SUB_RXD2|BIT_SUB_TXD2|BIT_SUB_ERR2);
    rINTSUBMSK&=~(BIT_SUB_RXD2|BIT_SUB_ERR2);
    ClearPending(BIT_UART2);
}

void __sub_IrDA_RxInt(void)
{
    while( (rUFSTAT2 & 0x100) || (rUFSTAT2 & 0xf)  )
    {
    	if ((IrDA_cnt == 0) && (rURXH2 == 255))
    		continue;
    	LED1ON;
		*IrDAdataPt=rURXH2;
		Uart_Printf("%d,",*IrDAdataPt++);
		IrDA_cnt++;
    }
    if(IrDA_cnt >= IrDA_BUFLEN)
    {
    	IrDA_end=1;
    	rINTMSK|=BIT_UART2;
    }
    LEDOFF;
}

void __sub_IrDA_RxErrInt(void)
{
    switch(rUERSTAT2)//to clear and check the status of register bits
    {
	case '1':
	    Uart_Printf("Overrun error\n");
	    break;
	case '2':
	    Uart_Printf("Parity error\n");
	    break;
	case '4':
	    Uart_Printf("Frame error\n");
	    break;
	case '8':
	    Uart_Printf("Breake detect\n");
	    break;
	default :
	    break;
    }
}

void Test_IrDA_Tx(void)
{
    int i;
    IrDA_cnt=0;
    IrDA_end=0;
    IrDAdataFl=(volatile U8 *)IrDABUFFER;
    IrDAdataPt=(volatile U8 *)IrDABUFFER;
    IrDA_Port_Set();
    
    rGPFCON = (rGPFCON | 0xFFFF) & 0xFFFFFF55;  //GPF0--GPF3设置为output       
   	rGPFUP  = rGPFUP & 0xFFF0;                  //使能GPF上拉电阻
   	rGPFDAT = 0x0F;						  	 //GPF低4位初始化为1
   	
    Uart_Select(0);
    for(i=0;i<IrDA_BUFLEN;i++) *IrDAdataFl++=i;	// Initialize IrDA Tx data
    pISR_UART2=(U32)IrDA_TxInt;

    IrDA_BAUD=19200;

    rUBRDIV2=( (int)(PCLK/16./IrDA_BAUD) -1 );
    Uart_Printf("rUBRDIV2=%d\n", rUBRDIV2);
    Uart_Printf("[UART IrDA Tx Test]\n");
    Uart_Printf("This test should be configured two boards.\n");

    rGPBDAT&=0x7fd;	// Enable nIrDATXDEN    
    rUFCON2=(1<<6)|(0<<4)|(1<<2)|(1<<1)|(1);
    //Tx and Rx FIFO Trigger Level:4byte,Tx and Rx FIFO Reset,FIFO on
    rUCON2=(0<<10)|(1<<9)|(1<<8)|(0<<7)|(0<<6)|(0<<5)|(0<<4)|(0<<2)|(0); // From H/W    
    rULCON2=(1<<6)|(0<<3)|(0<<2)|(3);	// IrDA,No parity,One stop bit, 8bit

    rUCON2=(0<<10)|(1<<9)|(1<<8)|(0<<7)|(0<<6)|(0<<5)|(0<<4)|(1<<2)|(0);
    // PCLK,Tx&Rx:Level,Rx timeout:x,Rx error int:x,Loop-back:x,Send break:x,Tx:int,Rx:x
    Uart_Printf("Now... Tx with IrDA\n");
    rINTMSK=~(BIT_UART2);
    rINTSUBMSK=~(BIT_SUB_RXD2|BIT_SUB_TXD2|BIT_SUB_ERR2);

    while(!IrDA_end && (Uart_GetKey() != ESC_KEY));

    rINTSUBMSK|=(BIT_SUB_RXD2|BIT_SUB_TXD2|BIT_SUB_ERR2);
    rUFCON2=(3<<6)|(2<<4)|(1<<2)|(1<<1)|(0);
    rGPBDAT|=(1<<1);	// Disable nIrDATXDEN        
    Uart_Printf("\nEnd Tx, transfer data count=%d\n",IrDA_cnt);
    IrDA_Port_Return();
}

void Test_IrDA_Rx(void)
{
	int i = 0;
    IrDA_cnt=0;
    IrDA_end=0;
    IrDA_err=0;
    IrDAdataFl=(volatile U8 *)IrDABUFFER;
    IrDAdataPt =(volatile U8 *)IrDABUFFER;
    IrDA_Port_Set();  
    
    rGPFCON = (rGPFCON | 0xFFFF) & 0xFFFFFF55;  //GPF0--GPF3设置为output       
   	rGPFUP  = rGPFUP & 0xFFF0;                  //使能GPF上拉电阻
   	rGPFDAT = 0x0F;						  	 //GPF低4位初始化为1
      
    Uart_Select(0);
    pISR_UART2=(unsigned)IrDA_RxOrErr;
    
    IrDA_BAUD=19200; //set IrDA Baud 19200

    rUBRDIV2=( (int)(PCLK/16./IrDA_BAUD) -1 );
    Uart_Printf("IrDA Baud = 19200\n");
    Uart_Printf("Now... Rx with IrDA\n");
    
    rUFCON2=(1<<6)|(0<<4)|(1<<2)|(1<<1)|(1);
    //Tx and Rx FIFO Trigger Level:4byte,Tx and Rx Reset,FIFO En
    rUCON2=(0<<10)|(1<<9)|(1<<8)|(0<<7)|(1<<6)|(0<<5)|(0<<4)|(0<<2)|(0); // From H/W
    rULCON2=(1<<6)|(0<<3)|(0<<2)|(3);	// IrDA,No parity,One stop bit, 8bit
    Delay(1); // From H/W
    rUCON2=(0<<10)|(1<<9)|(1<<8)|(0<<7)|(1<<6)|(0<<5)|(0<<4)|(0<<2)|(1);
    // PCLK,Tx&Rx:Level,Rx timeout:x,Rx error int:o,Loop-back:x,Send break:x,Tx:x,Rx:int
    rINTMSK=~(BIT_UART2);
    rINTSUBMSK=~(BIT_SUB_RXD2|BIT_SUB_ERR2);

    while(!IrDA_end && (Uart_GetKey() != ESC_KEY));
    
    rINTSUBMSK|=(BIT_SUB_RXD2|BIT_SUB_TXD2|BIT_SUB_ERR2);
    rUFCON2=(3<<6)|(2<<4)|(1<<2)|(1<<1)|(0);
    Uart_Printf("\nEnd Rx, receive data count=%d\n",IrDA_cnt);

//	while(*IrDAdataFl != 0)
//		*IrDAdataFl++;    
    for(i=0;i<IrDA_BUFLEN;i++)
    {
		if(i-(*IrDAdataFl++))
		{
		    Uart_Printf("i=%d,",i);
		    IrDA_err++;
		}
    }
    
    if(IrDA_err)
    {
    	Uart_Printf("IrDA test fail!! Error count=%d\n",IrDA_err);
    	LED3ON;	
    }
    else
    {
    	Uart_Printf("IrDA test is good!!\n");
    	LED2ON;	
    }
    
    IrDA_Port_Return();
}

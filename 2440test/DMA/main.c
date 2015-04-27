//======================================================================
//	�������ƣ�	Exa_DMA
//	����������	ͨ�������ն˿��Կ���DMA���ԵĽ����DMA�������ݵ�ʱ����
//  IDE������   ADS v1.2
//	����ļ���	main.c uart.c 
//	Ӳ�����ӣ�	�� 
//	ά����¼��	2009-8-14	V1.0    by xgc
//======================================================================

//=============================================================
// �ļ����ƣ�	main.c
// ����������	������������
// ά����¼��	2009-8-14	V1.0
//=============================================================

//====================================================
// ����ͷ�ļ���
//====================================================

#include "2440addr.h"
#include "2440lib.h"
#include "option.h"
#include "def.h"
#include "uart.h"
#include <string.h> 

extern unsigned int PCLK; 
//====================================================
// ����������
//====================================================
 void __irq Dma0Done(void);    //DMA0�жϺ���
 void __irq Dma1Done(void);    //DMA1�жϺ���
 void __irq Dma2Done(void);    //DMA2�жϺ���
 void __irq Dma3Done(void);    //DMA3�жϺ���
void DMA_M2M(int ch,int srcAddr,int dstAddr,int tc,int dsz,int burst); //�ڴ浽�ڴ��DMA���ݴ��亯��
void Test_DMA(void);           //DMA�������

//====================================================
//����������
//====================================================
                    /* DMA���⹦�ܼĴ��� */
typedef struct tagDMA
{
    volatile U32 DISRC;	    //0x0     DMA��ʼԴ�Ĵ���
    volatile U32 DISRCC;    //0x4     DMA��ʼԴ���ƼĴ���
    volatile U32 DIDST;	    //0x8     DMA��ʼĿ�ļĴ���
    volatile U32 DIDSTC;    //0xc     DMA��ʼĿ�Ŀ��ƼĴ���
    volatile U32 DCON;	    //0x10    DMA���ƼĴ���
    volatile U32 DSTAT;	    //0x14    DMA״̬�Ĵ���
    volatile U32 DCSRC;	    //0x18    ��ǰԴ�Ĵ���
    volatile U32 DCDST;	    //0x1c    ��ǰĿ�ļĴ���
    volatile U32 DMASKTRIG; //0x20    DMA���봥���Ĵ���
}DMA;

static volatile int dmaDone; //DMA�����������ʶ  0δ��� 1���

void Timer_Start(int divider)  //0:16us,1:32us 2:64us 3:128us
{
    rWTCON = ((PCLK/1000000-1)<<8)|(divider<<3);  //Watch-dog timer control register
    rWTDAT = 0xffff;  //Watch-dog timer data register
    rWTCNT = 0xffff;  //Watch-dog count register

      // Watch-dog timer enable & interrupt  disable
    rWTCON = (rWTCON & ~(1<<5) & ~(1<<2)) |(1<<5);
}

//=================================================================
int Timer_Stop(void)
{
    rWTCON = ((PCLK/1000000-1)<<8);
    return (0xffff - rWTCNT);
}


/********************************************************************
// �﷨��ʽ	: void Main(void)
// �������� : DMA����ʵ��������
//            ʵ�ֹ��ܣ�
//                ʵ��DMA��ʽ�ڴ浽�ڴ�Ŀ����������޸�DMA����
//                ���Ƚ��乤��Ч�ʣ�ʵ�������DMA0-DMA3
// ��ڲ���		: ��
// ���ڲ���     : ��
*********************************************************************/
void Main(void)
{
	memcpy((U8 *)0x0,(U8 *)0x30000000,0x1000);
	
	SetSysFclk(FCLK_400M);  			//����ϵͳʱ�� 400M
    ChangeClockDivider(2,1);      		//���÷�Ƶ 1��4��8
	CalcBusClk();           //��������Ƶ

	Uart_Select(0); 
    Uart_Init(0,115200);
    
	Uart_Printf("\n---DMA����ʵ��������---\n");
	
    Test_DMA();
    
    Uart_Printf("\nDMA���Խ���\n");
	while(1);
}

void Test_DMA(void)
{
    //DMA Ch 0   _NONCACHE_STARTADDRESS = 0x30400000 
    DMA_M2M(0,_NONCACHE_STARTADDRESS,_NONCACHE_STARTADDRESS+0x800000,0x80000,0,0); //byte,single
    DMA_M2M(0,_NONCACHE_STARTADDRESS,_NONCACHE_STARTADDRESS+0x800000,0x40000,1,0); //halfword,single    
    DMA_M2M(0,_NONCACHE_STARTADDRESS,_NONCACHE_STARTADDRESS+0x800000,0x20000,2,0); //word,single
    DMA_M2M(0,_NONCACHE_STARTADDRESS,_NONCACHE_STARTADDRESS+0x800000,0x20000,0,1); //byte,burst    
    DMA_M2M(0,_NONCACHE_STARTADDRESS,_NONCACHE_STARTADDRESS+0x800000,0x10000,1,1); //halfword,burst
    DMA_M2M(0,_NONCACHE_STARTADDRESS,_NONCACHE_STARTADDRESS+0x800000, 0x8000,2,1); //word,burst

    //DMA Ch 1
    DMA_M2M(1,_NONCACHE_STARTADDRESS,_NONCACHE_STARTADDRESS+0x800000,0x80000,0,0); //byte,single
    DMA_M2M(1,_NONCACHE_STARTADDRESS,_NONCACHE_STARTADDRESS+0x800000,0x40000,1,0); //halfword,single
    DMA_M2M(1,_NONCACHE_STARTADDRESS,_NONCACHE_STARTADDRESS+0x800000,0x20000,2,0); //word,single
    DMA_M2M(1,_NONCACHE_STARTADDRESS,_NONCACHE_STARTADDRESS+0x800000,0x20000,0,1); //byte,burst
    DMA_M2M(1,_NONCACHE_STARTADDRESS,_NONCACHE_STARTADDRESS+0x800000,0x10000,1,1); //halfword,burst
    DMA_M2M(1,_NONCACHE_STARTADDRESS,_NONCACHE_STARTADDRESS+0x800000, 0x8000,2,1); //word,burst

    //DMA Ch 2
    DMA_M2M(2,_NONCACHE_STARTADDRESS,_NONCACHE_STARTADDRESS+0x800000,0x80000,0,0); //byte,single
    DMA_M2M(2,_NONCACHE_STARTADDRESS,_NONCACHE_STARTADDRESS+0x800000,0x40000,1,0); //halfword,single
    DMA_M2M(2,_NONCACHE_STARTADDRESS,_NONCACHE_STARTADDRESS+0x800000,0x20000,2,0); //word,single
    DMA_M2M(2,_NONCACHE_STARTADDRESS,_NONCACHE_STARTADDRESS+0x800000,0x20000,0,1); //byte,burst
    DMA_M2M(2,_NONCACHE_STARTADDRESS,_NONCACHE_STARTADDRESS+0x800000,0x10000,1,1); //halfword,burst
    DMA_M2M(2,_NONCACHE_STARTADDRESS,_NONCACHE_STARTADDRESS+0x800000, 0x8000,2,1); //word,burst

    //DMA Ch 3
    DMA_M2M(3,_NONCACHE_STARTADDRESS,_NONCACHE_STARTADDRESS+0x800000,0x80000,0,0); //byte,single
    DMA_M2M(3,_NONCACHE_STARTADDRESS,_NONCACHE_STARTADDRESS+0x800000,0x40000,1,0); //halfword,single
    DMA_M2M(3,_NONCACHE_STARTADDRESS,_NONCACHE_STARTADDRESS+0x800000,0x20000,2,0); //word,single
    DMA_M2M(3,_NONCACHE_STARTADDRESS,_NONCACHE_STARTADDRESS+0x800000,0x20000,0,1); //byte,burst
    DMA_M2M(3,_NONCACHE_STARTADDRESS,_NONCACHE_STARTADDRESS+0x800000,0x10000,1,1); //halfword,burst
    DMA_M2M(3,_NONCACHE_STARTADDRESS,_NONCACHE_STARTADDRESS+0x800000, 0x8000,2,1); //word,burst   
}
/********************************************************************
// �﷨��ʽ��void DMA_M2M(int ch,int srcAddr,int dstAddr,int tc,int dsz,int burst)
// ��������: DMA��ʽ�ڴ濽��
// ��ڲ���: 
//         : int ch:DMAͨ��   0-DMA0, 1-DMA1, 2-DMA2, 3-DMA3
//         : int srcAddr:Դ��ַ
//         : int dstAddr:Ŀ�ĵ�ַ
//         : int tc:��ʼ�������ֵ
//         : int dsz:�������ݿ��  0:1�ֽ� 1:2�ֽ� 2:4�ֽ�
//         : int burst:�Զ�����Ĵ�����  0-��Ԫ���䣨һ���ֽڣ�  1-ͻ��ģʽ���䣨�ĸ��ֽڣ�
// ���ڲ���: ��
*********************************************************************/
void DMA_M2M(int ch,int srcAddr,int dstAddr,int tc,int dsz,int burst)
{
    int i,time;
    volatile U32 memSum0=0,memSum1=0;
    DMA *pDMA;
    int length;
        
    length=tc*(burst ? 4:1)*((dsz==0)+(dsz==1)*2+(dsz==2)*4); //ȷ��һ�δ�����ֽ���( ���䵥Ԫģʽ * �������ݿ�� )
        
    Uart_Printf("[DMA%d MEM2MEM Test]\n",ch);			

    switch(ch)
    {
	    case 0:
	        pISR_DMA0 = (unsigned)Dma0Done;	
	        EnableIrq(BIT_DMA0); 		//open DMA0 INTERRUPT
	    	pDMA=(void *)0x4b000000;
	    	break;
	    case 1:
	    	 pISR_DMA1 = (unsigned)Dma1Done;	
	        EnableIrq(BIT_DMA1); 		//open DMA1  INTERRUPT
	    	pDMA=(void *)0x4b000040;
	    	break;
	    case 2:
	    	 pISR_DMA2 = (unsigned)Dma2Done;	
	        EnableIrq(BIT_DMA2); 		//open DMA2  INTERRUPT
	    	pDMA=(void *)0x4b000080;
			break;
	    case 3:
	    	pISR_DMA3 = (unsigned)Dma3Done;	
	        EnableIrq(BIT_DMA3); 		//open  DMA3  INTERRUPT
	       	pDMA=(void *)0x4b0000c0;
	        break;
    }
                                                                                                                            
    Uart_Printf("DMA%d %8xh->%8xh,size=%xh(tc=%xh),dsz=%d,burst=%d\n",ch,
    		srcAddr,dstAddr,length,tc,dsz,burst);

   	Uart_Printf("Initialize the src.\n");
    
    for(i=srcAddr;i<(srcAddr+length);i+=4)
    {
    	*((U32 *)i)=i^0x55aa5aa5;   //��Դ��ַд���������� д�볤��Ϊlength
    	memSum0+=i^0x55aa5aa5;      //��д�������ۼӣ�ΪУ��������ݵ�׼ȷ��
    }

    Uart_Printf("DMA%d start\n",ch);  
    
    dmaDone=0;
    
    pDMA->DISRC=srcAddr;        //����Դ��ַ
    pDMA->DISRCC=(0<<1)|(0<<0); //����Դ���ƼĴ���   inc,AHB
    pDMA->DIDST=dstAddr;        //����Ŀ�ĵ�ַ
    pDMA->DIDSTC=(0<<1)|(0<<0); //����Ŀ�Ŀ��ƼĴ��� inc,AHB
    
    pDMA->DCON=(1<<31)|(1<<30)|(1<<29)|(burst<<28)|(1<<27)|
    	        (0<<23)|(1<<22)|(dsz<<20)|(tc);
    		//DMA���ƼĴ���  HS,AHB sync,enable interrupt,whole, SW request mode,relaod off
    		
     
    pDMA->DMASKTRIG=(1<<1)|1; //DMA on, SW_TRIG
     		
    Timer_Start(3);//128us resolution	    
    while(dmaDone==0);
    time=Timer_Stop();
    
    Uart_Printf("DMA transfer done.\n");
    Uart_Printf("time = %u MS\n", time*128/1000);   
 

   	DisableIrq(BIT_DMA0);
   	DisableIrq(BIT_DMA1);
   	DisableIrq(BIT_DMA2);
   	DisableIrq(BIT_DMA3);
    
    for(i=dstAddr;i<dstAddr+length;i+=4)
    {
    	memSum1+=*((U32 *)i)=i^0x55aa5aa5;
    }
    
    Uart_Printf("\n memSum0=%x,memSum1=%x\n",memSum0,memSum1);
    if(memSum0==memSum1)
    	Uart_Printf("DMA test result--------------------------------------O.K.\n");
    else 
    	Uart_Printf("DMA test result--------------------------------------ERROR!!!\n");

}
//====================================================
// �﷨��ʽ��void __irq Dma0Done(void)
// ��������: DMA0�жϺ��� 
//           ����DMA0��ʶ
// ��ڲ���: ��
// ���ڲ���: ��
//====================================================

 void __irq Dma0Done(void)
{   
    dmaDone=1;
    ClearPending(BIT_DMA0);
   
}
//====================================================
// �﷨��ʽ��void __irq Dma1Done(void)
// ��������: DMA1�жϺ��� 
//           ����DMA��ʶ
// ��ڲ���: ��
// ���ڲ���: ��
//====================================================
 void __irq Dma1Done(void)
{
    dmaDone=1;
    ClearPending(BIT_DMA1);
   
}
//====================================================
// �﷨��ʽ��void __irq Dma2Done(void)
// ��������: DMA2�жϺ��� 
//           ����DMA��ʶ
// ��ڲ���: ��
// ���ڲ���: ��
//====================================================
 void __irq Dma2Done(void)
{
   dmaDone=1;
   ClearPending(BIT_DMA2);
    
}
//====================================================
// �﷨��ʽ��void __irq Dma3Done(void)
// ��������: DMA3�жϺ��� 
//           ����DMA��ʶ
// ��ڲ���: ��
// ���ڲ���: ��
//====================================================
 void __irq Dma3Done(void)
{
   dmaDone=1;
   ClearPending(BIT_DMA3);
    
}
//======================================================================
//	工程名称：	Exa_DMA
//	功能描述：	通过超级终端可以看到DMA测试的结果和DMA传输数据的时长。
//  IDE环境：   ADS v1.2
//	组成文件：	main.c uart.c 
//	硬件连接：	无 
//	维护记录：	2009-8-14	V1.0    by xgc
//======================================================================

//=============================================================
// 文件名称：	main.c
// 功能描述：	定义了主函数
// 维护记录：	2009-8-14	V1.0
//=============================================================

//====================================================
// 包含头文件区
//====================================================

#include "2440addr.h"
#include "2440lib.h"
#include "option.h"
#include "def.h"
#include "uart.h"
#include <string.h> 

extern unsigned int PCLK; 
//====================================================
// 函数声明区
//====================================================
 void __irq Dma0Done(void);    //DMA0中断函数
 void __irq Dma1Done(void);    //DMA1中断函数
 void __irq Dma2Done(void);    //DMA2中断函数
 void __irq Dma3Done(void);    //DMA3中断函数
void DMA_M2M(int ch,int srcAddr,int dstAddr,int tc,int dsz,int burst); //内存到内存的DMA数据传输函数
void Test_DMA(void);           //DMA传输测试

//====================================================
//变量设置区
//====================================================
                    /* DMA特殊功能寄存器 */
typedef struct tagDMA
{
    volatile U32 DISRC;	    //0x0     DMA初始源寄存器
    volatile U32 DISRCC;    //0x4     DMA初始源控制寄存器
    volatile U32 DIDST;	    //0x8     DMA初始目的寄存器
    volatile U32 DIDSTC;    //0xc     DMA初始目的控制寄存器
    volatile U32 DCON;	    //0x10    DMA控制寄存器
    volatile U32 DSTAT;	    //0x14    DMA状态寄存器
    volatile U32 DCSRC;	    //0x18    当前源寄存器
    volatile U32 DCDST;	    //0x1c    当前目的寄存器
    volatile U32 DMASKTRIG; //0x20    DMA掩码触发寄存器
}DMA;

static volatile int dmaDone; //DMA传输完成与否标识  0未完成 1完成

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
// 语法格式	: void Main(void)
// 功能描述 : DMA操作实验主程序
//            实现功能：
//                实现DMA方式内存到内存的拷贝动作，修改DMA设置
//                并比较其工作效率，实验包括：DMA0-DMA3
// 入口参数		: 无
// 出口参数     : 无
*********************************************************************/
void Main(void)
{
	memcpy((U8 *)0x0,(U8 *)0x30000000,0x1000);
	
	SetSysFclk(FCLK_400M);  			//设置系统时钟 400M
    ChangeClockDivider(2,1);      		//设置分频 1：4：8
	CalcBusClk();           //计算总线频

	Uart_Select(0); 
    Uart_Init(0,115200);
    
	Uart_Printf("\n---DMA操作实验主程序---\n");
	
    Test_DMA();
    
    Uart_Printf("\nDMA测试结束\n");
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
// 语法格式：void DMA_M2M(int ch,int srcAddr,int dstAddr,int tc,int dsz,int burst)
// 功能描述: DMA方式内存拷贝
// 入口参数: 
//         : int ch:DMA通道   0-DMA0, 1-DMA1, 2-DMA2, 3-DMA3
//         : int srcAddr:源地址
//         : int dstAddr:目的地址
//         : int tc:初始传输计数值
//         : int dsz:传输数据宽度  0:1字节 1:2字节 2:4字节
//         : int burst:自动传输的传输宽度  0-单元传输（一个字节）  1-突发模式传输（四个字节）
// 出口参数: 无
*********************************************************************/
void DMA_M2M(int ch,int srcAddr,int dstAddr,int tc,int dsz,int burst)
{
    int i,time;
    volatile U32 memSum0=0,memSum1=0;
    DMA *pDMA;
    int length;
        
    length=tc*(burst ? 4:1)*((dsz==0)+(dsz==1)*2+(dsz==2)*4); //确定一次传输的字节数( 传输单元模式 * 传输数据宽度 )
        
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
    	*((U32 *)i)=i^0x55aa5aa5;   //向源地址写入任意数据 写入长度为length
    	memSum0+=i^0x55aa5aa5;      //将写入数据累加，为校验读出数据的准确性
    }

    Uart_Printf("DMA%d start\n",ch);  
    
    dmaDone=0;
    
    pDMA->DISRC=srcAddr;        //设置源地址
    pDMA->DISRCC=(0<<1)|(0<<0); //设置源控制寄存器   inc,AHB
    pDMA->DIDST=dstAddr;        //设置目的地址
    pDMA->DIDSTC=(0<<1)|(0<<0); //设置目的控制寄存器 inc,AHB
    
    pDMA->DCON=(1<<31)|(1<<30)|(1<<29)|(burst<<28)|(1<<27)|
    	        (0<<23)|(1<<22)|(dsz<<20)|(tc);
    		//DMA控制寄存器  HS,AHB sync,enable interrupt,whole, SW request mode,relaod off
    		
     
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
// 语法格式：void __irq Dma0Done(void)
// 功能描述: DMA0中断函数 
//           更新DMA0标识
// 入口参数: 无
// 出口参数: 无
//====================================================

 void __irq Dma0Done(void)
{   
    dmaDone=1;
    ClearPending(BIT_DMA0);
   
}
//====================================================
// 语法格式：void __irq Dma1Done(void)
// 功能描述: DMA1中断函数 
//           更新DMA标识
// 入口参数: 无
// 出口参数: 无
//====================================================
 void __irq Dma1Done(void)
{
    dmaDone=1;
    ClearPending(BIT_DMA1);
   
}
//====================================================
// 语法格式：void __irq Dma2Done(void)
// 功能描述: DMA2中断函数 
//           更新DMA标识
// 入口参数: 无
// 出口参数: 无
//====================================================
 void __irq Dma2Done(void)
{
   dmaDone=1;
   ClearPending(BIT_DMA2);
    
}
//====================================================
// 语法格式：void __irq Dma3Done(void)
// 功能描述: DMA3中断函数 
//           更新DMA标识
// 入口参数: 无
// 出口参数: 无
//====================================================
 void __irq Dma3Done(void)
{
   dmaDone=1;
   ClearPending(BIT_DMA3);
    
}
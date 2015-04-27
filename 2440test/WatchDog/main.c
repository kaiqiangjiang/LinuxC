//=========================================================================
//  工程名称：	WatchDog
//  功能描述： 	利用看门狗定时器产生中断，控制LED的闪烁			           	
//  组成文件：	main.c
//  硬件连接：			
//  维护记录：	2009-08-17 v1.0  by xgc
//              2009-10-14 V1.1  by xgc      
//=========================================================================

#include "def.h"
#include "2440addr.h"
#include "2440lib.h"
#include "string.h"

#define WDT_ENABLE			(0x01<<5)
#define WDT_INT_ENABLE		(0x01<<2)
#define WDT_RST_ENABLE		(0x01<<0)	
#define WDT_CLK_SEL			(0X03<<3)				/* 1/128 */
#define WDT_PRE_SCALER		(0XFF<<8)				/* 255   */

void __irq Wdt_int(void);

//=========================================================================
//  文件名：	main.c
//  功能描述：	定义了主函数
//  维护记录:	2008-08-17 v1.0	   by xgc
//              2009-10-14 V1.1    by xgc
//=========================================================================


//=====================================================================
//	语法格式：	int main(void)
//	实现功能：	看门狗实验
//	参数：		无
//	返回值：	无
//=====================================================================
int Main()
{
	unsigned int i = 0, j = 0;	 
	memcpy((unsigned char *)0x0,(unsigned char *)0x30000000,0x1000);

	SetSysFclk(FCLK_400M);  //设置系统时钟 400M     
	ChangeClockDivider(2, 1);      //设置分频 1：4：8
	CalcBusClk();           //计算总线频
	
	rGPFCON = (rGPFCON | 0xFFFF) & 0xFFFFFF55;  //GPF0--GPF4设置为output       
   	rGPFUP  = rGPFUP & 0xFFF0;                  //使能GPF上拉电阻
   	rGPFDAT = 0x0F;						  	 //GPF低4位初始化为1
   	
    //清WatchDog中断寄存器
	rSRCPND |=BIT_WDT_AC97;
    rINTPND |=BIT_WDT_AC97;
    rSUBSRCPND |= BIT_SUB_WDT; 
   
    //建立WatchDog中断
  	pISR_WDT_AC97=(unsigned)Wdt_int;
  
	rWTCON = WDT_PRE_SCALER| WDT_ENABLE|WDT_CLK_SEL|WDT_INT_ENABLE; //看门狗中断使能
//	rWTCON = WDT_PRE_SCALER| WDT_ENABLE|WDT_CLK_SEL|WDT_RST_ENABLE; //需要复位时用这个  
   
    //看门狗喂狗,看门狗设置复位周期1秒
  	rWTDAT = 0x3E8;   //WTDAT看门狗数据寄存器,看门狗定时器重载的计数值
  	rWTCNT = 0x3E8;   //WTCNT看门狗计数寄存器,看门狗定时器当前计数值

    //开中断
  	rINTMSK &= ~(BIT_WDT_AC97); 
  	rINTSUBMSK &= ~(BIT_SUB_WDT);              
	while(1);           
}
  
//===================================================
//名称： Wdt_Int
//功能： 中断响应函数
//参数： void
//返回值：void
//===================================================
void __irq Wdt_int(void)
{
	rGPFDAT = ~rGPFDAT;
  	if(rSUBSRCPND & ~ BIT_SUB_WDT)
  	{            //清除中断
      	rSUBSRCPND |=(BIT_SUB_WDT);   //清除一级源中断指示寄存器相应位 
      	rSRCPND |= BIT_WDT_AC97; 
     	rINTPND |= BIT_WDT_AC97;           //清除源中断指示寄存器相应位
  	}

 	if(rSUBSRCPND & ~ BIT_SUB_AC97)
  	{
       	rSUBSRCPND |=BIT_SUB_AC97; 
       	rSRCPND |= BIT_WDT_AC97; 
       	rINTPND |= BIT_WDT_AC97;       //清除源中断指示寄存器相应位
  	}
}




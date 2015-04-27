//====================================================================
// 文件名  : ADC.c
// 功  能  : ADC测试程序
//	维护记录：	2009-08-14	v1.0     by xgc
//				2009-10-28  v1.1     by xgc
//====================================================================
#include "def.h"
#include "2440addr.h"
#include "2440lib.h"
#include "uart.h"
#include "ADC.h"

void Test_ADC(void)
{
    U8 ch;
	
	Uart_Printf("\nSelect ADC Chanel: 0 or 1");
	ch=Uart_Getch();
	Uart_Printf("\n%c\n\n", ch);
	
	if (ch!='0' && ch !='1')
	{
		Uart_Printf("you select wrong chanel!\n");
		return;
	}
	
    rADCDLY = 100;				//ADC转换延时	
//    rADCTSC = 0;				//设置ADC为普通模式
   								/* 进行ADC模块设置，其中x<<n表示第n位设置为x(若x超过一位，则向高位顺延) */
    rADCCON = (0 << 0)     |	// ADC转换设置 未设置
              (0 << 1)     |    // 读AD数据触发AD转换 未使用
              (0 << 2)     |    // StandBy模式选择 为普通操作模式
              (ch << 3)    |    // ADC通道选择 ch
              (49 << 6)    | 	// CLKDIV = Fpclk /49+1/5 ，即转换时钟为1MHz  Fpclk = 10M   ADC转换频率400K
              (1 << 14)   ;	    // 使能软件预设值

	while(Uart_GetKey() != ESC_KEY)
	{
		AD_ENABLE();
	}
}

//=======================================================================
//名称：AD_ENABLE()
//功能：采用置位使能方式启动AD转换
//参数: 无
//返回值: 无
//=======================================================================
 
void AD_ENABLE (void) 
 {
   	int i,j;
    int val;
 	val = 0;
	
    for(i=0;i<16;i++)
    {
		rADCCON |= 0x1;         		 //使能ADC 转换
       
		while(rADCCON&0x1);    		 //判断是否使能ADC转换
       
		while(!rADCCON&0x8000);		 //判断ADC转换是否结束
       
		val += (rADCDAT0 &0x03ff);    //取出ADC转换值
		for(j=0;j<500;j++);       
    }
    val = val/16;                   //计算ADC平均转换值
    Delay(500);
    Uart_Printf("ADC val = %d\n", val);  //发送到串口显示
 
 }
 
 
//=======================================================================
//名称：AD_READ()
//功能：采用读控制器的方式启动AD转换
//参数: 无
//返回值: 无
//=======================================================================
 void AD_READ (void) 
 {
   	int i,j;
    int val,aa;
 	val = 0;
 	rADCCON |= 0x2;         		 //ADC转换通过读操作来启动
 	aa = rADCDAT0 &0x03ff; 			 //启动ADC转换
 	
    for(i=0;i<16;i++)
    {
		while(!rADCCON&0x8000);		 //判断ADC转换是否结束
       
		val += (rADCDAT0 &0x03ff);    //取出ADC转换值
		for(j=0;j<500;j++);       
    }
    val = val/16;                   //计算ADC平均转换值
    Delay(500);
    Uart_Printf("ADC val = %d\n", val);  //发送到串口显示
 
 }
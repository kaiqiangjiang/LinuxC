/**********************************************************
*实验要求：   定时器中断方式控制LED按精确时间间隔闪烁。
*功能描述：   本实验代码利用Timer0中断控制LED按照同样是0.5s的精确
*           时间间隔闪烁。       
*日    期：   2012-2-17
*作    者：   国嵌
**********************************************************/
#define	 GLOBAL_CLK		1
#include <stdlib.h>
#include <string.h>
#include "def.h"
#include "option.h"
#include "2440addr.h"
#include "2440lib.h"
#include "2440slib.h"
#include "mmu.h"
#include "profile.h"
#include "memtest.h"
void Timer0_init(void);
static void __irq IRQ_Timer0_Handle(void);
void Led1_init(void);
void Led1_run(void);
 
/*
 * 程序入口
 * */
int Main(void)
{	
    MMU_Init();
    Led1_init();
    Timer0_init();
	while(1);

	return 0;
}	

/*
 *  Timer0中断初始化函数
 * @ 每1s会触发一次定时器中断
 * */
void Timer0_init(void)
{
	//Timer 0 init, pclk==50MHZ
	rTCFG0 = 49;              	//pclk/(49+1)
	rTCFG1 = 0x03;            	//16分频=62500HZ
	rTCNTB0 = 62500/2;       	//TCNTB0[15:0]=计数值  
	rTCMPB0 = 0;
	rTCON |=(1<<1);           	//将计数值装入TCNTB0、TCMPB0

	rTCON =0x09;

	rPRIORITY = 0x00000000;  	// 默认优先级
	rINTMOD = 0x00000000;    	// 全部设置为普通中断模式

	ClearPending(BIT_TIMER0);
	pISR_TIMER0 = (U32)IRQ_Timer0_Handle;
	EnableIrq(BIT_TIMER0); 
}

/*
 *    Timer0中断处理函数
 * @ 运行Led1_run()，在退出处理函数之前先清除未决中断位。
 * */
static void __irq IRQ_Timer0_Handle(void)
{
    Led1_run();
    ClearPending(BIT_TIMER0);
}

/*
 *    LED1初始化函数
 * @ 将LED1对应的GPB5设置为输出。
 * */
void Led1_init(void)
{
    rGPBCON &= ~(0x3<<10);
    rGPBCON |=  (0x1<<10);
}

/*
 *    LED1控制函数
 * @ 如果LED1（对应管脚GPB5）之前被点亮，则灭掉它，否则点亮它。
 * */
void Led1_run(void)
{
    //rGPBDAT = rGPBDAT^(0x1<<5);
   
    if(rGPBDAT &(1<<5))
       rGPBDAT &=~(1<<5);
    else
       rGPBDAT |=(1<<5);
}

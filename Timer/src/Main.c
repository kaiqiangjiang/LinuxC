/**********************************************************
*ʵ��Ҫ��   ��ʱ���жϷ�ʽ����LED����ȷʱ������˸��
*����������   ��ʵ���������Timer0�жϿ���LED����ͬ����0.5s�ľ�ȷ
*           ʱ������˸��       
*��    �ڣ�   2012-2-17
*��    �ߣ�   ��Ƕ
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
 * �������
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
 *  Timer0�жϳ�ʼ������
 * @ ÿ1s�ᴥ��һ�ζ�ʱ���ж�
 * */
void Timer0_init(void)
{
	//Timer 0 init, pclk==50MHZ
	rTCFG0 = 49;              	//pclk/(49+1)
	rTCFG1 = 0x03;            	//16��Ƶ=62500HZ
	rTCNTB0 = 62500/2;       	//TCNTB0[15:0]=����ֵ  
	rTCMPB0 = 0;
	rTCON |=(1<<1);           	//������ֵװ��TCNTB0��TCMPB0

	rTCON =0x09;

	rPRIORITY = 0x00000000;  	// Ĭ�����ȼ�
	rINTMOD = 0x00000000;    	// ȫ������Ϊ��ͨ�ж�ģʽ

	ClearPending(BIT_TIMER0);
	pISR_TIMER0 = (U32)IRQ_Timer0_Handle;
	EnableIrq(BIT_TIMER0); 
}

/*
 *    Timer0�жϴ�����
 * @ ����Led1_run()�����˳�������֮ǰ�����δ���ж�λ��
 * */
static void __irq IRQ_Timer0_Handle(void)
{
    Led1_run();
    ClearPending(BIT_TIMER0);
}

/*
 *    LED1��ʼ������
 * @ ��LED1��Ӧ��GPB5����Ϊ�����
 * */
void Led1_init(void)
{
    rGPBCON &= ~(0x3<<10);
    rGPBCON |=  (0x1<<10);
}

/*
 *    LED1���ƺ���
 * @ ���LED1����Ӧ�ܽ�GPB5��֮ǰ����������������������������
 * */
void Led1_run(void)
{
    //rGPBDAT = rGPBDAT^(0x1<<5);
   
    if(rGPBDAT &(1<<5))
       rGPBDAT &=~(1<<5);
    else
       rGPBDAT |=(1<<5);
}

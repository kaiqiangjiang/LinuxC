#define	GLOBAL_CLK		1
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


void Led_port_init(void);

#define LED1_ON   ~(1<<5)
#define LED2_ON   ~(1<<6)
#define LED3_ON   ~(1<<7)
#define LED4_ON   ~(1<<8)

#define LED1_OFF   (1<<5)
#define LED2_OFF   (1<<6)
#define LED3_OFF   (1<<7)
#define LED4_OFF   (1<<8)
/*************************************************
Function name: delay
Parameter    : times
Description	 : ��ʱ����
Return		 : void
Argument     : void
Autor & date :
**************************************************/
void delay(int times)
{
    int i;
    for(;times>0;times--)
      for(i=0;i<400;i++);
}
/*************************************************
Function name: Main
Parameter    : void
Description	 : �����ܺ���
Return		 : void
Argument     : void
Autor & date :
**************************************************/
void Main(void)
{	
    int i;
    
    Led_port_init();
    
    for(i=0;i<100;i++)
    {
    	/* ȫ�� */
    	rGPBDAT = rGPBDAT&(LED1_ON)&(LED2_ON)&(LED3_ON)&(LED4_ON);
    
    	delay(10000);
    
    	/* ȫ�� */
    	rGPBDAT = rGPBDAT|(LED1_OFF)|(LED2_OFF)|(LED3_OFF)|(LED4_OFF);
    	
    	delay(10000);
    }
}	

/*************************************************
Function name: Led_port_init
Parameter    : void
Description	 : TQ2440��������4������������LED����
               �����ܣ��ֱ���GPB5--LED1��GPB6��LED2
               GPB7--LED3��GPB8--LED4���ú���������
               �Ƕ�������Ž��г�ʼ����
Return		 : void
Argument     : void
Autor & date : Daniel
**************************************************/   
void Led_port_init(void)
{
   /*����GPB5-GPB8Ϊ����˿�*/
   rGPBCON &= ~((3<<10)|(3<<12)|(3<<14)|(3<<16));
   
   rGPBCON |= (1<<10)|(1<<12)|(1<<14)|(1<<16);
   
}

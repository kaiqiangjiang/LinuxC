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


int Wait_key(void);
void key_init(void);
void Led1_run(void);
void Led1_init(void);

/*************************************************
Function name: delay
Parameter    : times
Description	 : 延时函数
Return		 : void
Argument     : void
Autor & date :
**************************************************/
void delay(int times)
{
    int i,j;
    for(i=0;i<times;i++)
       for(j=0;j<400;j++);
}


/*************************************************
Function name: Main
Parameter    : void
Description	 : 主功能函数
Return		 : void
Argument     : void
Autor & date :
**************************************************/
void Main(void)
{	
   Led1_init();
   
   while(1)
   {
        if(Wait_key())
           Led1_run();
   }
}	

void key_init(void) /*为什么这个函数没有被调用？*/
{
   rGPFCON &= ~(3<<0);
}

void Led1_init(void)
{
   rGPBCON &= ~(3<<10);
   rGPBCON |= (1<<10);
}

void Led1_run(void)
{
   rGPBDAT &=~(1<<5); /*LED亮还是灭？*/
   delay(1000);
   rGPBDAT |=(1<<5);
   delay(1000);
}

/*************************************************
Function name: Wait_key()
Parameter    : void
Description	 : 按键等待函数
Return		 : 返回1表示有按键动作，返回0表示无按键
Argument     : void
Autor & date : Daniel
**************************************************/
int Wait_key(void)
{
   if(!(rGPFDAT & 0x1))
   
   delay(200);
   
   if(!(rGPFDAT & 0x1)) /*为什么又要判断?*/
   {
      while(!(rGPFDAT & 0x1));
      return 1;
   }
   else
      return 0;
}
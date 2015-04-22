//=========================================================================
//  �������ƣ�	WatchDog
//  ���������� 	���ÿ��Ź���ʱ�������жϣ�����LED����˸			           	
//  ����ļ���	main.c
//  Ӳ�����ӣ�			
//  ά����¼��	2009-08-17 v1.0  by xgc
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
//  �ļ�����	main.c
//  ����������	������������
//  ά����¼:	2008-08-17 v1.0	   by xgc
//              2009-10-14 V1.1    by xgc
//=========================================================================


//=====================================================================
//	�﷨��ʽ��	int main(void)
//	ʵ�ֹ��ܣ�	���Ź�ʵ��
//	������		��
//	����ֵ��	��
//=====================================================================
int Main()
{
	unsigned int i = 0, j = 0;	 
	memcpy((unsigned char *)0x0,(unsigned char *)0x30000000,0x1000);

	SetSysFclk(FCLK_400M);  //����ϵͳʱ�� 400M     
	ChangeClockDivider(2, 1);      //���÷�Ƶ 1��4��8
	CalcBusClk();           //��������Ƶ
	
	rGPFCON = (rGPFCON | 0xFFFF) & 0xFFFFFF55;  //GPF0--GPF4����Ϊoutput       
   	rGPFUP  = rGPFUP & 0xFFF0;                  //ʹ��GPF��������
   	rGPFDAT = 0x0F;						  	 //GPF��4λ��ʼ��Ϊ1
   	
    //��WatchDog�жϼĴ���
	rSRCPND |=BIT_WDT_AC97;
    rINTPND |=BIT_WDT_AC97;
    rSUBSRCPND |= BIT_SUB_WDT; 
   
    //����WatchDog�ж�
  	pISR_WDT_AC97=(unsigned)Wdt_int;
  
	rWTCON = WDT_PRE_SCALER| WDT_ENABLE|WDT_CLK_SEL|WDT_INT_ENABLE; //���Ź��ж�ʹ��
//	rWTCON = WDT_PRE_SCALER| WDT_ENABLE|WDT_CLK_SEL|WDT_RST_ENABLE; //��Ҫ��λʱ�����  
   
    //���Ź�ι��,���Ź����ø�λ����1��
  	rWTDAT = 0x3E8;   //WTDAT���Ź����ݼĴ���,���Ź���ʱ�����صļ���ֵ
  	rWTCNT = 0x3E8;   //WTCNT���Ź������Ĵ���,���Ź���ʱ����ǰ����ֵ

    //���ж�
  	rINTMSK &= ~(BIT_WDT_AC97); 
  	rINTSUBMSK &= ~(BIT_SUB_WDT);              
	while(1);           
}
  
//===================================================
//���ƣ� Wdt_Int
//���ܣ� �ж���Ӧ����
//������ void
//����ֵ��void
//===================================================
void __irq Wdt_int(void)
{
	rGPFDAT = ~rGPFDAT;
  	if(rSUBSRCPND & ~ BIT_SUB_WDT)
  	{            //����ж�
      	rSUBSRCPND |=(BIT_SUB_WDT);   //���һ��Դ�ж�ָʾ�Ĵ�����Ӧλ 
      	rSRCPND |= BIT_WDT_AC97; 
     	rINTPND |= BIT_WDT_AC97;           //���Դ�ж�ָʾ�Ĵ�����Ӧλ
  	}

 	if(rSUBSRCPND & ~ BIT_SUB_AC97)
  	{
       	rSUBSRCPND |=BIT_SUB_AC97; 
       	rSRCPND |= BIT_WDT_AC97; 
       	rINTPND |= BIT_WDT_AC97;       //���Դ�ж�ָʾ�Ĵ�����Ӧλ
  	}
}




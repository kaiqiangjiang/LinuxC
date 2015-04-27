//====================================================================
// �ļ���  : ADC.c
// ��  ��  : ADC���Գ���
//	ά����¼��	2009-08-14	v1.0     by xgc
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
	
    rADCDLY = 100;				//ADCת����ʱ	
//    rADCTSC = 0;				//����ADCΪ��ͨģʽ
   								/* ����ADCģ�����ã�����x<<n��ʾ��nλ����Ϊx(��x����һλ�������λ˳��) */
    rADCCON = (0 << 0)     |	// ADCת������ δ����
              (0 << 1)     |    // ��AD���ݴ���ADת�� δʹ��
              (0 << 2)     |    // StandByģʽѡ�� Ϊ��ͨ����ģʽ
              (ch << 3)    |    // ADCͨ��ѡ�� ch
              (49 << 6)    | 	// CLKDIV = Fpclk /49+1/5 ����ת��ʱ��Ϊ1MHz  Fpclk = 10M   ADCת��Ƶ��400K
              (1 << 14)   ;	    // ʹ�����Ԥ��ֵ

	while(Uart_GetKey() != ESC_KEY)
	{
		AD_ENABLE();
	}
}

//=======================================================================
//���ƣ�AD_ENABLE()
//���ܣ�������λʹ�ܷ�ʽ����ADת��
//����: ��
//����ֵ: ��
//=======================================================================
 
void AD_ENABLE (void) 
 {
   	int i,j;
    int val;
 	val = 0;
	
    for(i=0;i<16;i++)
    {
		rADCCON |= 0x1;         		 //ʹ��ADC ת��
       
		while(rADCCON&0x1);    		 //�ж��Ƿ�ʹ��ADCת��
       
		while(!rADCCON&0x8000);		 //�ж�ADCת���Ƿ����
       
		val += (rADCDAT0 &0x03ff);    //ȡ��ADCת��ֵ
		for(j=0;j<500;j++);       
    }
    val = val/16;                   //����ADCƽ��ת��ֵ
    Delay(500);
    Uart_Printf("ADC val = %d\n", val);  //���͵�������ʾ
 
 }
 
 
//=======================================================================
//���ƣ�AD_READ()
//���ܣ����ö��������ķ�ʽ����ADת��
//����: ��
//����ֵ: ��
//=======================================================================
 void AD_READ (void) 
 {
   	int i,j;
    int val,aa;
 	val = 0;
 	rADCCON |= 0x2;         		 //ADCת��ͨ��������������
 	aa = rADCDAT0 &0x03ff; 			 //����ADCת��
 	
    for(i=0;i<16;i++)
    {
		while(!rADCCON&0x8000);		 //�ж�ADCת���Ƿ����
       
		val += (rADCDAT0 &0x03ff);    //ȡ��ADCת��ֵ
		for(j=0;j<500;j++);       
    }
    val = val/16;                   //����ADCƽ��ת��ֵ
    Delay(500);
    Uart_Printf("ADC val = %d\n", val);  //���͵�������ʾ
 
 }
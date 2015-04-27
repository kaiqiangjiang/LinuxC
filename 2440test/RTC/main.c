//=========================================================================
//  �������ƣ�	RTC.mcp
//  �ļ����ƣ�  main.c
//  ���������� 	ͨ�������ն˿��Կ���ÿ��ˢ�µ�ʵʱʱ�ӣ�ͬʱLED1������˸��
//              �������ӹ��ܣ����ӽ���ʱ��LED2��������ͬʱ�����ն����ַ�����ʾ��
//  ����ļ���	main.c uart.c 2440lib.c 2440init.s 2440slib.s 
//    ͷ�ļ���  2440addr.h def.h option.h 2440lib.h 2440slib.h uart.h
//  Ӳ�����ӣ�			
//  ά����¼��	2009-08-14  v1.0	by xgc
//              2009-10-15  v1.1    by xgc
//=========================================================================

#include "2440addr.h"
#include "2440lib.h"
#include "def.h"
#include "uart.h"
#include <stdio.h>
#include <string.h>

//====================================================
// ����������
//====================================================

void __irq RTC_tickHandler(void);
void __irq RTC_alarmHandler(void);

//====================================================
// ����������
//====================================================
U8 led_index;               //������ˢ�±�ʶ
U8 alarmflag = 0;            //���ӱ�����ʶ

typedef struct ST_DATE{      //��ʾ����ʱ������ݽṹ
	short year;
	U8 month;
	U8 day;
	U8 week_day;
	U8 hour;
	U8 minute;
	U8 second;
	}st_date;
char *date[7] = { "SUN","MON", "TUES", "WED", "THURS","FRI", "SAT" } ; //���ڼ� 

//====================================================
// ����������
//====================================================
//====================================================
// �﷨��ʽ��void RTC_setdate(st_date *p_date)
// ��������: ����ʵʱʱ�ӵ�ǰ���ڡ� ʱ��
// ��ڲ���: ��������ʱ��
// ���ڲ���: ��
//====================================================
void RTC_setdate(st_date *p_date)
{
	rRTCCON = 0x01;	             //RTC��дʹ�ܣ�ѡ��BCDʱ�ӡ����������޸�λ��1/32768
	rBCDYEAR = p_date->year;
	rBCDMON = p_date->month;
	rBCDDAY = p_date->day;
	rBCDDATE = p_date->week_day;
	rBCDHOUR = p_date->hour;
	rBCDMIN = p_date->minute;
	rBCDSEC = p_date->second;
	rRTCCON = 0x00;             //RTC��д��ֹ��ѡ��BCDʱ�ӡ����������޸�λ��1/32768
}

//====================================================
// �﷨��ʽ��void RTC_getdate(st_date *p_date)
// ��������: ��ȡʵʱʱ�ӵ�ǰ���ڡ� ʱ��
// ��ڲ���: ��������ʱ���ָ��
// ���ڲ���: ��
//====================================================
void RTC_getdate(st_date *p_date)
{
	rRTCCON = 0x01;             //RTC��дʹ�ܣ�ѡ��BCDʱ�ӡ����������޸�λ��1/32768
	p_date->year = rBCDYEAR;
	p_date->month = rBCDMON;
	p_date->day = rBCDDAY;
	p_date->week_day = rBCDDATE;
	p_date->hour = rBCDHOUR;
	p_date->minute = rBCDMIN;
	p_date->second = rBCDSEC;
	rRTCCON = 0x00;             //RTC��д��ֹ��ѡ��BCDʱ�ӡ����������޸�λ��1/32768
}

//====================================================
// �﷨��ʽ��void RTC_tickIRQ_init(U8 tick)
// ��������: TICK �жϳ�ʼ��
// ��ڲ���: tick��ʼֵ
// ���ڲ���: ��
//====================================================

void RTC_tickIRQ_init(U8 tick)
{
   	pISR_TICK = (unsigned)RTC_tickHandler;	     //�жϺ�������ڵ�ַ
	EnableIrq(BIT_TICK);//open RTC TICK INTERRUPT
   	rRTCCON = 0x00;                               //����RTC��д
   	rTICNT = (tick&0x7f)|0x80;	//����tick������ ���ж�ʹ��   (1+n)/128
}

//====================================================
// �﷨��ʽ��void RTC_alarm_setdate(st_date *p_date,U8 mode)
// ��������: �����������ڡ� ʱ��
// ��ڲ���: ��������ʱ�估�����ӻ���ģʽ
// ���ڲ���: ��
//====================================================
void RTC_alarm_setdate(st_date *p_date,U8 mode)
{
	rRTCCON = 0x01;
	rALMYEAR = p_date->year;
	rALMMON = p_date->month;
	rALMDAY = p_date->day;	
	rALMHOUR = p_date->hour;
	rALMMIN = p_date->minute;
	rALMSEC = p_date->second;
	rRTCALM = mode;
	rRTCCON = 0x00;                
	pISR_RTC = (unsigned)RTC_alarmHandler;	
    EnableIrq(BIT_RTC); //open RTC alarm  INTERRUPT
}

//====================================================
// �﷨��ʽ��void Main(void)
// ��������: ����ʵʱʱ�Ӽ�����ʱ�䣬��ʼ��RTC��alarm�ж�
// ��ڲ���: 
// ���ڲ���: ��
//====================================================

void Main(void)
{
    st_date m_date;
    U8 old_index;

	memcpy((unsigned char *)0x0,(unsigned char *)0x30000000,0x1000);
    
	SetSysFclk(FCLK_400M);  //����ϵͳʱ�� 400M     
	ChangeClockDivider(2, 1);      //���÷�Ƶ 1��4��8
	CalcBusClk();           //��������Ƶ

		
	rGPFCON = (rGPFCON | 0xFFFF) & 0xFFFFFF55;  //GPF0--GPF3����Ϊoutput       
   	rGPFUP  = rGPFUP & 0xFFF0;                  //ʹ��GPF��������
   	rGPFDAT = 0x0F;						  	 //GPF��4λ��ʼ��Ϊ1

	Uart_Select(0);    
	Uart_Init(0, 115200);   //���ö˿� ������115200  �����������   ����λ= 8λ 
    
    Uart_Printf("\n ---ʵʱʱ�Ӳ��Գ���---\n");
   

                               //���õ�ǰ���� ʱ��
    m_date.year =  0x09;    
    m_date.month = 0x10;
    m_date.day = 0x15;
    m_date.week_day = 0x04;
    m_date.hour = 0x13;
    m_date.minute = 0x32;
    m_date.second = 0x30;
    
    RTC_setdate(&m_date);     //����ʵʱʱ��
    
    m_date.second = 0x10;     //�޸�alarm����
    
    RTC_alarm_setdate(&m_date,0x41);//���ø澯��ʱ�估��ʽ��0x41��ʾʹ��RTC�澯���Լ�ʹ����ʱ�ӱ���
    
    RTC_tickIRQ_init(127);          // ����1����tickһ��
    
  
    while(1)
    {
    	if(old_index != led_index)
    	{   						   //���Ƿ�ˢ��
    		RTC_getdate(&m_date);      //��ȡ��ǰ����ʱ��
    		m_date.year += 0x2000;   
    		old_index = led_index;     //���±�ʶ
    		Uart_Printf("RTC TIME : %04x/%02x/%02x  %s - %02x:%02x:%02x\r",m_date.year,m_date.month,m_date.day,date[m_date.week_day],m_date.hour,m_date.minute,m_date.second);
    	}
    	
    	if(alarmflag)
    	{                			  //�Ƿ񱨾�
    	 	alarmflag = 0;               //���±�ʶ
    	 	Uart_Printf("\nRTC ALARM  %02x:%02x:%02x \n",m_date.hour,m_date.minute,m_date.second);
        }
    }
}

//=================================================================
// �﷨��ʽ��void __irq RTC_tickHandler(void)
// ��������: ʵʱʱ��tick�жϺ���������index��ʶ�����ö˿ڣ�LED1��˸��
// ��ڲ���: 
// ���ڲ���: ��
//==================================================================
void __irq RTC_tickHandler(void)
{
    rGPFDAT = ~(rGPFDAT^0xe);     //ˢ��LED1
    led_index ++;
    ClearPending(BIT_TICK);
} 

//====================================================
// �﷨��ʽ��void __irq RTC_alarmHandler(void)
// ��������: alarm�жϺ��������ö˿�(LED2��������
// ��ڲ���: 
// ���ڲ���: ��
//==================================================== 
void __irq RTC_alarmHandler(void)
{
   rGPFDAT = (rGPFDAT^0x2);      //ˢ��LED2
   alarmflag = 1;
   ClearPending(BIT_RTC);
}
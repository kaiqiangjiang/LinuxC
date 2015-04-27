//====================================================================================
//�� �� ��:    TouchPanel.c
//��������:    ��������������
//ά����¼:	   2009-08-18	V1.0   by xgc
//====================================================================================

#include "def.h"
#include "2440lib.h"
#include "2440addr.h"
#include "option.h"
#include "uart.h"
#include "TFT_API.h"
#include "TouchPanel.h"

#define LOOP  50
#define ADCPRS 39

short LeftBound = TOUCH_DEFAULT_LB;							// ���Ե��Ӧ��ADת��ֵ(16bit)
short RightBound = TOUCH_DEFAULT_RB;						// �ұ�Ե��Ӧ��ADת��ֵ(16bit)
short TopBound = TOUCH_DEFAULT_TB;							// �ϱ�Ե��Ӧ��ADת��ֵ(16bit)
short BottomBound = TOUCH_DEFAULT_BB;						// �±�Ե��Ӧ��ADת��ֵ(16bit)

unsigned int TempADCx;
unsigned int TempADCy;
short ADC_Counter;
short Debounce_Counter;
short DebounceX;
short DebounceY;
short ResultX;
short ResultY;

int count=0;
volatile int xdata, ydata;

void __irq Adc_or_TsSep(void);

//=============================================================
//�﷨��ʽ��void Touch_Init(void)
//ʵ�ֹ��ܣ�������������ʼ������ʼ��������ADC��IO�ȣ�������ADC�ж�
//������	��
//����ֵ��	��
//=============================================================
void Touch_Init(void)
{
	TempADCx = 0;
	TempADCy = 0;
	ADC_Counter = 0;
	Debounce_Counter = 0;
	DebounceX = -1;
	DebounceY = -1;
	ResultX = -1;
	ResultY = -1;
	
	rGPGCON |= 0xff000000;
	rGPGUP &= 0x0f;
	
	rADCDLY = (50000);  // ADC Start or Interval Delay

    rADCCON = (1<<14)|(ADCPRS<<6)|(0<<3)|(0<<2)|(0<<1)|(0);
      // Enable Prescaler,Prescaler,AIN7/5 fix,Normal,Disable read start,No operation
    rADCTSC = (0<<8)|(1<<7)|(1<<6)|(0<<5)|(1<<4)|(0<<3)|(0<<2)|(3);
      // Down,YM:GND,YP:AIN5,XM:Hi-z,XP:AIN7,XP pullup En,Normal,Waiting for interrupt mode
  
    if(rSRCPND &BIT_ADC) rSRCPND |= BIT_ADC;
    if(rINTPND & BIT_ADC) rINTPND |= BIT_ADC;
    if(rSUBSRCPND & BIT_SUB_TC) rSUBSRCPND|= BIT_SUB_TC;
    if(rSUBSRCPND & BIT_SUB_ADC_S)rSUBSRCPND |= BIT_SUB_ADC_S;
    rINTMSK &=~(BIT_ADC);
    rINTSUBMSK &=~(BIT_SUB_TC);
    pISR_ADC   = (unsigned)Adc_or_TsSep;
	
}

//=============================================================
//�﷨��ʽ��void Touch_SetBound(unsigned short left, unsigned short right, unsigned short top, unsigned short bottom);
//ʵ�ֹ��ܣ������ĸ���Ե��Ӧ��ADת��ֵ�����ڴ�����У׼
//������	left:	���Ե��Ӧ��ADת��ֵ(16bit)
//			right:	�ұ�Ե��Ӧ��ADת��ֵ(16bit)
//			top:	�ϱ�Ե��Ӧ��ADת��ֵ(16bit)
//			bottom:	�±�Ե��Ӧ��ADת��ֵ(16bit)
//����ֵ��	��
//=============================================================
void Touch_SetBound(unsigned short left, unsigned short right, unsigned short top, unsigned short bottom)
{
	LeftBound = left;
	RightBound = right;
	TopBound = top;
	BottomBound = bottom;
}

//=============================================================
//�﷨��ʽ��void __irq Adc_or_TsSep(void);
//ʵ�ֹ��ܣ�TC�жϷ����ӳ���
//������	��
//����ֵ��	��
//=============================================================
void __irq Adc_or_TsSep(void)
{
	unsigned int i;
	U32 PtX[6], PtY[6];
	short TempX, TempY;

	if(rADCDAT0&0x8000)
	{
		//Uart_Printf("\nStylus Up!!\n");
		rADCTSC&=0xff;	// Set stylus down interrupt bit
	}
	//else 
		//Uart_Printf("\nStylus Down!!\n");

	rADCTSC=(1<<3)|(1<<2);         //Pull-up disable, Seq. X,Y postion measure.

	rADCCON|=0x1;                   //start ADC

	for (i = 0; i < 5; i++)
	{
		while(rADCCON & 0x1);		//check if Enable_start is low
		while(!(rADCCON & 0x8000));        //check if EC(End of Conversion) flag is high, This line is necessary~!!
			
		while(!(rSRCPND & (BIT_ADC)));  //check if ADC is finished with interrupt bit

		PtX[i]=(rADCDAT0&0x3ff);
	 	PtY[i]=(rADCDAT1&0x3ff);
	}
	PtX[5]=(PtX[0]+PtX[1]+PtX[2]+PtX[3]+PtX[4])/5;
	PtY[5]=(PtY[0]+PtY[1]+PtY[2]+PtY[3]+PtY[4])/5;
	TempADCx = PtX[5];
	TempADCy = PtY[5];
	
	//check Stylus Up Interrupt.
	rSUBSRCPND|=BIT_SUB_TC;
	ClearPending(BIT_ADC);
	rINTSUBMSK=~(BIT_SUB_TC);
	rINTMSK=~(BIT_ADC);
			 
	rADCTSC =0xd3;    //Waiting for interrupt
	rADCTSC=rADCTSC|(1<<8); // Detect stylus up interrupt signal.

	while(1)		//to check Pen-up state
	{
		if(rSUBSRCPND & (BIT_SUB_TC))	//check if ADC is finished with interrupt bit
		{
			//Uart_Printf("Stylus Up Interrupt~!\n");
			break;	//if Stylus is up(1) state
		}
	}

//	Uart_Printf("count=%03d  XP=%04d, YP=%04d\n", count++, TempADCx, TempADCy);    //X-position Conversion data            

	rADCTSC=rADCTSC&~(1<<8); // Detect stylus Down interrupt signal.
	rSUBSRCPND|=BIT_SUB_TC;
	rINTSUBMSK=~(BIT_SUB_TC);	// Unmask sub interrupt (TC)     
	ClearPending(BIT_ADC);
	
	TempX = TOUCH_HEIGHT * (TOUCH_DEFAULT_RB - TempADCx) / (TOUCH_DEFAULT_RB - TOUCH_DEFAULT_LB);
	 
	TempY = TOUCH_WIDTH * (TempADCy - TOUCH_DEFAULT_TB) / (TOUCH_DEFAULT_BB - TOUCH_DEFAULT_TB);	
	 
	ResultX = (TempX );		// ��������
	ResultY = (TempY );		
}

//=============================================================
//�﷨��ʽ��short Touch_Get(short *x, short *y);
//ʵ�ֹ��ܣ���������ȡ�������꺯��
//������	*x: ������ĺ�����; *y: �������������
//����ֵ��	��
//=============================================================
short Touch_Get(short *x, short *y)
{
    if(ResultX != -1)
    {
        *x = ResultX;
        *y = ResultY;
        
        ResultX = -1;
        ResultY = -1;
        
        return 0;
    }
    else
    {
        *x = -1;
        *y = -1;
        return -1;
    }
}

void Test_TouchPanel(void)
{
    short X_Position[1],Y_Position[1];			// ���淵�صĴ�����λ��
	short x,y;	

	Uart_Printf("\nTouchPanel Test\n");	
	TFT_Init();
 	Touch_Init();
	TFT_SetColor(COLOR_BLACK);
	
	while(Uart_GetKey() != ESC_KEY)
	{
		if(Touch_Get(X_Position,Y_Position)!=-1)// ��ȡTouch Panel����
		{
			x = X_Position[0];					// ����д����ʵ��У�����X/Y����
			y = Y_Position[0];
			y = 240 - y;							// ����ʵ�������������ʾ����
	
			TFT_Circle(x,y,8,COLOR_WHITE,PAINT_SOLID);// ��ʾԲ��
		
			Uart_Printf("x=%d, y=%d\n",x,y);
		}	
	}
	Uart_Printf("Test End!\n");
}


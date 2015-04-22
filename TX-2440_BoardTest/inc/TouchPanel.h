#ifndef _TOUCHPANEL_H_
#define _TOUCHPANEL_H_

#define	TOUCH_X1_BIT		5						// X1��ADC�˿�
#define TOUCH_X2_BIT		7						// X2��ADC�˿�
#define TOUCH_Y1_BIT		4						// Y1��ADC�˿�
#define TOUCH_Y2_BIT		6						// Y2��ADC�˿�

#define TOUCH_WIDTH			240						// ��������ˮƽ�ֱ���
#define	TOUCH_HEIGHT		320						// �������Ĵ�ֱ�ֱ���

#define TOUCH_DEFAULT_LB	0x50					// ���Ե��Ӧ��Ĭ��ADת��ֵ(16bit)
#define TOUCH_DEFAULT_RB	0x3A2					// �ұ�Ե��Ӧ��Ĭ��ADת��ֵ(16bit)
#define TOUCH_DEFAULT_TB	0x7D					// �ϱ�Ե��Ӧ��Ĭ��ADת��ֵ(16bit)
#define TOUCH_DEFAULT_BB	0x370				// �±�Ե��Ӧ��Ĭ��ADת��ֵ(16bit)

#define TOUCH_DEBOUNCE_PIXEL	8					// ȥ�����������������������ȶ��ڰ뾶Ϊ�������صķ�Χ�ڲ�������
#define TOUCH_DEBOUNCE_TIME		5					// ȥ����ʱ�䣬��λΪ32*ADCת��ʱ��

///////////////////////////////////////////////////////////////////
//	�����޸����������
#define TOUCH_X1			(0x0001<<TOUCH_X1_BIT)
#define TOUCH_X2			(0x0001<<TOUCH_X2_BIT)
#define TOUCH_Y1			(0x0001<<TOUCH_Y1_BIT)
#define TOUCH_Y2			(0x0001<<TOUCH_Y2_BIT)

void Touch_Init(void);
short Touch_Get(short *x, short *y);
void Touch_SetBound(unsigned short left, unsigned short right, unsigned short top, unsigned short bottom);
void Test_TouchPanel(void);

#endif

#ifndef _TOUCHPANEL_H_
#define _TOUCHPANEL_H_

#define	TOUCH_X1_BIT		5						// X1的ADC端口
#define TOUCH_X2_BIT		7						// X2的ADC端口
#define TOUCH_Y1_BIT		4						// Y1的ADC端口
#define TOUCH_Y2_BIT		6						// Y2的ADC端口

#define TOUCH_WIDTH			240						// 触摸屏的水平分辨率
#define	TOUCH_HEIGHT		320						// 触摸屏的垂直分辨率

#define TOUCH_DEFAULT_LB	0x50					// 左边缘对应的默认AD转换值(16bit)
#define TOUCH_DEFAULT_RB	0x3A2					// 右边缘对应的默认AD转换值(16bit)
#define TOUCH_DEFAULT_TB	0x7D					// 上边缘对应的默认AD转换值(16bit)
#define TOUCH_DEFAULT_BB	0x370				// 下边缘对应的默认AD转换值(16bit)

#define TOUCH_DEBOUNCE_PIXEL	8					// 去抖动像素数，即触摸坐标稳定在半径为多少像素的范围内才输出结果
#define TOUCH_DEBOUNCE_TIME		5					// 去抖动时间，单位为32*ADC转换时间

///////////////////////////////////////////////////////////////////
//	不必修改下面的内容
#define TOUCH_X1			(0x0001<<TOUCH_X1_BIT)
#define TOUCH_X2			(0x0001<<TOUCH_X2_BIT)
#define TOUCH_Y1			(0x0001<<TOUCH_Y1_BIT)
#define TOUCH_Y2			(0x0001<<TOUCH_Y2_BIT)

void Touch_Init(void);
short Touch_Get(short *x, short *y);
void Touch_SetBound(unsigned short left, unsigned short right, unsigned short top, unsigned short bottom);
void Test_TouchPanel(void);

#endif

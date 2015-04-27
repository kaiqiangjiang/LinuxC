#ifndef _TFT_DRIVER_H_
#define _TFT_DRIVER_H_

#include "TFT_Config.h"

#define MVAL		(13)
#define MVAL_USED 	(0)		//0=each frame   1=rate by MVAL
#define INVVDEN		(1)		//0=normal       1=inverted
#define BSWP		(0)		//Byte swap control
#define HWSWP		(1)		//Half word swap control

#define M5D(n) ((n) & 0x1fffff)	// To get lower 21bits

//TFT 320240

#define LCD_XSIZE_TFT_320240 	(320)	
#define LCD_YSIZE_TFT_320240 	(240)

//Timing parameter for LCD
#define VBPD		(12)	//垂直同步信号的后肩
#define VFPD		(4)	//垂直同步信号的前肩
#define VSPW		(5)		//垂直同步信号的脉宽

#define HBPD		(22)	//水平同步信号的后肩
#define HFPD		(33)	//水平同步信号的前肩
#define HSPW		(44)	//水平同步信号的脉宽

//TFT 320240
#define SCR_XSIZE_TFT_320240 	(LCD_XSIZE_TFT_320240)
#define SCR_YSIZE_TFT_320240 	(LCD_YSIZE_TFT_320240)

#define CLKVAL_TFT	6


#if TFT_COLOR_RESV
	#define 	COLOR_WHITE		0x0000
	#define		COLOR_BLACK		0xFFFF
	#define		COLOR_RED		0x07FF
	#define		COLOR_GREEN		0xF81F
	#define		COLOR_BLUE		0xFFE0
	#define		COLOR_MAGENTA	0x07E0
	#define		COLOR_CYAN		0xF810
	#define		COLOR_YELLOW	0x001F
#else
	#define 	COLOR_WHITE		0xFFFF
	#define		COLOR_BLACK		0x0000
	#define		COLOR_RED		0xF800
	#define		COLOR_GREEN		0x07E0
	#define		COLOR_BLUE		0x001F
	#define		COLOR_MAGENTA	0xF81F
	#define		COLOR_CYAN		0x07FF
	#define		COLOR_YELLOW	0xFFE0
#endif

#define     PAINT_SOLID     1
#define     PAINT_HOLLOW    0


void TFT_InitHardware(void);
void Lcd_PowerEnable(int invpwren,int pwren);
void Lcd_EnvidOnOff(int onoff);



#endif

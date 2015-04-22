#ifndef _TFT_API_H_
#define _TFT_API_H_

#include "TFT_Driver.h"
//#include "TFT_Font.h"

typedef unsigned short COLOR;

typedef struct {
	unsigned short Width;
	unsigned short Height;
	void *ImageBuf;
}IMAGE;

void TFT_Init(void);

void TFT_SetColor(unsigned short color);
void TFT_ClearSrc(void);
void TFT_PutPixel(short x, short y, unsigned short color);
void TFT_Line(short x1, short y1, short x2, short y2, unsigned short color);
void TFT_Circle(unsigned x,unsigned y,unsigned r,unsigned short color,unsigned Mode);
void TFT_Rectangle(unsigned x1, unsigned y1, unsigned x2, unsigned y2, unsigned short color, unsigned Mode);
void TFT_Pitcure(int x0,int y0,int h,int l,unsigned char bmp[]);
void Test_TFT(void);

extern unsigned char Image[];

#endif  //_TFT_API_H_

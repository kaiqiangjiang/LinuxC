//====================================================================================
//文 件 名：TFT_API.c
//功能描述: TFT驱动程序(API)
//维护记录: 2009年8月18日 V1.0   by xgc
//====================================================================================

#include "TFT_API.h"
#include "def.h"
#include "2440lib.h"
#include "uart.h"

extern volatile unsigned short LCD_BUFFER[SCR_YSIZE_TFT_320240][SCR_XSIZE_TFT_320240];

//=============================================================
//语法格式：	void TFT_Init(void);
//实现功能：	TFT驱动程序初始化
//参数：		无
//返回值：		无
//=============================================================
void TFT_Init(void)
{
	TFT_InitHardware();
	Lcd_PowerEnable(0, 1);
	Lcd_EnvidOnOff(1);		//turn on vedio
	
	TFT_ClearSrc();
}

//=============================================================
//语法格式：	void TFT_SetColor(COLOR color)
//实现功能：	设置屏幕颜色
//参数：		color -- 颜色值
//返回值：		无
//=============================================================
void TFT_SetColor(COLOR color)
{
	unsigned int x,y ;
		
    for( y = 0; y < SCR_YSIZE_TFT_320240; y++ )
    {
    	for( x = 0; x < SCR_XSIZE_TFT_320240; x++ )
    	{
			LCD_BUFFER[y][x] = color;
    	}
    }
}

//=============================================================
//语法格式：	void TFT_ClearSrc(void)
//实现功能：	清屏函数（黑屏）
//参数：		无
//返回值：		无
//=============================================================
void TFT_ClearSrc(void)
{
	TFT_SetColor(COLOR_BLACK);	
}

//=============================================================
//语法格式：	void TFT_PutPixel(short x, short y, COLOR color);
//实现功能：	在屏幕中指定位置画点
//参数：		
//				x - x坐标
//				y - y坐标
//				color - 点的颜色
//返回值：		无
//=============================================================
void TFT_PutPixel(short x, short y, COLOR color)
{
	if ( (x < SCR_XSIZE_TFT_320240) && (y < SCR_YSIZE_TFT_320240) )
		LCD_BUFFER[(y)][(x)] = color;
}

//=============================================================
//语法格式：	void TFT_Line(short x1, short y1, short x2, short y2, COLOR color)
//实现功能：	在屏幕中画直线
//参数：
//				x1 - 起始点的x坐标
//				y1 - 起始点的y坐标
//				x2 - 结束点的x坐标
//				y2 - 结束点的y坐标
//              color - 直线颜色
//返回值：		无
//=============================================================
void TFT_Line(short x1, short y1, short x2, short y2, COLOR color)
{
	short dx, dy, e;
	dx = x2 - x1; 
	dy = y2 - y1;
    
	if(dx >= 0)
	{
		if(dy >= 0) // dy>=0
		{
			if(dx >= dy) // 1/8 octant
			{
				e = dy-dx/2;
				while(x1 <= x2)
				{
					TFT_PutPixel(x1,y1,color);
					
					if(e > 0)
					{
						y1 += 1;
						e -= dx;
					}
						
					x1 += 1;
					e += dy;
				}
			}
			
			else		// 2/8 octant
			{
				e = dx-dy/2;
				
				while(y1 <= y2)
				{
					TFT_PutPixel(x1, y1, color);
					if(e > 0)
					{
						x1 += 1;
						e -= dy;
					}	
					y1 += 1;
					e += dx;
				}
			}
		}
		
		else		   // dy<0
		{
			dy = -dy;   // dy=abs(dy)

			if(dx >= dy) // 8/8 octant
			{
				e = dy-dx/2;
				
				while(x1 <= x2)
				{
					TFT_PutPixel(x1,y1,color);
					if(e > 0)
					{
						y1 -= 1;
						e -= dx;
					}	
					x1 += 1;
					e += dy;
				}
			}
			
			else		// 7/8 octant
			{
				e = dx-dy/2;
				while(y1 >= y2)
				{
					TFT_PutPixel(x1, y1, color);
					if(e > 0)
					{
						x1 += 1;
						e -= dy;
					}	
					y1 -= 1;
					e += dx;
				}
			}
		}	
	}
	
	else //dx<0
	{
		dx = -dx;		//dx=abs(dx)
		if(dy >= 0) // dy>=0
		{
			if(dx >= dy) // 4/8 octant
			{
				e = dy-dx/2;
				while(x1 >= x2)
				{
					TFT_PutPixel(x1, y1, color);
					if(e > 0)
					{
						y1 += 1;
						e -= dx;
					}	
					x1 -= 1;
					e += dy;
				}
			}
			
			else		// 3/8 octant
			{
				e = dx-dy/2;
				while(y1 <= y2)
				{
					TFT_PutPixel(x1, y1, color);
					if(e > 0)
					{
						x1 -= 1;
						e -= dy;
					}	
					y1 += 1;
					e += dx;
				}
			}
		}
		
		else		   // dy<0
		{
			dy = -dy;   // dy=abs(dy)

			if(dx >= dy) // 5/8 octant
			{
				e = dy-dx/2;
				while(x1 >= x2)
				{
					TFT_PutPixel(x1, y1, color);
					if(e > 0)
					{
						y1 -= 1;
						e -= dx;
					}	
					x1 -= 1;
					e += dy;
				}
			}
			else		// 6/8 octant
			{
				e = dx-dy/2;
				while(y1 >= y2)
				{
					TFT_PutPixel(x1, y1 ,color);
					if(e > 0)
					{
						x1 -= 1;
						e -= dy;
					}	
					y1 -= 1;
					e += dx;
				}
			}
		}	
	}	
}

//=============================================================
//语法格式：	void TFT_Circle(unsigned x,unsigned y,unsigned r,COLOR color,unsigned Mode);
//实现功能：	在屏幕中画圆
//参数：		
//				x - 圆心的x坐标
//				y - 圆心的y坐标
//				r - 圆的半径
//              color - 圆的颜色
//				Mode -	0(PAINT_HOLLOW)	空心圆
//						1(PAINT_SOLID)	实心圆
//返回值：		无
//=============================================================
void TFT_Circle(unsigned x,unsigned y,unsigned r,COLOR color,unsigned Mode)
{
	int dx,dy,ref;
	int Temp;
	
	if(r == 0)
		return;
	dx = r;
	dy = 1;
	ref = 707 * r / 1000;					// 参考值为0.707r

	if(Mode == PAINT_SOLID)					// 实心圆
	{
		while(dy <= dx)
		{
			if(dx > ref)
				TFT_Line(x + ref + 1, y + dy, x + dx, y + dy, color);

			if(x >= ref + 1 && dx > ref)
			{
				if(x < dx)
					Temp = 0;
				else
					Temp = x - dx;
				TFT_Line(Temp, y + dy, x - ref - 1, y + dy, color);
			}
			
			if(y >= dy && dx > ref)
				TFT_Line(x + ref + 1, y - dy, x + dx, y - dy, color);
			if(x >= ref + 1 && y >= dy && dx > ref)
			{
				if(x < dx)
					Temp = 0;
				else
					Temp = x - dx;
				TFT_Line(Temp, y - dy, x - ref - 1, y - dy, color);
			}

			if(dy != dx || dx == ref)
			{
				if(y < dx)
					Temp = 0;
				else
					Temp = y - dx;
				TFT_Line(x + dy, Temp, x + dy, y + dx, color);
			}

			if(x >= dy && (dy != dx || dy == ref))
			{
				if(y < dx)
					Temp = 0;
				else
					Temp = y - dx;
				TFT_Line(x - dy, Temp, x - dy, y + dx, color);
			}			
			dy++;
			if((r*r-dy*dy)<=(dx-1)*dx)
				dx--;
		}

		TFT_Line(x + ref + 1, y, x + r, y, color);

		if(x >= ref + 1)
		{
			if(x < r)
				Temp = 0;
			else
				Temp = x - r;
			TFT_Line(Temp, y, x - ref - 1, y, color);
		}

		if(y < r)
			Temp = 0;
		else
			Temp = y - r;
		TFT_Line(x, Temp, x, y + r, color);
	}
	else													// 空心圆
	{
		while(dy <= dx)
		{
			TFT_PutPixel(x + dx, y + dy, color);
			TFT_PutPixel(x - dx, y + dy, color);
			TFT_PutPixel(x + dx, y - dy, color);
			TFT_PutPixel(x - dx, y - dy, color);
			if(dx != dy)
			{
				TFT_PutPixel(x + dy, y + dx, color);
				TFT_PutPixel(x + dy, y - dx, color);
				TFT_PutPixel(x - dy, y + dx, color);
				TFT_PutPixel(x - dy, y - dx, color);
			}
			dy++;
			if((r*r-dy*dy)<=(dx-1)*dx)
				dx--;
		}
		TFT_PutPixel(x + r, y, color);
		TFT_PutPixel(x - r, y, color);
		TFT_PutPixel(x, y + r, color);
		TFT_PutPixel(x, y - r, color);
	}	
}

//=============================================================
//语法格式：	void TFT_Rectangle(unsigned x1, unsigned y1, unsigned x2, unsigned y2, COLOR color, unsigned Mode);
//实现功能：	在屏幕中画矩形
//参数：
//				x1 - 矩形的左上角x坐标
//				y1 - 矩形的左上角y坐标
//				x2 - 矩形的右下角x坐标
//				y2 - 矩形的右下角y坐标
//    			color - 填充颜色
//				Mode -	0(PAINT_HOLLOW)	空心矩形
//						1(PAINT_SOLID)	实心矩形
//返回值：		无
//=============================================================
void TFT_Rectangle(unsigned x1, unsigned y1, unsigned x2, unsigned y2, COLOR color, unsigned Mode)
{
	int i;

	if(x1>x2)
	{
		i = x1;
		x1 = x2;
		x2 = i;
	}

	if(y1>y2)
	{
		i = y1;
		y1 = y2;
		y2 = i;
	}

	if(Mode == PAINT_SOLID)									// 实心矩形
	{
		for(i=y1; i<=y2; i++)
		{
			TFT_Line(x1, i, x2, i, color);
		}
	}
	
	else													// 空心矩形
	{
		TFT_Line(x1, y1, x2, y1, color);
		TFT_Line(x1, y2, x2, y2, color);
		TFT_Line(x1, y1, x1, y2, color);
		TFT_Line(x2, y1, x2, y2, color);
	}
}

//=============================================================
//语法格式：	void TFT_Pitcure(int x0,int y0,int h,int l,unsigned char bmp[])
//实现功能：	在屏幕上指定坐标画一个指定大小的图片
//参数：
//				x0 - 图片左上角x坐标
//				y0 - 图片左上角y坐标
//				h - 图片的高
//				l - 图片的长
//              bmp[] - 图片的模
//返回值：		无
//注：用Image2LCD取模  水平扫描 16位真色彩 高位在前
//=============================================================
void TFT_Pitcure(int x0,int y0,int height,int width,unsigned char bmp[])
{
	int x,y;
	U32 c;
	int p = 0;
	
    for( y = y0 ; y < width ; y++ )
    {
    	for( x = x0 ; x < height ; x++ )
    	{
    		c = bmp[p+1] | (bmp[p]<<8) ;

			if ( ( (x0+x) <SCR_XSIZE_TFT_320240) && ( (y0+y) < SCR_YSIZE_TFT_320240) )
				LCD_BUFFER[y0+y][x0+x] = c ;
			
    		p = p + 2 ;
    	}
    }
}

void Test_TFT(void)
{
	Uart_Printf("\nTFT Test!!\n");
	TFT_Init();
	
	TFT_SetColor(COLOR_WHITE);
	Delay(1000);	
	TFT_SetColor(COLOR_RED);
	Delay(1000);
	TFT_SetColor(COLOR_GREEN);
	Delay(1000);
	TFT_SetColor(COLOR_BLUE);
	Delay(1000);
	TFT_SetColor(COLOR_YELLOW);
	Delay(1000);
	TFT_SetColor(COLOR_BLACK);
	Delay(500);
	TFT_PutPixel(20,20,COLOR_WHITE);
	TFT_PutPixel(60,20,COLOR_RED);
	TFT_PutPixel(40,20,COLOR_WHITE);
	TFT_PutPixel(80,20,COLOR_BLUE);
	TFT_PutPixel(100,20,COLOR_WHITE);
	TFT_PutPixel(120,20,COLOR_YELLOW);
	Delay(500);
	TFT_Line(30,30,300,30,COLOR_RED);
	Delay(500);
	TFT_Circle(50,100,35,COLOR_GREEN,PAINT_SOLID);
	Delay(500);
	TFT_Circle(200,200,30,COLOR_YELLOW,PAINT_HOLLOW);
	Delay(500);
	TFT_Rectangle(30,180,80,230,COLOR_BLUE,PAINT_SOLID);
	Delay(500);
	TFT_Rectangle(200,80,300,150,COLOR_WHITE,PAINT_HOLLOW);
	Delay(3000);

	TFT_Pitcure(0,0,320,240,Image);
	while(Uart_GetKey() != ESC_KEY);
	Uart_Printf("\nTFT Test End!\n");
}
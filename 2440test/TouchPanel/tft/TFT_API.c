//====================================================================================
//�� �� ����TFT_API.c
//��������: TFT��������(API)
//ά����¼: 2009��8��18�� V1.0   by xgc
//====================================================================================

#include "TFT_API.h"
#include "def.h"
#include "string.h"

extern volatile unsigned short LCD_BUFFER[SCR_YSIZE_TFT_320240][SCR_XSIZE_TFT_320240];

//=============================================================
//�﷨��ʽ��	void TFT_Init(void);
//ʵ�ֹ��ܣ�	TFT���������ʼ��
//������		��
//����ֵ��		��
//=============================================================
void TFT_Init(void)
{
	TFT_InitHardware();
	Lcd_PowerEnable(0, 1);
	Lcd_EnvidOnOff(1);		//turn on vedio
	
	TFT_ClearSrc();
}

//=============================================================
//�﷨��ʽ��	void TFT_SetColor(COLOR color)
//ʵ�ֹ��ܣ�	������Ļ��ɫ
//������		color -- ��ɫֵ
//����ֵ��		��
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
//�﷨��ʽ��	void TFT_ClearSrc(void)
//ʵ�ֹ��ܣ�	����������������
//������		��
//����ֵ��		��
//=============================================================
void TFT_ClearSrc(void)
{
	TFT_SetColor(COLOR_BLACK);	
}

//=============================================================
//�﷨��ʽ��	void TFT_PutPixel(short x, short y, COLOR color);
//ʵ�ֹ��ܣ�	����Ļ��ָ��λ�û���
//������		
//				x - x����
//				y - y����
//				color - �����ɫ
//����ֵ��		��
//=============================================================
void TFT_PutPixel(short x, short y, COLOR color)
{
	if ( (x < SCR_XSIZE_TFT_320240) && (y < SCR_YSIZE_TFT_320240) )
		LCD_BUFFER[(y)][(x)] = color;
}

//=============================================================
//�﷨��ʽ��	void TFT_Line(short x1, short y1, short x2, short y2, COLOR color)
//ʵ�ֹ��ܣ�	����Ļ�л�ֱ��
//������
//				x1 - ��ʼ���x����
//				y1 - ��ʼ���y����
//				x2 - �������x����
//				y2 - �������y����
//              color - ֱ����ɫ
//����ֵ��		��
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
//�﷨��ʽ��	void TFT_Circle(unsigned x,unsigned y,unsigned r,COLOR color,unsigned Mode);
//ʵ�ֹ��ܣ�	����Ļ�л�Բ
//������		
//				x - Բ�ĵ�x����
//				y - Բ�ĵ�y����
//				r - Բ�İ뾶
//              color - Բ����ɫ
//				Mode -	0(PAINT_HOLLOW)	����Բ
//						1(PAINT_SOLID)	ʵ��Բ
//����ֵ��		��
//=============================================================
void TFT_Circle(unsigned x,unsigned y,unsigned r,COLOR color,unsigned Mode)
{
	int dx,dy,ref;
	int Temp;
	
	if(r == 0)
		return;
	dx = r;
	dy = 1;
	ref = 707 * r / 1000;					// �ο�ֵΪ0.707r

	if(Mode == PAINT_SOLID)					// ʵ��Բ
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
	else													// ����Բ
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
//�﷨��ʽ��	void TFT_Rectangle(unsigned x1, unsigned y1, unsigned x2, unsigned y2, COLOR color, unsigned Mode);
//ʵ�ֹ��ܣ�	�ڹ������о���
//������
//				x1 - ���ε����Ͻ�x����
//				y1 - ���ε����Ͻ�y����
//				x2 - ���ε����½�x����
//				y2 - ���ε����½�y����
//    			color - �����ɫ
//				Mode -	0(PAINT_HOLLOW)	���ľ���
//						1(PAINT_SOLID)	ʵ�ľ���
//����ֵ��		��
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

	if(Mode == PAINT_SOLID)									// ʵ�ľ���
	{
		for(i=y1; i<=y2; i++)
		{
			TFT_Line(x1, i, x2, i, color);
		}
	}
	
	else													// ���ľ���
	{
		TFT_Line(x1, y1, x2, y1, color);
		TFT_Line(x1, y2, x2, y2, color);
		TFT_Line(x1, y1, x1, y2, color);
		TFT_Line(x2, y1, x2, y2, color);
	}
}
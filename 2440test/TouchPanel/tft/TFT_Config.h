//====================================================================================
//文 件 名：TFT_Config.h
//功能描述: TFT驱动程序配置文件，请根据实际情况配置参数
//维护记录: 2009年8月18日
//====================================================================================
#ifndef _TFT_CONFIG_H_
#define _TFT_CONFIG_H_

#define		TFT_COLOR_RESV		0								// 是否翻转颜色
#define		TFT_MAXFONT			5								// 字库数量
#define		MAX_WINDOW 			16


#define		TFT_WIDTH			320								// 屏幕宽度
#define		TFT_HEIGHT			240								// 屏幕高度

#define		TFT_CLK_SET			C_TFT_CLK_27MDIV1				// 工作时钟

#define		TFT_INTERFACE		C_LCD_PORT_TOPPOLY				// 显示器接口类型
#define		TFT_DATA_SEQ		C_TFT_OUTPUT_YCBCR				// 显示器接口的数据顺序
#define		TFT_DATA_FMT		C_TFT_PARALLEL_RGB				// 显示器接口的数据格式


#define		TFT_HOR_ACT			TFT_WIDTH						// 水平工作时钟数
#define		TFT_HOR_FRONT		20								// 水平Front-Porch时钟数
#define		TFT_HOR_BACK		67								// 水平Back-Porch时钟数
#define		TFT_HOR_SYNC		30								// 水平同步脉冲宽度

#define		TFT_VER_ACT			TFT_HEIGHT						// 垂直工作时钟数
#define		TFT_VER_FRONT		12								// 垂直Front-Porch时钟数
#define		TFT_VER_BACK		17								// 垂直Back-Porch时钟数
#define		TFT_VER_SYNC		3								// 垂直同步脉冲宽度

////////////////////////////

#define		TFT_FRAME_FMT1		C_TFT_BUF_RGB565
#define		TFT_FRAME_FMT2		C_TFT_BUF_YUV

#define		TFT_WINDOW_WIDTH	TFT_WIDTH						// 逻辑屏幕宽度初始值
#define		TFT_WINDOW_HEIGHT	TFT_HEIGHT						// 逻辑屏幕高度初始值

#define		TFT_BUFFER0_SA		0x30400000						// 显示Buffer0起始地址
#define		TFT_BUFFER1_SA		(TFT_BUFFER0_SA + TFT_WIDTH * TFT_HEIGHT * 2)
																// 显示Buffer1起始地址
#define		TFT_BUFFER2_SA		(TFT_BUFFER0_SA + TFT_WIDTH * TFT_HEIGHT * 4)
																// 显示Buffer2起始地址
#define		TFT_DEPRESS_ADDR	0x30700000						// 用于存储GUI显示图像时的临时数据



#endif

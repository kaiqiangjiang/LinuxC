//====================================================================================
//文 件 名：TFT_Driver.c
//功能描述: TFT驱动程序(底层驱动)
//维护记录: 2009年8月18日
//====================================================================================
#include "TFT_Driver.h"
#include "2440addr.h"
#include "def.h"
#include "string.h"

volatile unsigned short LCD_BUFFER[SCR_YSIZE_TFT_320240][SCR_XSIZE_TFT_320240];

//=============================================================
//语法格式：	void TFT_InitHardware(void);
//实现功能：	初始化TFT相关硬件寄存器
//参数：		无
//返回值：		无
//=============================================================

static void Lcd_Port_Init(void)
{
	rGPCUP  = 0x00000000;
	rGPCCON = 0xaaaa02a9; 
	 
	rGPDUP  = 0x00000000;
	rGPDCON=0xaaaaaaaa; //Initialize VD[15:8]
}

void Lcd_PowerEnable(int invpwren,int pwren)
{
    //GPG4 is setted as LCD_PWREN
    rGPGUP=rGPGUP&(~(1<<4))|(1<<4); // Pull-up disable
    rGPGCON=rGPGCON&(~(3<<8))|(3<<8); //GPG4=LCD_PWREN
    //Enable LCD POWER ENABLE Function
    rLCDCON5=rLCDCON5&(~(1<<3))|(pwren<<3);   // PWREN
    rLCDCON5=rLCDCON5&(~(1<<5))|(invpwren<<5);   // INVPWREN
}  

void Lcd_EnvidOnOff(int onoff)
{
    if(onoff==1)
		rLCDCON1|=1; // ENVID=ON
    else
		rLCDCON1 =rLCDCON1 & 0x3fffe; // ENVID Off
}    


/**************************************************************
320×240 16Bpp TFT LCD功能模块初始化
**************************************************************/
void TFT_InitHardware(void)
{
    Lcd_Port_Init();

	rLCDCON1=(CLKVAL_TFT<<8)|(MVAL_USED<<7)|(3<<5)|(12<<1)|0;
    	// TFT LCD panel,12bpp TFT,ENVID=off
	rLCDCON2=(VBPD<<24)|((LCD_YSIZE_TFT_320240 - 1)<<14)|(VFPD<<6)|(VSPW);
	rLCDCON3=(HBPD<<19)|((LCD_XSIZE_TFT_320240 - 1)<<8)|(HFPD);
	rLCDCON4=(MVAL<<8)|(HSPW);
	rLCDCON5 = (1<<11) | (1<<10) | (1<<9) | (1<<8) | (0<<7) | (0<<6) | (1<<3)  |(BSWP<<1) | (HWSWP);

	rLCDSADDR1=(((U32)LCD_BUFFER>>22)<<21)|M5D((U32)LCD_BUFFER>>1);
	rLCDSADDR2=M5D( ((U32)LCD_BUFFER+(SCR_XSIZE_TFT_320240*LCD_YSIZE_TFT_320240*2))>>1 );
	rLCDSADDR3=(((SCR_XSIZE_TFT_320240-LCD_XSIZE_TFT_320240)/1)<<11)|(LCD_XSIZE_TFT_320240/1);
	rLCDINTMSK|=(3); // MASK LCD Sub Interrupt
	rLPCSEL &= (~7) ;     // Disable LPC3480
	rTPAL=0; // Disable Temp Palette
}


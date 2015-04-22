
//===================================================================
// File Name : 2440lib.h
// Function  : S3C2440 
// Program   : Shin, On Pil (SOP)
// Date      : February 26, 2002
// Version   : 1.0
// History
//   0.0 : Programming start (February 20,2002) -> SOP
//	 1.0 : Modified for SMDK2440 PPC2003 BSP ( November 29, 2003 ) -> HMSEO
//===================================================================

#ifndef __2440lib_h__
#define __2440lib_h__

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __ARMCC_VERSION     // ARM Developer Suite
//#define DebugOut Uart_Printf
#define RETAILMSG(cond, printf_exp)
#define ASSERT
#define NULL 0
#define min(x1,x2) ((x1<x2)? x1:x2)
#define max(x1,x2) ((x1>x2)? x1:x2)
#endif

#define ONESEC0 (62500)	             //16us resolution, max 1.04 sec
#define ONESEC1 (31250)	             //32us resolution, max 2.09 sec
#define ONESEC2 (15625)	             //64us resolution, max 4.19 sec
#define ONESEC3 (7812)	             //128us resolution, max 8.38 sec
#define ONESEC4 (PCLK/128/(0xff+1))  //@60Mhz, 128*4us resolution, max 32.53 sec
#define EnterPWDN(clkcon) ((void (*)(int))0x20)(clkcon)

//选择系统时钟频率，用SetSysFclk()
#define	FCLK_200M		((94<<12)|(4<<4)|1)
#define	FCLK_300M		((67<<12)|(1<<4)|1)
#define	FCLK_400M		((92<<12)|(1<<4)|1)
#define	FCLK_440M		((102<<12)|(1<<4)|1)
#define	FCLK_220M		((102<<12)|(4<<4)|1)

//使能&禁止中断
#define	EnableIrq(bit)		rINTMSK &= ~(bit)
#define	DisableIrq(bit)		rINTMSK |= (bit)
#define	EnableSubIrq(bit)	rINTSUBMSK &= ~(bit)
#define	DisableSubIrq(bit)	rINTSUBMSK |= (bit)

// 2440lib.c
void Delay(int time);

//设置时钟频率函数 
void SetSysFclk(int val);
void ChangeMPllValue(int m,int p,int s);
void CalcBusClk(void);
void ChangeClockDivider(int hdivn,int pdivn);
void ChangeUPllValue(int m,int p,int s);

#ifdef __cplusplus
}
#endif

#endif  //__2440lib_h__

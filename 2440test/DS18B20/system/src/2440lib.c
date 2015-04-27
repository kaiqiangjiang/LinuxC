//===================================================================
// File Name : 2440lib.c
// Function  : S3C2440 PLL,Uart, LED, Port Init
// Program   : Shin, On Pil (SOP)
// Date      : March 20, 2002
// Version   : 1.0
// History
//   0.0 : Programming start (February 20,2002) -> SOP
//	 1.0 : Modified for SMDK2440 PPC2003 BSP ( November 29, 2003 ) -> HMSEO
//===================================================================

#include "def.h"
#include "option.h"
#include "2440addr.h"
#include "2440lib.h"
#include "2440slib.h" 

#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

unsigned int FCLK;
unsigned int HCLK;
unsigned int PCLK;
unsigned int UCLK;
unsigned int UPLL;

void Delay(int time)
{
	U32 val = (PCLK>>3)/1000-1;
	
	rTCFG0 &= ~(0xff<<8);
	rTCFG0 |= 3<<8;			//prescaler = 3+1
	rTCFG1 &= ~(0xf<<12);
	rTCFG1 |= 0<<12;		//mux = 1/2

	rTCNTB3 = val;
	rTCMPB3 = val>>1;		// 50%
	rTCON &= ~(0xf<<16);
	rTCON |= 0xb<<16;		//interval, inv-off, update TCNTB3&TCMPB3, start timer 3
	rTCON &= ~(2<<16);		//clear manual update bit
	while(time--) {
		while(rTCNTO3>=val>>1);
		while(rTCNTO3<val>>1);
	};
}

void SetSysFclk(int val)
{
	rMPLLCON = val;
}


void ChangeClockDivider(int hdivn,int pdivn)
{
     // hdivn,pdivn FCLK:HCLK:PCLK
     //     0,0         1:1:1 
     //     0,1         1:1:2 
     //     1,0         1:2:2
     //     1,1         1:2:4
     //     2,0         1:4:4
     //     2,1         1:4:8
     //     3,0         1:3:3
     //     3,1         1:3:6
    rCLKDIVN = (hdivn<<1) | pdivn;    

	if (hdivn == 2)
		rCAMDIVN = (rCAMDIVN & ~(3<<8)) | (1<<9);			
	if (hdivn == 3)
		rCAMDIVN = (rCAMDIVN & ~(3<<8)) | (1<<8);
}

void CalcBusClk(void)
{
	U32 val;
	U8 m, p, s;
	val = rMPLLCON;
	m = (val >> 12) & 0xff;
	p = (val >> 4) & 0x3f;
	s = val & 3;

	FCLK = ((m+8)*(FIN/100)*2)/((p+2)*(1<<s))*100;
	
	val = rCLKDIVN;
	m = (val >> 1) & 3;
	p = val & 1;	
	val = rCAMDIVN;
	s = val >> 8;
	
	switch (m) 
	{
		case 0:
			HCLK = FCLK;
			break;
		case 1:
			HCLK = FCLK >> 1;
			break;
		case 2:
			if(s & 2)
				HCLK = FCLK >> 3;
			else
				HCLK = FCLK >> 2;
			break;
		case 3:
			if(s & 1)
				HCLK = FCLK / 6;
			else
				HCLK = FCLK / 3;
			break;
	}
	
	if(p)
		PCLK = HCLK >> 1;
	else
		PCLK = HCLK;
		
	val = rUPLLCON;
	m = (val >> 12) & 0xff;
	p = (val >> 4) & 0x3f;
	s = val & 3;
	UPLL = ((m+8)*FIN)/((p+2)*(1<<s));
	UCLK = (rCLKDIVN&8)?(UPLL>>1):UPLL;
}

void ChangeMPllValue(int mdiv,int pdiv,int sdiv)
{
    rMPLLCON = (mdiv<<12) | (pdiv<<4) | sdiv;
}

void ChangeUPllValue(int mdiv,int pdiv,int sdiv)
{
    rUPLLCON = (mdiv<<12) | (pdiv<<4) | sdiv;
}

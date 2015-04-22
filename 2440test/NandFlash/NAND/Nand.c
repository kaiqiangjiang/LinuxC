//=============================================================
// 文件名称：	Nand.c
// 功能描述：	NandFlash驱动
//              选择Nand类型：1.K9F1208(small page) 2.K9F2G08(large page)
//              选择要执行的操作：
//              0: 读取Chip ID
//              1: 复位Nand
//              2: 块擦除，输入要擦除的块
//              3: 读Nand，输入要读取的块，页
//              4: 写Nand，输入要写入的块，页，偏移量
//              5: 检查坏块
//
// 维护记录：	2009-08-15	V1.0 by xgc
//              2009-10-14  V1.1 by xgc 支持大页Nand，还需完善ECC校验
//=============================================================
#include "def.h"
#include "2440lib.h"
#include "2440addr.h"
#include "Nand.h"
#include "uart.h"

//====================================================
// 常量定义区
//====================================================

#define	EnNandFlash()	(rNFCONF |= (1<<0))  //bit0=1 enable NAND flash controller
#define	DsNandFlash()	(rNFCONF &= ~(1<<)) //bit0=1 disable NAND flash controller
#define	InitEcc()		(rNFCONF |= (1<<4))  //bit4=1 initialize ECC
#define	NoEcc()			(rNFCONT &= ~(1<<4)) //bit4=0 initialize ECC
#define	NF_ChipEn()		(rNFCONT&=~(1<<1))  //bit1=0 NAND flash nFCE = L (active)
#define	NF_ChipDs()		(rNFCONT|=(1<<1))   //bit1=1 NAND flash nFCE = H (inactive)
#define NF_MECC_UnLock()         {rNFCONT&=~(1<<5);}
#define NF_MECC_Lock()         {rNFCONT|=(1<<5);}
#define NF_SECC_UnLock()         {rNFCONT&=~(1<<6);}
#define NF_SECC_Lock()         {rNFCONT|=(1<<6);}

#define NF_CMD(cmd)			{rNFCMD=cmd;}
#define NF_ADDR(addr)		{rNFADDR=addr;}	
#define NF_RSTECC()			{rNFCONT|=(1<<4);}
#define NF_RDDATA() 		(rNFDATA)
#define NF_RDDATA8() 		((*(volatile unsigned char*)0x4E000010) )
#define NF_WRDATA(data) 	{rNFDATA=data;}
#define NF_WRDATA8(data) 	{rNFDATA8=data;}

#define	NFIsBusy()		(!(rNFSTAT&1))     //whether nand flash is busy?
#define	NFIsReady()		(rNFSTAT&1)        //whether nand flash is ready?

#define	READCMD0	0      //Read0 model  command  == Page addr  0~127
#define	READCMD1	1      //Read1 model  command  == Page addr  128~511
#define	READCMD2	0x50   //Read2 model  command  == Page addr  512~527
#define READCMD3    0x30   //Read3 model  for Large Page 
#define	ERASECMD0	0x60   //Block erase  command 0
#define	ERASECMD1	0xd0   //Block erase  command 1
#define	PROGCMD0	0x80   //page write   command 0
#define	PROGCMD1	0x10   //page write   command 1
#define	QUERYCMD	0x70   //query command
#define	RdIDCMD		0x90   //read id command
#define RESETCMD    0xFF   //reset command

// RnB Signal
#define NF_CLEAR_RB()    		{rNFSTAT |= (1<<2);}	// Have write '1' to clear this bit.
#define NF_DETECT_RB()    		{while(!(rNFSTAT&(1<<2)));}

#define TACLS		7	// 1-clk(0ns) 
#define TWRPH0		7	// 3-clk(25ns)
#define TWRPH1		7	// 1-clk(10ns)  //TACLS+TWRPH0+TWRPH1>=50ns

#define   OK        1
#define   FAIL      0

#define ASM		1
#define C_LANG	2
#define DMA		3
#define TRANS_MODE 2

#define K9F1208_SUPPORT	1
#define K9F2G08_SUPPORT	2

volatile int NFConDone;
U8 Spare_Data_1208[16];
U8 Spare_Data_2G08[64];
U8 Nand_Selet;

static U8 se8Buf[16]={
	0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff
};

void __irq NFCon_Int(void);

void * nand_func[][2]=
{
	(void *)Print_Id,			    "Read ID         ",
	(void *)Nand_Reset,				"Nand reset      ",
	(void *)Test_Block_Erase,	    "Block erase     ",
	(void *)Test_Page_Read,		    "Page read       ",
	(void *)Test_Page_Write,	    "Page write      ",
	(void *)PrintBadBlockNum,	    "Check Badblock  ",	
	0,
};

void Test_Nand(void)
{
	U8 ch;
	U32 gpacon;
	
	Uart_Printf("Nand test\n");

	gpacon = rGPACON;
	rGPACON = (rGPACON &~(0x3f<<17)) | (0x3f<<17);
	// GPA     22         21           20      19     18     17
	//           nFCE   nRSTOUT  nFRE  nFWE   ALE   CLE

	Uart_Printf("Select Nand flash type, K9F1208(1)/K9F2G08(2) : ");
	ch=Uart_Getch();
	Uart_Printf("%c\n\n", ch);
	
	switch(ch) {
		case '1':
			Nand_Selet = 1;
			Test_K9F1208();			
		break;
		case '2':
			Nand_Selet = 2;
			Test_K9F2G08();
		break;
		default:
		break;
	}

	rGPACON = gpacon;
}

void Test_K9F1208(void)
{
//	int i;
	U8 i;

	Uart_Printf("\nK9F1208(Small Page) Nand flash test start.\n");

	InitNandFlash();

	while(1)
	{
		PrintSubMessage();
		Uart_Printf("\nSelect: ");
		i = Uart_GetIntNum_GJ();
		
//		Uart_Printf("i=%d\n",i);
		
		if(i>=0 && (i<(sizeof(nand_func)/8)) ) 
	    	( (void (*)(void)) (nand_func[i][0]) )();	// execute selected function.
		else
			Uart_Printf("\nWrong Num!\n");
	}
}

void Test_K9F2G08(void)
{
//	int i;
	U8 i;

	Uart_Printf("\nK9F2G08(Large Page) Nand flash test start.\n");

	InitNandFlash();

	while(1)
	{
		PrintSubMessage();
		Uart_Printf("\nSelect: ");
		i = Uart_GetIntNum_GJ();
		
//		Uart_Printf("i=%d\n",i);
		
		if(i>=0 && (i<(sizeof(nand_func)/8)) ) 
	    	( (void (*)(void)) (nand_func[i][0]) )();	// execute selected function.
		else
			Uart_Printf("\nWrong Num!\n");
	}
}

void PrintSubMessage(void)
{
	int i;
	
	i=0;	
	Uart_Printf("\n");
	while(1)
	{   //display menu
	    Uart_Printf("%2d:%s\n",i,nand_func[i][1]);
	    i++;
	    if((int)(nand_func[i][0])==0)
	    {
		 Uart_Printf("\n");
		 break;
	    }
	}
}

static void InitNandCfg(void)
{
	// for S3C2440

	rNFCONF = (TACLS<<12)|(TWRPH0<<8)|(TWRPH1<<4)|(0<<0);	
	// TACLS		[14:12]	CLE&ALE duration = HCLK*TACLS.
	// TWRPH0		[10:8]	TWRPH0 duration = HCLK*(TWRPH0+1)
	// TWRPH1		[6:4]	TWRPH1 duration = HCLK*(TWRPH1+1)
	// AdvFlash(R)	[3]		Advanced NAND, 0:256/512, 1:1024/2048
	// PageSize(R)	[2]		NAND memory page size
	//						when [3]==0, 0:256, 1:512 bytes/page.
	//						when [3]==1, 0:1024, 1:2048 bytes/page.
	// AddrCycle(R)	[1]		NAND flash addr size
	//						when [3]==0, 0:3-addr, 1:4-addr.
	//						when [3]==1, 0:4-addr, 1:5-addr.
	// BusWidth(R/W) [0]	NAND bus width. 0:8-bit, 1:16-bit.
	
	rNFCONT = (0<<13)|(0<<12)|(0<<10)|(0<<9)|(0<<8)|(1<<6)|(1<<5)|(1<<4)|(1<<1)|(1<<0);
	// Lock-tight	[13]	0:Disable lock, 1:Enable lock.
	// Soft Lock	[12]	0:Disable lock, 1:Enable lock.
	// EnablillegalAcINT[10]	Illegal access interupt control. 0:Disable, 1:Enable
	// EnbRnBINT	[9]		RnB interrupt. 0:Disable, 1:Enable
	// RnB_TrandMode[8]		RnB transition detection config. 0:Low to High, 1:High to Low
	// SpareECCLock	[6]		0:Unlock, 1:Lock
	// MainECCLock	[5]		0:Unlock, 1:Lock
	// InitECC(W)	[4]		1:Init ECC decoder/encoder.
	// Reg_nCE		[1]		0:nFCE=0, 1:nFCE=1.
	// NANDC Enable	[0]		operating mode. 0:Disable, 1:Enable.

//	rNFSTAT = 0;
    
//    Nand_Reset();	
}

U16 Read_Status(void)
{
	U16 stat;
	int i;
	
	NF_ChipEn();	
	NF_CMD(QUERYCMD);
	for(i=0; i<10; i++);	
	stat = NF_RDDATA();	
	NF_ChipDs();
	
	return stat;
}

U32 ReadChipId(void)
{
	U32 id;
	
	NF_ChipEn();	
	NF_CMD(RdIDCMD);
	NF_ADDR(0);
	while(NFIsBusy());	
	id  = NF_RDDATA()<<8;
	id |= NF_RDDATA();		
	NF_ChipDs();		
	
	return id;
}

void Print_Id(void)
{
	U16 id;
	U8 maker, device;

	id = ReadChipId();
	
	device = (U8)id;
	maker = (U8)(id>>8);
	
	Uart_Printf("\nMaker:%x, Device:%x\n", maker, device);
}

void Nand_Reset(void)
{
    int i;
   
	NF_ChipEn();

	NF_CLEAR_RB();
	NF_CMD(RESETCMD);	//reset command
	for(i=0;i<10;i++);  //tWB = 100ns. //??????
	NF_DETECT_RB();
	
	NF_ChipDs();
		
	Uart_Printf("\nNand Reset!\n");
}

void Test_Block_Erase(void)
{
	U32 block=0;
	if (Nand_Selet == K9F1208_SUPPORT)
	{
		Uart_Printf("\nK9F1208 NAND Block erase\n");

		if((Read_Status()&0x80)==0) {
			Uart_Printf("Write protected.\n");
			return;
		}

		Uart_Printf("Block # to erase: ");
		block = Uart_GetIntNum();
		
		if(EraseBlock_1208(block)==FAIL) return;

		Uart_Printf("\n%d-block erased.\n", block);
	}
	else if (Nand_Selet == K9F2G08_SUPPORT)
	{
		Uart_Printf("\nK9F2G08 NAND Block erase\n");

		if((Read_Status()&0x80)==0) {
			Uart_Printf("Write protected.\n");
			return;
		}

		Uart_Printf("Block # to erase: ");
		block = Uart_GetIntNum();
		
		if(EraseBlock_2G08(block)==FAIL) return;

		Uart_Printf("\n%d-block erased.\n", block);
	}
}

void Test_Page_Read(void)
{
	U32 block=0, page=0;
	U32 i;
	unsigned char * downPt;
	
	downPt=(unsigned char *)_NONCACHE_STARTADDRESS;
	
	if (Nand_Selet == K9F1208_SUPPORT)
	{
		Uart_Printf("\nK9F1208 NAND Page Read.\n");

		Uart_Printf("\nBlock # to read: ");
		block = Uart_GetIntNum();
		Uart_Printf("\nPage # to read: ");
		page = Uart_GetIntNum();

		if(ReadPage_1208(block, page, (U8 *)downPt )==FAIL) {
			Uart_Printf("\nRead error.\n");
		} else {
			Uart_Printf("\nRead OK.\n");
		}
		// Print data.
		Uart_Printf("Read data(%d-block,%d-page)\n", block, page);
		
		for(i=0; i<512; i++) {
			if((i%16)==0) Uart_Printf("\n%4x: ", i);
			Uart_Printf("%02x ", *(U8 *)downPt++);
			}
		Uart_Printf("\n");
		Uart_Printf("Spare:");
		for(i=0; i<16; i++) {
			Uart_Printf("%02x ", Spare_Data_1208[i]);
		}
		Uart_Printf("\n");
	}
	else if (Nand_Selet == K9F2G08_SUPPORT)
	{
		Uart_Printf("\nK9F2G08 NAND Page Read.\n");

		Uart_Printf("\nBlock # to read: ");
		block = Uart_GetIntNum();
		Uart_Printf("\nPage # to read: ");
		page = Uart_GetIntNum();

		if(ReadPage_2G08(block, page, (U8 *)downPt )==FAIL) {
			Uart_Printf("\nRead error.\n");
		} else {
			Uart_Printf("\nRead OK.\n");
		}
		// Print data.
		Uart_Printf("Read data(%d-block,%d-page)\n", block, page);
		
		for(i=0; i<2048; i++) {
			if((i%16)==0) Uart_Printf("\n%4x: ", i);
			Uart_Printf("%02x ", *(U8 *)downPt++);
			}
		Uart_Printf("\n");
//		Uart_Printf("Spare:");
//		for(i=0; i<16; i++) {
//			Uart_Printf("%02x ", Spare_Data_1208[i]);
//		}
//		Uart_Printf("\n");	
	}
}

void Test_Page_Write(void)
{
	U32 block=0, page=0;
	int i, offset;
	unsigned char * srcPt;
	srcPt=(unsigned char *)0x31100000;

	if (Nand_Selet == K9F1208_SUPPORT)
	{	
		Uart_Printf("\nK9F1208 NAND Page Write.\n");
		Uart_Printf("You must erase block before you write data!!! \n");

		Uart_Printf("\nBlock # to write: ");
		block = Uart_GetIntNum();
		Uart_Printf("\nPage # to write: ");
		page = Uart_GetIntNum();
		Uart_Printf("\noffset data(-1:random): ");
		offset = Uart_GetIntNum();

		// Init wdata.
		for(i=0; i<512; i++)
		{
			if(offset==-1) *srcPt++ = i%0xff;
			else *srcPt++ =  i+offset;
		}
		srcPt=(unsigned char *)0x31100000;
		Uart_Printf("\nWrite data[%d block, %d page].\n", block, page);
		
		if(WritePage_1208(block, page, srcPt)==FAIL)
		{
			Uart_Printf("Write Error.\n");
		} 
		else
		{
			Uart_Printf("Write OK.\n");
		}

		Uart_Printf("Write data is");
		for(i=0; i<512; i++)
		{
			if((i%16)==0) Uart_Printf("\n%4x: ", i);
			Uart_Printf("%02x ", *srcPt++);
		}
		Uart_Printf("\n");

		Uart_Printf("Spare:");
		for(i=0; i<16; i++)
		{
			Uart_Printf("%02x ", Spare_Data_1208[i]);
		}
		Uart_Printf("\n\n");
	}
	else if (Nand_Selet == K9F2G08_SUPPORT)
	{
		Uart_Printf("\nK9F2G08 NAND Page Write.\n");
		Uart_Printf("You must erase block before you write data!!! \n");

		Uart_Printf("\nBlock # to write: ");
		block = Uart_GetIntNum();
		Uart_Printf("\nPage # to write: ");
		page = Uart_GetIntNum();
		Uart_Printf("\noffset data(-1:random): ");
		offset = Uart_GetIntNum();

		// Init wdata.
		for(i=0; i<2048; i++)
		{
			if(offset==-1) *srcPt++ = i%0xff;
			else *srcPt++ =  i+offset;
		}
		srcPt=(unsigned char *)0x31100000;
		Uart_Printf("\nWrite data[%d block, %d page].\n", block, page);
		
		if(WritePage_2G08(block, page, srcPt)==FAIL)
		{
			Uart_Printf("Write Error.\n");
		} 
		else
		{
			Uart_Printf("Write OK.\n");
		}

		Uart_Printf("Write data is");
		for(i=0; i<2048; i++)
		{
			if((i%16)==0) Uart_Printf("\n%4x: ", i);
			Uart_Printf("%02x ", *srcPt++);
		}
		Uart_Printf("\n");

//		Uart_Printf("Spare:");
//		for(i=0; i<16; i++)
//		{
//			Uart_Printf("%02x ", Spare_Data_1208[i]);
//		}
//		Uart_Printf("\n\n");	
	}
}

void PrintBadBlockNum(void)
{
    int i;
    U16 id;
    int badblock = 0;

	if (Nand_Selet == K9F1208_SUPPORT)
	{
		Uart_Printf("\n[K9F1208 NAND Flash bad block check]\n");
		
		id=ReadChipId();
		Uart_Printf("ID=%x(0xec76)\n",id);
	//    if(id!=0xec76)
	//    return;
		for(i=0;i<4096;i++)
		{
			if (IsBadBlock_1208(i) == FAIL)   // Print bad block
				badblock ++;
		}
		if (badblock == 0)
			Uart_Printf("no bad block\n");
	}
	else if (Nand_Selet == K9F2G08_SUPPORT)
	{
		Uart_Printf("\n[K9F2G08 NAND Flash bad block check]\n");
		
		id=ReadChipId();
		Uart_Printf("ID=%x(0xec76)\n",id);
	//    if(id!=0xec76)
	//    return;
		for(i=0;i<2048;i++)
		{
			if (IsBadBlock_2G08(i) == FAIL)   // Print bad block
				badblock ++;
		}
		if (badblock == 0)
			Uart_Printf("no bad block\n");	
	}
}

static int EraseBlock_1208(U32 block)
{
	U32 blockPage=(block<<5);
    int i;

    NFConDone=0;
    rNFCONT|=(1<<9);
    rNFCONT|=(1<<10);
    pISR_NFCON= (unsigned)NFCon_Int;
    rSRCPND=BIT_NFCON;
    rINTMSK=~(BIT_NFCON);
	
#if BAD_CHECK
    if(IsBadBlock_1208(block))
	return FAIL;
#endif

	NF_ChipEn();
    
	NF_CMD(ERASECMD0);   // Erase one block 1st command, Block Addr:A9-A25
	// Address 3-cycle
	NF_ADDR(blockPage&0xff);	    // Page number=0
	NF_ADDR((blockPage>>8)&0xff);   
	NF_ADDR((blockPage>>16)&0xff);


	NF_CLEAR_RB();
	NF_CMD(ERASECMD1);	// Erase one blcok 2nd command
//	NF_DETECT_RB();
    while(NFConDone==0);
	rNFCONT&=~(1<<9);
	rNFCONT&=~(1<<10); // Disable Illegal Access Interrupt
	if(rNFSTAT&0x8) 
		return FAIL;

	NF_CMD(QUERYCMD);   // Read status command

    if (NF_RDDATA()&0x1) // Erase error
    {	
    	NF_ChipDs();
		Uart_Printf("[ERASE_ERROR:block#=%d]\n",block);
//	MarkBadBlock_1208(block);
		return FAIL;
    }
    else 
    {
    	NF_ChipDs();
		return OK;
    }
}

static int ReadPage_1208(U32 block,U32 page,U8 *buffer)
{
    int i;
    unsigned int blockPage;
	U32 Mecc, Secc;
	U8 *bufPt=buffer;
	U8 se[16], ecc0, ecc1, ecc2, ecc3,a,b,c,d,e;
    
    blockPage=(block<<5)+page;
	NF_RSTECC();    // Initialize ECC
	NF_MECC_UnLock();
    
	NF_ChipEn();    

	NF_CLEAR_RB();
	NF_CMD(READCMD0);	// Read command
	NF_ADDR(0); 	// Column = 0
	NF_ADDR(blockPage&0xff);		//
	NF_ADDR((blockPage>>8)&0xff);	// Block & Page num.
	NF_ADDR((blockPage>>16)&0xff);	//
	NF_DETECT_RB();
	 
	
#if TRANS_MODE==C_LANG
	for(i=0;i<512;i++)
	{
		*bufPt++=NF_RDDATA8();	// Read one page
	}
#elif TRANS_MODE==DMA
	// Nand to memory dma setting
	rSRCPND=BIT_DMA0;	// Init DMA src pending.
	rDISRC0=NFDATA; 	// Nand flash data register
	rDISRCC0=(0<<1) | (1<<0); //arc=AHB,src_addr=fix
	rDIDST0=(unsigned)bufPt;
	rDIDSTC0=(0<<1) | (0<<0); //dst=AHB,dst_addr=inc;
	rDCON0=(1<<31)|(1<<30)|(1<<29)|(1<<28)|(1<<27)|(0<<23)|(1<<22)|(2<<20)|(512/4/4);
	//Handshake,AHB,interrupt,(4-burst),whole,S/W,no_autoreload,word,count=128;

	// DMA on and start.
	rDMASKTRIG0=(1<<1)|(1<<0);

	while(!(rSRCPND & BIT_DMA0));	// Wait until Dma transfer is done.
		
	rSRCPND=BIT_DMA0;

#elif TRANS_MODE==ASM
	__RdPage512(bufPt);
#endif

	NF_MECC_Lock();

	NF_SECC_UnLock();
	Mecc=NF_RDDATA();
	rNFMECCD0=((Mecc&0xff00)<<8)|(Mecc&0xff);
	rNFMECCD1=((Mecc&0xff000000)>>8)|((Mecc&0xff0000)>>16);
	
	NF_SECC_Lock();
	Spare_Data_1208[0]=Mecc&0xff;
	Spare_Data_1208[1]=(Mecc&0xff00)>>8;
	Spare_Data_1208[2]=(Mecc&0xff0000)>>16;
	Spare_Data_1208[3]=(Mecc&0xff000000)>>24;
	NF_RDDATA();  // read 4~7
	Secc=NF_RDDATA();
	rNFSECCD=((Secc&0xff00)<<8)|(Secc&0xff);
	Spare_Data_1208[8]=Secc&0xff;
	Spare_Data_1208[9]=(Secc&0xff00)>>8;
	Spare_Data_1208[10]=(Secc&0xff0000)>>16;
	Spare_Data_1208[11]=(Secc&0xff000000)>>24;
	NF_ChipDs();    

	if ((rNFESTAT0&0xf) == 0x0)
	{
		Uart_Printf("ECC OK!\n");
		return OK;
	}
	else
	{
		Uart_Printf("ECC FAIL!\n");
	    return FAIL;
	}
}

static int WritePage_1208(U32 block,U32 page,U8 *buffer)
{
    int i;
	U32 blockPage, Mecc, Secc;
	U8 *bufPt=buffer;

	NFConDone=0;
    rNFCONT|=(1<<9);
    rNFCONT|=(1<<10);
    pISR_NFCON= (unsigned)NFCon_Int;
    rSRCPND=BIT_NFCON;
    rINTMSK=~(BIT_NFCON);
	  
	NF_RSTECC();    // Initialize ECC
    NF_MECC_UnLock();
	blockPage=(block<<5)+page;

	NF_ChipEn(); 
//	NF_CMD(0x0);//??????
	NF_CMD(PROGCMD0);   // Write 1st command
	NF_ADDR(0);			    // Column 0
	NF_ADDR(blockPage&0xff);	    //
	NF_ADDR((blockPage>>8)&0xff);   // Block & page num.
	NF_ADDR((blockPage>>16)&0xff);  //

	
#if TRANS_MODE==C_LANG
     
	for(i=0;i<512;i++)
	{
		NF_WRDATA8(*bufPt++);	// Write one page to NFM from buffer
    }
#elif TRANS_MODE==DMA
      
	// Memory to Nand dma setting
	rSRCPND=BIT_DMA0;	// Init DMA src pending.
	rDISRC0=(unsigned)bufPt; 	// Nand flash data register
	rDISRCC0=(0<<1) | (0<<0); //arc=AHB,src_addr=inc
	rDIDST0=NFDATA;
	rDIDSTC0=(0<<1) | (1<<0); //dst=AHB,dst_addr=fix;
	rDCON0=(1<<31)|(1<<30)|(1<<29)|(0<<28)|(1<<27)|(0<<23)|(1<<22)|(2<<20)|(512/4);
	//  only unit transfer in writing!!!!
	//Handshake,AHB,interrupt,(unit),whole,S/W,no_autoreload,word,count=128;
	
	// DMA on and start.
	rDMASKTRIG0=(1<<1)|(1<<0);
	
	while(!(rSRCPND & BIT_DMA0));	// Wait until Dma transfer is done.
	rSRCPND=BIT_DMA0;	
#endif

    NF_MECC_Lock();
	// Get ECC data.
	// Spare data for 8bit
	// byte  0     1    2     3     4          5               6      7            8         9
	// ecc  [0]  [1]  [2]  [3]    x   [Bad marking]                    SECC0  SECC1
	Mecc = rNFMECC0;
	se8Buf[0]=(U8)(Mecc&0xff);
	se8Buf[1]=(U8)((Mecc>>8) & 0xff);
	se8Buf[2]=(U8)((Mecc>>16) & 0xff);
	se8Buf[3]=(U8)((Mecc>>24) & 0xff);
	se8Buf[5]=0xff;		// Marking good block

	NF_SECC_UnLock();
	//Write extra data(ECC, bad marking)
	for(i=0;i<4;i++)
	{
		NF_WRDATA8(se8Buf[i]);	// Write spare array(Main ECC)
		Spare_Data_1208[i]=se8Buf[i];
    }  
    NF_SECC_Lock(); 
	Secc=rNFSECC; 
	se8Buf[8]=(U8)(Secc&0xff);
	se8Buf[9]=(U8)((Secc>>8) & 0xff);
	for(i=4;i<16;i++)
	{
		NF_WRDATA8(se8Buf[i]);  // Write spare array(Spare ECC and Mark)
		Spare_Data_1208[i]=se8Buf[i];
	}  
 	NF_CLEAR_RB();
	NF_CMD(PROGCMD1);	 // Write 2nd command
//	NF_DETECT_RB();
	while(NFConDone==0);
	rNFCONT&=~(1<<9);
	rNFCONT&=~(1<<10); // Disable Illegal Access Interrupt
	if(rNFSTAT&0x8) return FAIL;

	NF_CMD(QUERYCMD);   // Read status command   
    
	for(i=0;i<3;i++);  //twhr=60ns
    
    if (NF_RDDATA()&0x1)  // Page write error
	{
    	NF_ChipDs();
		Uart_Printf("[PROGRAM_ERROR:block#=%d]\n",block);
		MarkBadBlock_1208(block);
		return FAIL;
    } 
	else
	{
    	NF_ChipDs();
	    return OK;
	}
}

static int IsBadBlock_1208(U32 block)
{
    int i;
    unsigned int blockPage;
	U8 data;
    
    blockPage=(block<<5);	// For 2'nd cycle I/O[7:5] 
    
	NF_ChipEn();
	NF_CLEAR_RB();

	NF_CMD(READCMD2);		 // Spare array read command
	NF_ADDR((512+5)&0xf);		 // Read the mark of bad block in spare array(M addr=5), A4-A7:Don't care
	NF_ADDR(blockPage&0xff);	 // The mark of bad block is in 0 page
	NF_ADDR((blockPage>>8)&0xff);	 // For block number A[24:17]
	NF_ADDR((blockPage>>16)&0xff);  // For block number A[25]
	 
	NF_DETECT_RB();	 // Wait tR(max 12us)

    data=NF_RDDATA();

	NF_ChipDs();    

    if(data!=0xff)
    {
    	Uart_Printf("[block %d has been marked as a bad block(%x)]\n",block,data);
    	return FAIL;
    }
    else
    {
    	return OK;
    }

}

static int MarkBadBlock_1208(U32 block)
{
    int i;
	U32 blockPage=(block<<5);
 
    se8Buf[0]=0xff;
    se8Buf[1]=0xff;    
    se8Buf[2]=0xff;    
    se8Buf[5]=0x44;   // Bad blcok mark=44
    
	NF_ChipEn(); 
	NF_CMD(READCMD2);   //????
	NF_CMD(PROGCMD0);   // Write 1st command
    
	NF_ADDR(0x0);		    // The mark of bad block is 
	NF_ADDR(blockPage&0xff);	    // marked 5th spare array 
	NF_ADDR((blockPage>>8)&0xff);   // in the 1st page.
	NF_ADDR((blockPage>>16)&0xff);  //
    
	for(i=0;i<16;i++)
    {
		NF_WRDATA(se8Buf[i]);	// Write spare array
    }

	NF_CLEAR_RB();
	NF_CMD(PROGCMD1);   // Write 2nd command
	NF_DETECT_RB();

	NF_CMD(QUERYCMD);
    
	for(i=0;i<3;i++);  //twhr=60ns////??????
    
    if (NF_RDDATA()&0x1) // Spare arrray write error
    {	
    	NF_ChipDs();
    	Uart_Printf("[Program error is occurred but ignored]\n");
    }
    else 
    {
    	NF_ChipDs();
    }

	Uart_Printf("[block #%d is marked as a bad block]\n",block);
    return OK;
}

static int EraseBlock_2G08(U32 block)
{
	U32 blockPage=(block<<5);
    int i;

    NFConDone=0;
    rNFCONT|=(1<<9);
    rNFCONT|=(1<<10);
    pISR_NFCON= (unsigned)NFCon_Int;
    rSRCPND=BIT_NFCON;
    rINTMSK=~(BIT_NFCON);
	
#if BAD_CHECK
    if(IsBadBlock_2G08(block))
	return FAIL;
#endif

	NF_ChipEn();
    
	NF_CMD(ERASECMD0);   // Erase one block 1st command, Block Addr:A9-A25
	// Address 3-cycle
	NF_ADDR(blockPage&0xff);	    // Page number=0
	NF_ADDR((blockPage>>8)&0xff);   
	NF_ADDR((blockPage>>16)&0xff);


	NF_CLEAR_RB();
	NF_CMD(ERASECMD1);	// Erase one blcok 2nd command
//	NF_DETECT_RB();
    while(NFConDone==0);
	rNFCONT&=~(1<<9);
	rNFCONT&=~(1<<10); // Disable Illegal Access Interrupt
	if(rNFSTAT&0x8) 
		return FAIL;

	NF_CMD(QUERYCMD);   // Read status command

    if (NF_RDDATA()&0x1) // Erase error
    {	
    	NF_ChipDs();
		Uart_Printf("[ERASE_ERROR:block#=%d]\n",block);
//	MarkBadBlock_1208(block);
		return FAIL;
    }
    else 
    {
    	NF_ChipDs();
		return OK;
    }
}

static int ReadPage_2G08(U32 block,U32 page,U8 *buffer)
{
    int i;
    unsigned int blockPage;
	U32 Mecc, Secc;
	U8 *bufPt=buffer;
	U8 se[64], ecc0, ecc1, ecc2, ecc3,a,b,c,d,e;
    
    blockPage=(block<<5)+page;
	NF_RSTECC();    // Initialize ECC
	NF_MECC_UnLock();
    
	NF_ChipEn();    

	NF_CLEAR_RB();
	NF_CMD(READCMD0);	// Read command
	NF_ADDR(0); 	// Column = 0
	NF_ADDR(0); 
	NF_ADDR(blockPage&0xff);		//
	NF_ADDR((blockPage>>8)&0xff);	// Block & Page num.
	NF_ADDR((blockPage>>16)&0xff);	//
	
	NF_CMD(READCMD3);
	NF_DETECT_RB();
	 
	
#if TRANS_MODE==C_LANG
	for(i=0;i<2048;i++)
	{
		*bufPt++=NF_RDDATA8();	// Read one page
	}
#elif TRANS_MODE==DMA
	// Nand to memory dma setting
	rSRCPND=BIT_DMA0;	// Init DMA src pending.
	rDISRC0=NFDATA; 	// Nand flash data register
	rDISRCC0=(0<<1) | (1<<0); //arc=AHB,src_addr=fix
	rDIDST0=(unsigned)bufPt;
	rDIDSTC0=(0<<1) | (0<<0); //dst=AHB,dst_addr=inc;
	rDCON0=(1<<31)|(1<<30)|(1<<29)|(1<<28)|(1<<27)|(0<<23)|(1<<22)|(2<<20)|(512/4/4);
	//Handshake,AHB,interrupt,(4-burst),whole,S/W,no_autoreload,word,count=128;

	// DMA on and start.
	rDMASKTRIG0=(1<<1)|(1<<0);

	while(!(rSRCPND & BIT_DMA0));	// Wait until Dma transfer is done.
		
	rSRCPND=BIT_DMA0;

#elif TRANS_MODE==ASM
	__RdPage512(bufPt);
#endif
/*
	NF_MECC_Lock();

	NF_SECC_UnLock();
	Mecc=NF_RDDATA();
	rNFMECCD0=((Mecc&0xff00)<<8)|(Mecc&0xff);
	rNFMECCD1=((Mecc&0xff000000)>>8)|((Mecc&0xff0000)>>16);
	
	NF_SECC_Lock();
	Spare_Data_2G08[0]=Mecc&0xff;
	Spare_Data_2G08[1]=(Mecc&0xff00)>>8;
	Spare_Data_2G08[2]=(Mecc&0xff0000)>>16;
	Spare_Data_2G08[3]=(Mecc&0xff000000)>>24;
	NF_RDDATA();  // read 4~7
	Secc=NF_RDDATA();
	rNFSECCD=((Secc&0xff00)<<8)|(Secc&0xff);
	Spare_Data_2G08[8]=Secc&0xff;
	Spare_Data_2G08[9]=(Secc&0xff00)>>8;
	Spare_Data_2G08[10]=(Secc&0xff0000)>>16;
	Spare_Data_2G08[11]=(Secc&0xff000000)>>24;
	NF_ChipDs();    

	if ((rNFESTAT0&0xf) == 0x0)
	{
		Uart_Printf("ECC OK!\n");
		return OK;
	}
	else
	{
		Uart_Printf("ECC FAIL!\n");
	    return FAIL;
	}
	*/
}

static int WritePage_2G08(U32 block,U32 page,U8 *buffer)
{
    int i;
	U32 blockPage, Mecc, Secc;
	U8 *bufPt=buffer;

	NFConDone=0;
    rNFCONT|=(1<<9);
    rNFCONT|=(1<<10);
    pISR_NFCON= (unsigned)NFCon_Int;
    rSRCPND=BIT_NFCON;
    rINTMSK=~(BIT_NFCON);
	  
	NF_RSTECC();    // Initialize ECC
    NF_MECC_UnLock();
	blockPage=(block<<5)+page;

	NF_ChipEn(); 
//	NF_CMD(0x0);//??????
	NF_CMD(PROGCMD0);   // Write 1st command
	NF_ADDR(0);    //Column (A[7:0]) = 0
	NF_ADDR(0);    // A[11:8]
	NF_ADDR((blockPage)&0xff);	// A[19:12]
	NF_ADDR((blockPage>>8)&0xff);	// A[27:20]
	NF_ADDR((blockPage>>16)&0xff);  //

	
#if TRANS_MODE==C_LANG
     
	for(i=0;i<2048;i++)
	{
		NF_WRDATA8(*bufPt++);	// Write one page to NFM from buffer
    }
#elif TRANS_MODE==DMA
      
	// Memory to Nand dma setting
	rSRCPND=BIT_DMA0;	// Init DMA src pending.
	rDISRC0=(unsigned)bufPt; 	// Nand flash data register
	rDISRCC0=(0<<1) | (0<<0); //arc=AHB,src_addr=inc
	rDIDST0=NFDATA;
	rDIDSTC0=(0<<1) | (1<<0); //dst=AHB,dst_addr=fix;
	rDCON0=(1<<31)|(1<<30)|(1<<29)|(0<<28)|(1<<27)|(0<<23)|(1<<22)|(2<<20)|(512/4);
	//  only unit transfer in writing!!!!
	//Handshake,AHB,interrupt,(unit),whole,S/W,no_autoreload,word,count=128;
	
	// DMA on and start.
	rDMASKTRIG0=(1<<1)|(1<<0);
	
	while(!(rSRCPND & BIT_DMA0));	// Wait until Dma transfer is done.
	rSRCPND=BIT_DMA0;	
#endif
/*
    NF_MECC_Lock();
	// Get ECC data.
	// Spare data for 8bit
	// byte  0     1    2     3     4          5               6      7            8         9
	// ecc  [0]  [1]  [2]  [3]    x   [Bad marking]                    SECC0  SECC1
	Mecc = rNFMECC0;
	se8Buf[0]=(U8)(Mecc&0xff);
	se8Buf[1]=(U8)((Mecc>>8) & 0xff);
	se8Buf[2]=(U8)((Mecc>>16) & 0xff);
	se8Buf[3]=(U8)((Mecc>>24) & 0xff);
	se8Buf[5]=0xff;		// Marking good block

	NF_SECC_UnLock();
	//Write extra data(ECC, bad marking)
	for(i=0;i<4;i++)
	{
		NF_WRDATA8(se8Buf[i]);	// Write spare array(Main ECC)
		Spare_Data_1208[i]=se8Buf[i];
    }  
    NF_SECC_Lock(); 
	Secc=rNFSECC; 
	se8Buf[8]=(U8)(Secc&0xff);
	se8Buf[9]=(U8)((Secc>>8) & 0xff);
	for(i=4;i<16;i++)
	{
		NF_WRDATA8(se8Buf[i]);  // Write spare array(Spare ECC and Mark)
		Spare_Data_1208[i]=se8Buf[i];
	} 
*/	
 	NF_CLEAR_RB();
	NF_CMD(PROGCMD1);	 // Write 2nd command
//	NF_DETECT_RB();
	while(NFConDone==0);
	rNFCONT&=~(1<<9);
	rNFCONT&=~(1<<10); // Disable Illegal Access Interrupt
	if(rNFSTAT&0x8) return FAIL;

	NF_CMD(QUERYCMD);   // Read status command   
    
	for(i=0;i<3;i++);  //twhr=60ns
    
    if (NF_RDDATA()&0x1)  // Page write error
	{
    	NF_ChipDs();
		Uart_Printf("[PROGRAM_ERROR:block#=%d]\n",block);
		MarkBadBlock_2G08(block);
		return FAIL;
    } 
	else
	{
    	NF_ChipDs();
	    return OK;
	}
}

static int IsBadBlock_2G08(U32 block)
{
    int i;
    unsigned int blockPage;
	U8 data;
    
    blockPage=(block<<5);	// For 2'nd cycle I/O[7:5] 
    
	NF_ChipEn();
	NF_CLEAR_RB();

	NF_CMD(READCMD0);		 // Spare array read command
	NF_ADDR((2048+0)&0xff);		 // Read the mark of bad block in spare array(M addr=0)
	NF_ADDR(((2048+0)>>8)*0xff);
	NF_ADDR(blockPage&0xff);	 // The mark of bad block is in 0 page
	NF_ADDR((blockPage>>8)&0xff);	 // For block number A[24:17]
	NF_ADDR((blockPage>>16)&0xff);  // For block number A[25]
	
	NF_CLEAR_RB();
	NF_CMD(READCMD3);
	NF_DETECT_RB();	 // Wait tR(max 12us)

    data=NF_RDDATA();

	NF_ChipDs();    

    if(data!=0xff)
    {
    	Uart_Printf("[block %d has been marked as a bad block(%x)]\n",block,data);
    	return FAIL;
    }
    else
    {
    	return OK;
    }

}

static int MarkBadBlock_2G08(U32 block)
{
    int i;
	U32 blockPage=(block<<5);
 
    se8Buf[0]=0xff;
    se8Buf[1]=0xff;    
    se8Buf[2]=0xff;    
    se8Buf[5]=0x44;   // Bad blcok mark=44
    
	NF_ChipEn(); 
//	NF_CMD(READCMD2);   //????
	NF_CMD(PROGCMD0);   // Write 1st command
    
	NF_ADDR((2048+0)&0xff);			// 2060 = 0x080c
	NF_ADDR(((2048+0)>>8)&0xff);	// A[10:8]
	NF_ADDR((blockPage)&0xff);	// A[11;18]
	NF_ADDR((blockPage>>8)&0xff);	// A[26:19]
	NF_ADDR((blockPage>>16)&0xff);  //
    
	NF_WRDATA(0);

	NF_CLEAR_RB();
	NF_CMD(PROGCMD1);   // Write 2nd command
	NF_DETECT_RB();

	NF_CMD(QUERYCMD);
    
	for(i=0;i<3;i++);  //twhr=60ns////??????
    
    if (NF_RDDATA()&0x1) // Spare arrray write error
    {	
    	NF_ChipDs();
    	Uart_Printf("[Program error is occurred but ignored]\n");
    }
    else 
    {
    	NF_ChipDs();
    }

	Uart_Printf("[block #%d is marked as a bad block]\n",block);
    return OK;
}

static void InitNandFlash(void)
{
	InitNandCfg();
}

void __irq NFCon_Int(void)
{
    NFConDone=1;
	rINTMSK|=BIT_NFCON;
	ClearPending(BIT_NFCON);
	if(rNFSTAT&0x8) Uart_Printf("Illegal Access is detected!!!\n");
//	else Uart_Printf("RnB is Detected!!!\n"); 
}
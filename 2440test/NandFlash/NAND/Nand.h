#ifndef __NAND_H
#define __NAND_H

////////////////////////////// 8-bit ////////////////////////////////
// Main function
void Test_Nand(void);

// Sub function
void Test_K9F1208(void);
void Test_K9F2G08(void);
void PrintSubMessage(void);
void Test_Page_Write(void);
void Test_Page_Read(void);
void Test_Block_Erase(void);
void PrintBadBlockNum(void);

U16 Read_Status(void);
void Nand_Reset(void);

void Print_Id(void);
static U32 ReadChipId(void);
static int EraseBlock_1208(U32 blockNum);
static int ReadPage_1208(U32 block,U32 page,U8 *buffer);
static int WritePage_1208(U32 block,U32 page,U8 *buffer);
static int IsBadBlock_1208(U32 block);
static int MarkBadBlock_1208(U32 block);

static int EraseBlock_2G08(U32 blockNum);
static int ReadPage_2G08(U32 block,U32 page,U8 *buffer);
static int WritePage_2G08(U32 block,U32 page,U8 *buffer);
static int IsBadBlock_2G08(U32 block);
static int MarkBadBlock_2G08(U32 block);

static void InitNandFlash(void);

//*******************************************************

#endif /*__NAND_H*/


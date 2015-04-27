#ifndef __DS18B20_H_
#define __DS18B20_H_

//====================================================
// 常量定义区
//====================================================
#define   DQ_IN     rGPBCON &= 0xFFFFF3FF
#define   DQ_OUT    rGPBCON = (rGPBCON | 0xC00) & 0xFFFFF7FF
#define   DQ_DOWN   rGPBUP |= (1 << 5)
#define   DQ_UP     rGPBUP &= ~(1 << 5)
#define   DQ_H      rGPBDAT |= (1 << 5)
#define   DQ_L      rGPBDAT &= ~(1 << 5)
#define   DQ_STAT   (rGPBDAT >> 5) & 0x1

//定义DS18B20ROM指令
#define    DS18B20_ReadROM        0x33    //读ROM
#define    DS18B20_MatchROM       0x55    //匹配ROM
#define    DS18B20_SkipROM        0xCC    //跳过ROM
#define    DS18B20_SearchROM      0xF0    //搜索ROM
#define    DS18B20_AlarmROM       0xEC    //报警搜索

//定义DS18B20存储器操作命令
#define    DS18B20_WriteSCR       0x4E    //写暂存存储器
#define    DS18B20_ReadSCR        0xBE    //读暂存存储器
#define    DS18B20_CopySCR        0x48    //复制暂存存储器
#define    DS18B20_ConvertTemp    0x44    //温度变换
#define    DS18B20_RecallEP       0xB8    //重新调出
#define    DS18B20_ReadPower      0xB4    //读电源

void Test_DS18B20(void);

#endif
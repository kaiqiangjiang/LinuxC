#ifndef __DS18B20_H_
#define __DS18B20_H_

//====================================================
// ����������
//====================================================
#define   DQ_IN     rGPBCON &= 0xFFFFF3FF
#define   DQ_OUT    rGPBCON = (rGPBCON | 0xC00) & 0xFFFFF7FF
#define   DQ_DOWN   rGPBUP |= (1 << 5)
#define   DQ_UP     rGPBUP &= ~(1 << 5)
#define   DQ_H      rGPBDAT |= (1 << 5)
#define   DQ_L      rGPBDAT &= ~(1 << 5)
#define   DQ_STAT   (rGPBDAT >> 5) & 0x1

//����DS18B20ROMָ��
#define    DS18B20_ReadROM        0x33    //��ROM
#define    DS18B20_MatchROM       0x55    //ƥ��ROM
#define    DS18B20_SkipROM        0xCC    //����ROM
#define    DS18B20_SearchROM      0xF0    //����ROM
#define    DS18B20_AlarmROM       0xEC    //��������

//����DS18B20�洢����������
#define    DS18B20_WriteSCR       0x4E    //д�ݴ�洢��
#define    DS18B20_ReadSCR        0xBE    //���ݴ�洢��
#define    DS18B20_CopySCR        0x48    //�����ݴ�洢��
#define    DS18B20_ConvertTemp    0x44    //�¶ȱ任
#define    DS18B20_RecallEP       0xB8    //���µ���
#define    DS18B20_ReadPower      0xB4    //����Դ

void Test_DS18B20(void);

#endif
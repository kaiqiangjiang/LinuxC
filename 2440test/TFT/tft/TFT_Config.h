//====================================================================================
//�� �� ����TFT_Config.h
//��������: TFT�������������ļ��������ʵ��������ò���
//ά����¼: 2009��8��18��
//====================================================================================
#ifndef _TFT_CONFIG_H_
#define _TFT_CONFIG_H_

#define		TFT_COLOR_RESV		0								// �Ƿ�ת��ɫ
#define		TFT_MAXFONT			5								// �ֿ�����
#define		MAX_WINDOW 			16


#define		TFT_WIDTH			320								// ��Ļ���
#define		TFT_HEIGHT			240								// ��Ļ�߶�

#define		TFT_CLK_SET			C_TFT_CLK_27MDIV1				// ����ʱ��

#define		TFT_INTERFACE		C_LCD_PORT_TOPPOLY				// ��ʾ���ӿ�����
#define		TFT_DATA_SEQ		C_TFT_OUTPUT_YCBCR				// ��ʾ���ӿڵ�����˳��
#define		TFT_DATA_FMT		C_TFT_PARALLEL_RGB				// ��ʾ���ӿڵ����ݸ�ʽ


#define		TFT_HOR_ACT			TFT_WIDTH						// ˮƽ����ʱ����
#define		TFT_HOR_FRONT		20								// ˮƽFront-Porchʱ����
#define		TFT_HOR_BACK		67								// ˮƽBack-Porchʱ����
#define		TFT_HOR_SYNC		30								// ˮƽͬ��������

#define		TFT_VER_ACT			TFT_HEIGHT						// ��ֱ����ʱ����
#define		TFT_VER_FRONT		12								// ��ֱFront-Porchʱ����
#define		TFT_VER_BACK		17								// ��ֱBack-Porchʱ����
#define		TFT_VER_SYNC		3								// ��ֱͬ��������

////////////////////////////

#define		TFT_FRAME_FMT1		C_TFT_BUF_RGB565
#define		TFT_FRAME_FMT2		C_TFT_BUF_YUV

#define		TFT_WINDOW_WIDTH	TFT_WIDTH						// �߼���Ļ��ȳ�ʼֵ
#define		TFT_WINDOW_HEIGHT	TFT_HEIGHT						// �߼���Ļ�߶ȳ�ʼֵ

#define		TFT_BUFFER0_SA		0x30400000						// ��ʾBuffer0��ʼ��ַ
#define		TFT_BUFFER1_SA		(TFT_BUFFER0_SA + TFT_WIDTH * TFT_HEIGHT * 2)
																// ��ʾBuffer1��ʼ��ַ
#define		TFT_BUFFER2_SA		(TFT_BUFFER0_SA + TFT_WIDTH * TFT_HEIGHT * 4)
																// ��ʾBuffer2��ʼ��ַ
#define		TFT_DEPRESS_ADDR	0x30700000						// ���ڴ洢GUI��ʾͼ��ʱ����ʱ����



#endif

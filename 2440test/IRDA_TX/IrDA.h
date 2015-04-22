#ifndef __IrDA_H_
#define __IrDA_H_
#define IrDABUFFER  _NONCACHE_STARTADDRESS


#define GPRS 		0x00
#define GPS  		0x01
#define Bluetooth 	0x02
#define IrDA 		0x03

void IrDA_Port_Set(void);
void Select_Device(char Device);
void Test_IrDA_Tx(void);
void Test_IrDA_Rx(void);

#endif/*__IrDA_H_*/
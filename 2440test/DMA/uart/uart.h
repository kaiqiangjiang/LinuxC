#ifndef __UART_H_
#define __UART_H_

void Uart_Select(int ch);
void Uart_Init(int whichuart, int baud);
void Uart_TxEmpty(int ch);
void Uart_SendByte(char ch);
char Uart_ReceiveByte(void);
char Uart_Getch(void);
char Uart_GetKey(void);
int  Uart_GetIntNum(void);
int  Uart_GetIntNum_GJ(void);
void Uart_Send(char *str);
void Uart_receive(char *string);
void Uart_Printf(char *fmt,...);

#endif //__UART_H_
#ifndef __LCD_H_
#define __LCD_H_
#define LCD1602_4PINS
#include<8052.h>

#ifndef uchar
#define uchar unsigned char
#endif

#ifndef uint 
#define uint unsigned int
#endif
#define LCD1602_DATAPINS P0
#define LCD1602_E P2_7
#define LCD1602_RW P2_5
#define LCD1602_RS P2_6

void Lcd1602_Delay1ms(uint c);   
void LcdWriteCom(uchar com);
void LcdWriteData(uchar dat);	
void LcdInit();						  
void LcdDisplay(int value);
#endif

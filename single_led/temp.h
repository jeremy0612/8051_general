#ifndef __TEMP_H_
#define __TEMP_H_

#include <8051.h>

#define DSPORT P3_7

void Delay1ms(unsigned int c);
unsigned char Ds18b20Init();
void Ds18b20WriteByte(unsigned char com);
unsigned char Ds18b20ReadByte();
void  Ds18b20ChangTemp();
void  Ds18b20ReadTempCom();
int Ds18b20ReadTemp();

#endif

#ifndef __SERIAL_H_
#define __SERIAL_H_
#include<8052.h>

#ifndef uchar
#define uchar unsigned char
#endif

#ifndef uint 
#define uint unsigned int
#endif
void serial_init(void);
void serial_send(unsigned char x);
#endif

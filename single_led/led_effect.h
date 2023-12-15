#ifndef __LED_EFFECT_H_
#define __LED_EFFECT_H_
#include<8052.h>

#ifndef uchar
#define uchar unsigned char
#endif

#ifndef uint 
#define uint unsigned int
#endif

#define heart P1
#define CK P3_6

void func_config(unsigned char mode);
// ------------- Serial functions ------------
void serial_receive() __interrupt(4);
// ------------- Alert functions -------------
void pwm() __interrupt(1);
void alert_on();
void alert_off();
void temp_check() __interrupt(3);
//-------------- Effect functions ------------
void toggle();
void blink();
void sequential(unsigned char direction);
void accumulate();
void respective();
void middle_respective();
void binary_counter(unsigned char mode);

#endif

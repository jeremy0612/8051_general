#include "serial.h"

void serial_init(void)
{
	TMOD = 0x20; // Timer 1, 8-bit, auto reload
	TH1 = 0xFD;  // 9600 baudrate
	SCON = 0x50; // Serial: use timer1, receive enable
	TR1 = 1; // start timer 1

	EA = 1; // enable global interrupt
	ES = 1; // enable serial interrupt
}
void serial_send(unsigned char x)
{
	SBUF = x; // move data to buffer register, system will automatically send it
	while(TI==0); // wait until send successfully
	TI = 0;  // need to clear flag by software

}
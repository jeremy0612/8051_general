#include "temp.h"
void Delay1ms(unsigned int y)
{
	unsigned int x;
	for(y;y>0;y--)
		for(x=110;x>0;x--);
}

unsigned char Ds18b20Init()
{
	unsigned int i = 70;
	DSPORT = 0;			 	
	while(i--){;}
	DSPORT=1;			
	i=0;
	while(DSPORT)	
	{
		i++;
		if(i>5000)
			return 0;
	}
	return 1;
}
void Ds18b20WriteByte(unsigned char dat)
{
	unsigned int i,j;
	for(j=0;j<8;j++)
	{
		DSPORT=0;			
		i++;
		DSPORT=dat&0x01; 
		i=6;
		while(i--); 
		DSPORT=1;	
		dat>>=1;
	}
}

unsigned char Ds18b20ReadByte()
{
	unsigned char byte,bi;
	unsigned int i,j;	
	for(j=8;j>0;j--)
	{
		DSPORT=0;
		i++;
		DSPORT=1;
		i++;
		i++;
		bi=DSPORT;	
		byte=(byte>>1)|(bi<<7);						  
		i=4;		
		while(i--);
	}				
	return byte;
}
void  Ds18b20ChangTemp()
{
	Ds18b20Init();
	Delay1ms(1);
	Ds18b20WriteByte(0xcc);			 
	Ds18b20WriteByte(0x44);	   
//	Delay1ms(100);	
}

void  Ds18b20ReadTempCom()
{	

	Ds18b20Init();
	Delay1ms(1);
	Ds18b20WriteByte(0xcc);	 
	Ds18b20WriteByte(0xbe);	
}


int Ds18b20ReadTemp()
{
	int temp=0;
	unsigned char tmh,tml;
	Ds18b20ChangTemp();			 	
	Ds18b20ReadTempCom();			
	tml=Ds18b20ReadByte();		
	tmh=Ds18b20ReadByte();		
	temp=tmh;
	temp<<=8;
	temp|=tml;
	return temp;
}



#include"lcd.h"

void Lcd1602_Delay1ms(uint c)   
{
    uchar a,b;
	for (; c>0; c--)
        for (b=199;b>0;b--)
            for(a=1;a>0;a--);
}

#ifndef 	LCD1602_4PINS	 
void LcdWriteCom(uchar com)	 
{
	LCD1602_E = 0;   
	LCD1602_RS = 0;	  
	LCD1602_RW = 0;	 
	
	LCD1602_DATAPINS = com;   
	Lcd1602_Delay1ms(1);	

	LCD1602_E = 1;	      
	Lcd1602_Delay1ms(5);	
	LCD1602_E = 0;
}
#else 
void LcdWriteCom(uchar com)
{
	LCD1602_E = 0;	
	LCD1602_RS = 0;	
	LCD1602_RW = 0;

	LCD1602_DATAPINS = com;	
    Lcd1602_Delay1ms(1);

	LCD1602_E = 1;	
	Lcd1602_Delay1ms(5);
	LCD1602_E = 0;

//	Lcd1602_Delay1ms(1);
	LCD1602_DATAPINS = com << 4;
	Lcd1602_Delay1ms(1);

	LCD1602_E = 1;	
	Lcd1602_Delay1ms(5);
	LCD1602_E = 0;
}
#endif
		   
#ifndef 	LCD1602_4PINS		   
void LcdWriteData(uchar dat)			
{
	LCD1602_E = 0;	
	LCD1602_RS = 1;	
	LCD1602_RW = 0;	

	LCD1602_DATAPINS = dat; 
	Lcd1602_Delay1ms(1);

	LCD1602_E = 1;   
	Lcd1602_Delay1ms(5);   
	LCD1602_E = 0;
}
#else
void LcdWriteData(uchar dat)			
{
	LCD1602_E = 0;	  
	LCD1602_RS = 1;	  
	LCD1602_RW = 0;	  

	LCD1602_DATAPINS = dat;	
    Lcd1602_Delay1ms(1);

	LCD1602_E = 1;	 
	Lcd1602_Delay1ms(5);
	LCD1602_E = 0;

	LCD1602_DATAPINS = dat << 4; 
	Lcd1602_Delay1ms(1);

	LCD1602_E = 1;	 
	Lcd1602_Delay1ms(5);
	LCD1602_E = 0;
}
#endif
		   
#ifndef		LCD1602_4PINS
void LcdInit()						  
{
 	LcdWriteCom(0x38);  
	LcdWriteCom(0x0c);  
	LcdWriteCom(0x06);  
	LcdWriteCom(0x01);  
	LcdWriteCom(0x80);  
}
#else
void LcdInit()						 
{
	LcdWriteCom(0x32);	 
	LcdWriteCom(0x28);	
	LcdWriteCom(0x0c); 
	LcdWriteCom(0x06);  
	LcdWriteCom(0x01); 
	LcdWriteCom(0x80);  
}
#endif
#ifndef 	LCD1602_4PINS	
void LcdDisplay(int value)
{
  if (value < 0) 
  {
    LcdWriteData('-');
    value *= -1;
  }
  int digitCount = 0;
  int tempValue = value;
  while (tempValue > 0) 
  {
    tempValue /= 10;
    digitCount++;
  }
  // Handle leading zeros
  for (int i = 0; i < 4 - digitCount; i++) 
  {
    LcdWriteData('0');
  }
  // Display the value
  do 
  {
    int digit = value % 10;
    LcdWriteData(digit + '0');
    value /= 10;
  } while (value > 0);
}
#else
void LcdDisplay(int temp) 	
{
  	unsigned char datas[] = {0, 0, 0, 0, 0}; 
	float tp;  
	if(temp< 0)	
  	{
	  	LcdWriteCom(0x80);		
	    LcdWriteData('-');  		
		temp=temp-1;
		temp=~temp;
		tp=temp;
		temp=tp*0.0625*100+0.5;	
  	}
 	else
  	{			
	  	LcdWriteCom(0x80);		
	    LcdWriteData('+'); 		
		tp=temp;
		temp=tp*0.0625*100+0.5;	
	}
	datas[0] = temp / 10000;
	datas[1] = temp % 10000 / 1000;
	datas[2] = temp % 1000 / 100;
	datas[3] = temp % 100 / 10;
	datas[4] = temp % 10;
 
	LcdWriteCom(0x82);		  
	LcdWriteData('0'+datas[0]);

	
	LcdWriteCom(0x83);		 
	LcdWriteData('0'+datas[1]); 

	LcdWriteCom(0x84);	
	LcdWriteData('0'+datas[2]); 

	LcdWriteCom(0x85);		
	LcdWriteData('.'); 		

	LcdWriteCom(0x86);		 
	LcdWriteData('0'+datas[3]); 

	LcdWriteCom(0x87);		 
	LcdWriteData('0'+datas[4]); 
}

#endif

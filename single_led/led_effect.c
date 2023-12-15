#include "led_effect.h"
#include "temp.h"

// -------------- effect parameters & flag ----------------
extern unsigned int speed = 100;
extern unsigned char effect_on ;
extern unsigned char effect_skip ;
// -------------- alert parameters & flag -----------------
extern unsigned int pwm_width;
extern unsigned char pwm_flag;
extern unsigned char alert_flag;
volatile unsigned char sol = 10;

void func_config(unsigned char mode)
{
    /*This function is used to config the shift register based on the current func mode
            the args respectively: 0 ~ alert | 1 ~ effect mode | 2 ~ serial mode */
    switch (mode)
    {
        case 0:
            // alert mode 
            TMOD = 0x10;  // Set Timer 1 in 16-bit mode
            TH1 = 0xFC;    // Load high byte 
            TL1 = 0x18;    // Load low byte 
            ET1 = 1;  // Enable Timer 1 interrupt
            // ES = 0;
            TR1 = 1;  // Start Timer 1
            // alert_flag = 1;
            break;
        case 2:
            // serial mode
            TMOD = 0x20; // Timer 1, 8-bit, auto reload
            TH1 = 0xFD;  // 9600 baudrate
            SCON = 0x50; // Serial: use timer1, receive enable
            ET1 = 0; // Disable Timer 1 interrupt
            ES = 1; // enable serial interrupt
            TR1 = 1; // start timer 1
            // alert_flag = 1;
            break;
        default:
            break;
    }
}
//==================== Serial mode definition ============================
void serial_receive() __interrupt(4)
{
    P2_2 = !P2_2;
	if(RI) // check if data received is available on SBUF
	{
		heart = SBUF; // move data from buffer to another shift register
		RI = 0; // need to clean the flag
	}
}
//=======================================================================

//==================== Alert mode definition ============================
void alert_on()
{
    TMOD |= 0x01; // Timer 0, mode 1 (16 bit mode)
    TH0 = pwm_width;  // Load high byte 
    TL0 = 0;  // Load low byte 
    ET0 = 1;  // Enable Timer 0 interrupt
    // IE = 0x82;   // Enable Timer 0 overflow interrupt
    TR0 = 1;    // Start Timer 0
    // while (TF0 == 0);
    // IE = 0;     // Clear Timer 0 overflow flag
    // TR0 = 0;    // Stop Timer 0
}
void alert_off()
{
    // ET0 = 0;
    // IE = 0;
    TR0 = 0;    // Stop Timer 0
}
void pwm() __interrupt(1)
{
    if(sol == 0)
    {
        pwm_width += 10;
        if(pwm_width > 255)
        {
            pwm_width = 0;
            Delay1ms(50);
        }
        sol = 10;
    }
    else
        sol--;

    if (!pwm_flag) 
    {	/* Start of High level */
        pwm_flag = 1;	/* Set flag */
        heart = 0x55;		/* Set PWM o/p pin */
        TH0 = pwm_width;	/* Load timer */
        TF0 = 0;		/* Clear interrupt flag */
    } 
    else 
    {			/* Start of Low level */
        pwm_flag = 0;	/* Clear flag */
        heart = 0xaa;		/* Clear PWM o/p pin */
        TH0 = 255 - pwm_width;	/* Load timer */
        TF0 = 0;		/* Clear Interrupt flag */
    }
}
// ---------------- Timer 1 interrupt for periodly temp checking -------------------
void temp_check() __interrupt(3)
{
    int temp = Ds18b20ReadTemp();			
    float tp=temp;
    temp=tp*0.0625+0.5;	
    
    if(temp >= 40 )
    {
        alert_on(); // turn on alert
        alert_flag = 1;
    }
    else if (temp < 40 )
    {
        alert_off();
        alert_flag = 0;
    }
    TF1 = 0;
}
// ------------------------------------------------------------------------
//=======================================================================




//====================== Effect mode definition =========================
void toggle()
{   effect_on = !effect_on; }
void blink()
{
    heart = 0xAA;
    Delay1ms(speed*2);
    heart = 0x55;
    Delay1ms(speed*2);
    if(effect_on == 0 || effect_skip == 1)
        return;
}
void sequential(unsigned char direction) 
{
    heart = 0xFF;
    if(direction == 0)
        for (int i = 4; i <= 8; ++i) 
        {
            heart &= ~(1 << i); // turn on bit i
            Delay1ms(speed+100);
        }
    else if(direction == 1)
        for (int i = 3; i >= 0; --i) 
        {
            heart &= ~(1 << i); // turn on bit i
            Delay1ms(speed+100);
        }
    Delay1ms(speed);
    // heart |= (1 << 8-i-1);
    if(effect_on == 0 || effect_skip == 1)
        return;
    CK = 0;
    CK = 1;
}

void accumulate()
{
    for (int i = 0; i < 8; i++) 
    {
        for(int j=0; j<8-i; j++)
        {   
            heart |= (1 << j); // turn on bit j
            Delay1ms(speed);
            heart &= ~(1 << j); // turn off bit j
            //Delay1ms(speed+100);
        }
        heart |= (1 << 8-i-1);
        if(effect_on == 0 || effect_skip == 1)
            return;
        Delay1ms(speed);
        CK = 0;
        CK = 1;
    }
    for (int i = 0; i < 8; i++) 
    {
        for(int j=0; j<8-i; j++)
        {   
            heart &= ~(1 << j); // turn off bit j
            Delay1ms(speed);
            heart |= (1 << j); // turn on bit j
            //Delay1ms(speed+200);
        }
        heart &= ~(1 << 8-i-1);
        if(effect_on == 0 || effect_skip == 1)
            return;
        Delay1ms(speed);
        CK = 0;
        CK = 1;
    }

}
void respective()
{
    for (int i = 0; i < 8; i++) 
    {
        heart |= (1 << i);
        Delay1ms(speed);
        if(effect_on == 0 || effect_skip == 1)
            return;
        CK = 0;
        CK = 1;
    }
    for (int i = 0; i < 8; i++) 
    {
        heart &= ~(1 << i);
        Delay1ms(speed);
        if(effect_on == 0 || effect_skip == 1)
            return;
        CK = 0;
        CK = 1;
    }
}
void middle_respective()
{
    unsigned char x, y = 4;
    for(x = 4; x <= 8; x++)
    {
        Delay1ms(speed);
        heart |= (1 << x);
        heart |= (1 << y);
        if(effect_on == 0 || effect_skip == 1)
            return;
        y--;
        CK = 0;
        CK = 1;
    }
    for(x = 8; x >= 4; x--)
    {
        Delay1ms(speed);
        y++;
        heart &= ~(1 << x);
        heart &= ~(1 << y);
        if(effect_on == 0 || effect_skip == 1)
            return;
        CK = 0;
        CK = 1;
    }
}
void binary_counter(unsigned char mode)
{
    if(mode == 1)
    {
        heart = 0x00;
        for(int i=0; i<257; i++)
        {
            heart = i;
            if(effect_on == 0 || effect_skip == 1)
                return;
            Delay1ms(speed);
            CK = 0;
            CK = 1;
        }
    }
    else if(mode == 0)
    {
        heart = 0xff;
        for(int i=256; i>=0; i--)
        {
            heart = i;
            if(effect_on == 0 || effect_skip == 1)
                return;
            Delay1ms(speed);
            CK = 0;
            CK = 1;
        }
    }
}
//=======================================================================
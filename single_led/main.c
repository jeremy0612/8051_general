#include <8052.h>
#include "temp.h"   // bultin ds18b20
#include "lcd.h"    // bultin lcd
#include "serial.h"   // new serial
#include "led_effect.h" // new led effect
#define CK P3_6

// ========== From high to low state of bar chart ===========
volatile unsigned char bars[8] = {0xFE, 0xFC, 0xF8, 0xF0, 0xE0, 0xC0, 0x80, 0x00};
// --> reserved 
// ==========================================================

// ========== Controller variable : metadata ================
volatile unsigned char led_mode = 1;
volatile unsigned char effect = 0; // current effect
volatile unsigned char datas[] = {0, 0, 0, 0, 0}; 

//----------- linked var from led_effect module ------------
unsigned char effect_on = 1; 
unsigned char effect_skip = 0; 
//           pwm handler
unsigned int pwm_width = 0; 
unsigned char pwm_flag = 0;
unsigned char alert_flag;
// ==========================================================

// =========== Customized functions for program =============
void configure();
void effect_mode() __interrupt(0);
void func_mode() __interrupt(2);
void change_mode_display();
// ==========================================================

void main(void)
{
    // ------------- I/O Initialization & setup --------------
	LcdInit();			
	LcdWriteCom(0x88);	
	LcdWriteData('C'); 
    Ds18b20Init();

    configure();
    func_config(0); // let alert as initial 
    change_mode_display();
    
    P1 = 0x00;
    P2_1 = 1;
    // ------------ main loop -------------
	while(1)
	{
        while(alert_flag == 1); 
        if(1) // effect mode
        {
            if(effect_skip == 1)
                effect_skip = 0; // the effect skipped

            P1 = 0x00; // default display
            switch (effect)
            {
                case 0:
                    accumulate();
                    break;
                case 1:
                    respective();
                    break;
                case 2:
                    middle_respective();
                    break;
                case 3:
                    binary_counter(0);
                    break;
                case 4:
                    binary_counter(1);
                    break;
                case 5:
                    sequential(0);
                    break;
                case 6:
                    sequential(1);
                    break;
                case 7:
                    blink();
                    break;
                default:
                    break;
            }
        }
        // else if (led_mode != 0)
        // {
            
        //     // heart = 0x00;
        // }
        // Delay1ms(100);
    }
}


// ==================== Interrupt & timer configuration =================
void configure()
{
    EX0 = 1;    // Enable INT0 interrupt
    EX1 = 1;    // Enable INT1 interrupt
    IT0 = 1;  // Set INT0 to be edge-triggered (1 for falling edge, 0 for low level)
    IT1 = 1;  // Set INT1 to be edge-triggered (1 for falling edge, 0 for low level)
    EA = 1;   // Enable global interrupts
}

// =============== External interrupt for mode changing ================
void effect_mode() __interrupt(0)
{
    led_mode = 1;
    effect_on = 1;
    effect_skip = 1;
    if(effect == 7)
        effect = 0;
    else
        effect += 1;
    led_mode = 0;
    func_config(led_mode);
    // alert_flag = 1;
    // alert_on();    
    
    change_mode_display();
}
void func_mode() __interrupt(2)
{
    led_mode = 2;    // serial mode
    alert_flag = 1;
    alert_off(); 
    func_config(led_mode);
        
    // func_config(led_mode);
    LcdWriteCom(0x82);		 
    LcdWriteData('0'+led_mode); 
    change_mode_display();
}
void change_mode_display()
{
    LcdWriteCom(0xC0);
    if(led_mode == 0)
    {
        unsigned char *display = "Temp mode";        
        for(int i=0;i<9;i++)
            LcdWriteData(display[i]);
    }
    else if(led_mode == 1)
    {
        unsigned char *display = "Effect mode";        
        for(int i=0;i<11;i++)
            LcdWriteData(display[i]);
    }
}
















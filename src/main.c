/*
 * @Author: your name
 * @Date: 2021-04-02 23:16:06
 * @LastEditTime: 2021-04-30 00:45:34
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \WSJ\src\main.c
 */
#include "IIC.h"
#include "ADC.h"
#include "power.h"
#include "Tools.h"
#include "OB38R08A1.h"
#include "oled.h"


#define BT_POW P0_0
#define BT_MIN P3_1
#define BT_ADD P3_0

u8 test;
u8 add;
void testRead(u8 add)
{
    Debug(add);
    if(ReadCmd(add,&test))
    {
        Debug(test);
    }else
        Debug(0xE0);
}

void main()
{
    //ADC_Init();
    //ADC();

    initPow();
    Initial();
    Display_test();
    Delay_ms(100);  
   
    WriteCmd(0x01,0x31);
    WriteCmd(0x02,0xC0);
    WriteCmd(0x03,0x7C);
    WriteCmd(0x04,0xC0);
    WriteCmd(0x05,0xFF);
    WriteCmd(0x06,0xFF);
    WriteCmd(0x07,0x2C);
    WriteCmd(0x08,0x34);
    WriteCmd(0x09,0x8C);
    WriteCmd(0x0a,0x11);
    WriteCmd(0x0b,0x01);
    WriteCmd(0x0c,0x22);
    WriteCmd(0x00,0x09);
    Delay_ms(100);  
    
    startPow();

    Delay_ms(100);  

    //WriteCmd(0x0a,0x11);
    //Delay_ms(1000);  

    while(1)
    { 
        testRead(0x00); 
        Delay_ms(100); 
        testRead(0x01); 
        Delay_ms(100); 
        testRead(0x02); 
        Delay_ms(100); 
        testRead(0x03);
        Delay_ms(100); 
        testRead(0x04);
        Delay_ms(100); 
        testRead(0x05);
        Delay_ms(100); 
        testRead(0x06);
        Delay_ms(100); 
        testRead(0x07);
        Delay_ms(100);  
        testRead(0x08);
        Delay_ms(100); 
        testRead(0x09);
        Delay_ms(100);  
        testRead(0x0A);
        Delay_ms(100); 
        testRead(0x0B);
        Delay_ms(100); 
        testRead(0x0C);
        Delay_ms(100); 
        testRead(0x0F);
        Delay_ms(100); 
        testRead(0x10);
        Delay_ms(10000);    
    }
}

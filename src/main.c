/*
 * @Author: your name
 * @Date: 2021-04-02 23:16:06
 * @LastEditTime: 2021-04-19 01:04:23
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \WSJ\src\main.c
 */
#include "IIC.h"
#include "ADC.h"
#include "power.h"
#include "Tools.h"
#include "OB38R08A1.h"
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
    ADC_Init();
    //ADC();
    initPow();


    WriteCmd(0x09,0x84);

    Delay_ms(1000);   
    WriteCmd(0x00,0x11);
    Delay_ms(1000);  
    WriteCmd(0x08,0x3B);
    Delay_ms(1000);  
    WriteCmd(0x0c,0x22);
    // if(WriteCmd(0x09,0x84))
    //     Debug(0x66);
    // else
    //     Debug(0xe0);


    //WriteCmd(0x0a,0x11);
    //Delay_ms(1000);  

    while(1)
    { 
        testRead(0x01); 
        Delay_ms(100); 
        testRead(0x02);
        Delay_ms(100);  
        testRead(0x08);
        Delay_ms(100); 
        testRead(0x09);
        Delay_ms(100);  
        testRead(0x0f);
        Delay_ms(100); 
        testRead(0x10);
        Delay_ms(10000);    
    }
}

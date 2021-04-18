/*
 * @Author: your name
 * @Date: 2021-04-16 23:27:08
 * @LastEditTime: 2021-04-18 23:01:56
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \WSJ\src\Tools.c
 */
#include "Tools.h"
void uartDelay(void) //波特率1200
{
    u8 i, j;

    for (i = 0; i < 8; i++)
    {
        for (j = 0; j < 165; j++)
        {
            ;
        }
    }
}

//延时10*n微秒
void Delay_10us(u8 n)
{
    u8 i, j;
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < 8; j++)
        {
            ;
        }
    }
}

void Delay_ms(u16 n)
{
    u16 i, j;
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < 878; j++)
        {
            ;
        }
    }
}


void Debug(u8 dat)
{
    
    u8 i = 8;

    P1_5 = 0;
    uartDelay();

    while (i--)
    {
        if (dat & 1)
            P1_5 = 1;
        else
            P1_5 = 0;
        uartDelay();
        dat >>= 1;
    }
    P1_5 = 1;
    uartDelay();
}
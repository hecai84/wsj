/*
 * @Author: your name
 * @Date: 2021-04-18 09:32:50
 * @LastEditTime: 2021-04-19 22:54:45
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \WSJ\src\power.c
 */
#include "Tools.h"
#include "IIC.h"
#define CE P0_6
#define PSTOP P0_7
#define INT P1_6
#define M_CTRL P1_7
u8 I2cRecArr[10]={0};

void startPow(void)
{
    PSTOP=0;
}

void initPow(void)
{
    PSTOP=1;
    CE=0;
    P1M0=P1M0 | 0x80;
    M_CTRL=1;
    P0M0=P0M0 | 0xC4;
    P0_2=0;
}




u8 ReadCmd(u8 addr,u8 * dat)
{
    if(read_add(addr,I2cRecArr,1))
    {
        *dat=I2cRecArr[0];
        return 1;
    }else
        return 0;
}
u8 WriteCmd(u8 addr,u8 dat)
{
    if(write_add(addr,&dat,1))
    {
        return 1;
    }else
        return 0;
}

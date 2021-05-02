/*
 * @Author: your name
 * @Date: 2021-04-18 09:32:50
 * @LastEditTime: 2021-05-02 16:39:30
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \WSJ\src\power.c
 */
#include "power.h"
#include "IIC.h"
#include "EEPROM.h"
#define CE P0_6
#define PSTOP P0_7
#define INT P1_6
#define M_CTRL P1_7
#define FB_CTRL P0_2
u8 I2cRecArr[10]={0};
u8 curVolt;
u8 curOtg=1;

void startPow(void)
{
    WriteCmd(0x09,0x8C);
    Delay_ms(100);  
    curOtg=1;
    //PSTOP=0;
    M_CTRL=1;
}
void stopPow(void)
{
    //PSTOP=1;    
    WriteCmd(0x09,0x0C);
    curOtg=0;
    M_CTRL=0;
    Delay_ms(100);  
}
void stop8812(void)
{
    CE=1;
    PSTOP=1;
}

void init8812(void)
{
    PSTOP=1;
    CE=0;
    // P1M0=P1M0 | 0x80;
    // P0M0=P0M0 | 0xC4;
    M_CTRL=0;
    FB_CTRL=0;
    curVolt=Read_EEPROM(7);    
    Delay_ms(100);  
    if(curVolt>=35 && curVolt<=150)
        SetVolt(curVolt);
    else
        SetVolt(50);
   
    WriteCmd(0x01,0x31);
    WriteCmd(0x02,0xC0);
    WriteCmd(0x05,0xFF);
    WriteCmd(0x06,0xFF);
    WriteCmd(0x07,0x2C);
    WriteCmd(0x08,0x3A);
    WriteCmd(0x09,0x0C);
    WriteCmd(0x0a,0x11);
    WriteCmd(0x0b,0x01);
    WriteCmd(0x0c,0x22);
    WriteCmd(0x00,0x09);

    
    Delay_ms(100);  
    curOtg=0;
    PSTOP=0;
}

void VoltAdd()
{
    if(curVolt<150)
    {
        curVolt++;
        SetVolt(curVolt);
    }
}
void VoltMin()
{
    if(curVolt>35)
    {
        curVolt--;
        SetVolt(curVolt);
    }
}

/**
 * @description: 设置电压
 * @param {u8} v  电压*10
 * @return {*}
 */
void SetVolt(u8 v)
{
    u8 set1,set2=0;
    u32 temp;
    curVolt=v;
    temp=v*1000;
    if(v>80)
    {
        FB_CTRL=1;
        set1=temp/747;    //(1+100/12)*8*10
        set2=((temp%747)/186)<<8;  //(1+100/12)*2*10
    }
    else
    {
        FB_CTRL=0;
        set1=temp/413;    //(1+100/24)*8*10
        set2=((temp%413)/103)<<8;  //(1+100/24)*2*10
    }    

    WriteCmd(0x03,set1);
    WriteCmd(0x04,set2);
}
/**
 * @description: 获取电池电量 
 * @param {*}
 * @return {*}
 */
u8 GetBat()
{
    u8 value1,value2;
    u16 v;
    if(ReadCmd(0x0F,&value1))
    {
        if(ReadCmd(0x10,&value2))
        {
            v=value1;
            v=(v*4+(value2>>8)+1)*10;
            if(v<6800)
                return 0;
            else if(v<7200)
                return 1;
            else if(v<7600)
                return 2;
            else if(v<8000)
                return 3;
            else
                return 4;
        }
    }
    return 0;
}
/**
 * @description:获取电流x100 
 * @param {*}
 * @return {*}
 */
u8 GetIBus()
{
    u8 value1,value2;
    u16 A;
    if(ReadCmd(0x11,&value1))
    {
        if(ReadCmd(0x12,&value2))
        {
            A=value1;
            A=(A*4+(value2&0x3)+1)/2;
            return A;
        }
    }
    return 0;
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

/*
 * @Author: your name
 * @Date: 2021-04-18 09:32:50
 * @LastEditTime: 2021-05-14 01:08:47
 * @LastEditors: huzhenhong
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
#define IBUSARR_LEN 10
#define VBATARR_LEN 5
u8 idata I2cRecArr[10]={0};
u8 idata iBusArr[IBUSARR_LEN]={0};
u16 idata vBatArr[VBATARR_LEN]={0};
u8 curVolt;
u8 isOtg=0;
u8 forcePow=0;
u8 stableIBus=0;
u8 stableCount=0;
u8 emptyIBus=0;

u8 ReadCmd(u8 addr,u8 * dat);
void setIBusLim(u8 v);


void startPow(void)
{
    u16 i;
    u8 tempVolt=curVolt;
    stableIBus=0;
    stableCount=0;
    M_CTRL=0;
    if(curVolt<100 && forcePow==1)
    {
        SetVolt(100);
    }
    
    WriteCmd(0x05,0xFF);
    WriteCmd(0x06,0xFF);
    WriteCmd(0x09,0x86);
    isOtg=0;
    for(i=0;i<30;i++)
    {
        emptyIBus=GetIBusAvg();
    }

    isOtg=1;

    //PSTOP=0;
    Delay_ms(100);  
    for(i=0;i<1000;i++)
    {
        M_CTRL=0;
        Delay_10us(1);    
        M_CTRL=1;
        Delay_10us(1);  
    }
     
    for(i=0;i<1000;i++)
    {
        M_CTRL=0;
        Delay_10us(1);    
        M_CTRL=1;        
        Delay_10us(1); 
        Delay_10us(1);  
    }

    M_CTRL=1;
    Delay_ms(100);
    setIBusLim(tempVolt);
    SetVolt(tempVolt);
}
void stopPow(void)
{
    //PSTOP=1;    
    WriteCmd(0x06,0x19);
    WriteCmd(0x09,0x06);
    isOtg=0;
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
    P1M0=P1M0 | 0x80;
    // P0M0=P0M0 | 0xC4;
    M_CTRL=0;
    forcePow=Read_EEPROM(6);
    if(forcePow!=1)
        forcePow=0;
    curVolt=Read_EEPROM(7);    
    Delay_ms(100);  
    if(curVolt>=30 && curVolt<=150)
        SetVolt(curVolt);
    else
        SetVolt(50);

    WriteCmd(0x05,0x95);
    WriteCmd(0x06,0x19);
    WriteCmd(0x07,0x2C);
    WriteCmd(0x08,0x3B);
    WriteCmd(0x09,0x06);
    WriteCmd(0x0a,0x01);
    WriteCmd(0x0b,0x01);
    WriteCmd(0x0c,0x22);
    WriteCmd(0x00,0x09);


    
    Delay_ms(100);  
    isOtg=0;
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
    if(curVolt>30)
    {
        curVolt--;
        SetVolt(curVolt);
    }
}
/**
 * @description: 设置限流
 * @param {u8} v
 * @return {*}
 */
void setIBusLim(u8 v)
{
    if(v<60)
    {
        WriteCmd(0x05,0x95);
    }else if(v<100)
    {
        WriteCmd(0x05,0x55);
    }else
    {
        WriteCmd(0x05,0x35);
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
    temp=v*100;
    setIBusLim(v);
    if(v>102)
    {
        //FB_CTRL=1;
        WriteCmd(0x08,0x3A);
        set1=temp/100;    //(1+100/12)*8*10
        set2=0;  //(1+100/12)*2*10
    }
    else
    {
        //FB_CTRL=0;
        WriteCmd(0x08,0x3B);
        set1=temp/40;    //(1+100/24)*8*10
        set2=0;  //(1+100/24)*2*10
    }    

    WriteCmd(0x01,set1);
    //WriteCmd(0x02,set2);
}
/**
 * @description: 动态调整电压
 * @param {u8} i
 * @return {*}
 */
void SetVoltTemp(u8 i)
{
    u8 temp;
    u8 tempVolt=curVolt;
    if(curVolt<90)
    {
        if(i<=stableIBus)
        {
            SetVolt(curVolt);
        }else
        {
            temp=(i-stableIBus)*6/10+curVolt;
            if(temp>90)
                temp=90;
            SetVolt(temp);
            curVolt=tempVolt;
        }
    }
}


/**
 * @description: 获取电池电量 
 * @param {*}
 * @return {*}
 */
u16 GetVBat()
{
    u8 value1,value2;
    u16 v=0;
    if(ReadCmd(0x0F,&value1))
    {
        if(ReadCmd(0x10,&value2))
        {
            v=value1;
            v=(v*4+(value2>>6)+1)*10;      
        }
    }
    return v;
}

u8 GetVBatAvg()
{
    u8 i;
    u16 temp=vBatArr[0];

    for(i=1;i<VBATARR_LEN;i++)
    {
        temp+=vBatArr[i];
        vBatArr[i-1]=vBatArr[i];
    }
    vBatArr[VBATARR_LEN-1]=GetVBat();
    temp=temp/VBATARR_LEN; 

    if(temp<6600)
        return 0;
    else if(temp<7000)
        return 1;
    else if(temp<7400)
        return 2;
    else if(temp<8000)
        return 3;
    else
        return 4;
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
            A=(A*4+(value2>>6)+1);
            return A;
        }
    }
    return 0;
}


u8 GetIBusAvg()
{
    u8 i;
    u16 temp=iBusArr[0];

    for(i=1;i<IBUSARR_LEN;i++)
    {
        temp+=iBusArr[i];
        iBusArr[i-1]=iBusArr[i];
    }
    iBusArr[IBUSARR_LEN-1]=GetIBus();
    i=temp/IBUSARR_LEN;
    if(isOtg==1)
    {
        if(i>emptyIBus)
            i=i-emptyIBus;
        else
            i=0;
    }


    if(isOtg==1 && forcePow==0 && stableCount<10 && i>10)
    {        
        if(stableIBus>i && stableIBus-i<2)
        {
            stableCount++;
        }else if(stableIBus<=i && i-stableIBus<2)
        {
            stableCount++;
        }else
            stableCount=0;
        stableIBus=i;
    }
    if(isOtg==1 && forcePow==0 && stableCount>=5)
        SetVoltTemp(i);
    return i;
}


// u8 GetIBat()
// {
//     u8 value1,value2;
//     u16 A;
//     if(ReadCmd(0x13,&value1))
//     {
//         if(ReadCmd(0x14,&value2))
//         {
//             A=value1;
//             A=(A*4+(value2>>6)+1);
//             return A;
//         }
//     }
//     return 0;
// }


// u8 GetIBatAvg()
// {
//     u8 i;
//     u16 temp=iBatArr[0];

//     for(i=1;i<IBUSARR_LEN;i++)
//     {
//         temp+=iBatArr[i];
//         iBatArr[i-1]=iBatArr[i];
//     }
//     iBatArr[IBUSARR_LEN-1]=GetIBat();
//     i=temp/IBUSARR_LEN;
//     if(isOtg==1)
//     {
//         if(i>emptyIBat)
//             i=i-emptyIBat;
//         else
//             i=0;
//     }
        
//     return i;
// }



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

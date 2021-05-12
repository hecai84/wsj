/*
 * @Author: your name
 * @Date: 2021-04-02 23:16:06
 * @LastEditTime: 2021-05-11 23:55:18
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \WSJ\src\main.c
 */
#include "IIC.h"
#include "EEPROM.h"
#include "power.h"
#include "Tools.h"
#include "OB38R08A1.h"
#include "oled.h"
#include "Timer.h"

#define BT_POW   P3_0
#define BT_ADD  P3_1
#define BT_MIN   P0_0
#define POW_INT P1_6

u8 test;
u8 num;
u8 curStop,curBtPow,curBtMin,curBtAdd=1;
u32 clickTime;
u32 refreshTime;
extern u8 curVolt;
extern u8 curOtg;
extern u8 curDisplay;
extern u8 forcePow;

void refreshDisplay();




void testRead(u8 add)
{
    Debug(add);
    if(ReadCmd(add,&test))
    {
        Debug(test);
    }else
        Debug(0xE0);
}

/**
 * @description: 系统停止
 * @param {*}
 * @return {*}
 */
void SystemStop()
{
    curStop=0;
    stop8812();
    //闪屏提醒
    clear();
    Delay_ms(300);
    refreshDisplay();
    Delay_ms(300);
    clear();
    Delay_ms(300);
    refreshDisplay();
    Delay_ms(300);
    clear();
    Delay_ms(300);
    refreshDisplay();
    Delay_ms(300);
    clear();
    DisplayOff();
    
    //mcu休眠
    PCON=2;
}
/**
 * @description: 系统恢复
 * @param {*}
 * @return {*}
 */
void SystemResume()
{
    PCON=0;    
    init8812();
    DisplayOn();
}

void init()
{
    //定时器初始化
    TIMER0_initialize();    
    TR0  = 1;
    TR1  = 1;
    //EX0 = 1;        //开启外部中断0
    //IT0 = 1;                //设置外部中断0触发模式:下降沿触发
    IEKBI = 1;         //启用键盘中断
    EA   = 1;//中断开启

    //屏幕初始化
    Initial();
    clear();
    
    //8812初始化    
    init8812();
    DisplayChar_b(curVolt);

}


void refreshDisplay()
{
    u32 diffTime=GetSysTick()-refreshTime;
    if(diffTime>300)
    {        
        DisplayChar_s(GetIBus());
        refreshTime=GetSysTick();

        if(POW_INT==0 && curOtg==0)
            DisplayBat(255);
        else
            DisplayBat(GetBat());
        
        DisplayShan_s(forcePow);
    }
}

void waitClickUp()
{
    while(BT_ADD==0 || BT_POW==0 || BT_MIN==0)
    {
        ;
    }
}

void powClick()
{
    if(curOtg==1)
    {
        stopPow();
    }
    else 
    {
        if(curDisplay==1)
            startPow();
    }
}
/**
 * @description: 长按加和减按钮
 * @param {*}
 * @return {*}
 */
void doubleAddMin()
{
    forcePow=~forcePow & 1;
    DisplayShan_s(forcePow);
    waitClickUp();
}

void doublePowAdd()
{
}
void doublePowMin()
{
}
void powClickLong2()
{
    SystemStop();
}

void powClickLong()
{
    u32 diffTime;  

    while(1)
    {
        diffTime=GetSysTick()-clickTime;
        if(BT_ADD==0)
        {
            doublePowAdd();
        }
        else if(BT_MIN==0)
        {
            doublePowMin();
        }
        else if(diffTime>2000)
        {
            if(curStop==0)
            {
                SystemResume();
            }else{
                if(curDisplay)
                {
                    stopPow();
                    DisplayOff();
                    Write_EEPROM(6,forcePow);
                    Write_EEPROM(7,curVolt);
                }
                else
                {
                    DisplayOn();
                }
                while(BT_POW==0)
                {
                    diffTime=GetSysTick()-clickTime;
                    if(diffTime>5000)
                    {
                        powClickLong2();
                        break;
                    }
                }
            }
        }else 
            continue;
        
        waitClickUp();
        break;
    } 
}


void minClick()
{
    VoltMin();
    DisplayChar_b(curVolt);
}
void minClickLong()
{
    VoltMin();
    DisplayChar_b(curVolt);
    Delay_ms(50);
    if(BT_ADD==0)
    {
        doubleAddMin();
    }
}
void addClick()
{
    VoltAdd();
    DisplayChar_b(curVolt);
}
void addClickLong()
{
    VoltAdd();
    DisplayChar_b(curVolt); 
    Delay_ms(50);    
    if(BT_MIN==0)
    {
        doubleAddMin();
    }
}


/**
 * @description: 处理按键
 * @param {*}
 * @return {*}
 */
void procClick()
{
    u32 diffTime;
    if(BT_POW==0)
    {
        if(curBtPow==1)
        {
            curBtPow=0;
            clickTime=GetSysTick();
            powClick();
        }else
        {
            diffTime=GetSysTick()-clickTime;
            if(diffTime>1000)
            {
                powClickLong();
            }
        }
    }else if(BT_ADD==0)
    {
        if(curBtAdd==1)
        {
            curBtAdd=0;
            clickTime=GetSysTick();
            addClick();
        }else
        {
            diffTime=GetSysTick()-clickTime;
            if(diffTime>1000)            
                addClickLong();            
        }
    }else if(BT_MIN==0)
    {
        if(curBtMin==1)
        {
            curBtMin=0;
            clickTime=GetSysTick();
            minClick();
        }else
        {
            diffTime=GetSysTick()-clickTime;
            if(diffTime>1000)            
                minClickLong();            
        }
    }else
    {
        curBtPow=1;
        curBtMin=1;
        curBtAdd=1;
    }
}


void main()
{
    init();

    Delay_ms(100);  

    num=0;
    DisplayBat(4);
    refreshTime=GetSysTick();
    while(1)
    {         
        procClick();
        refreshDisplay();
        // testRead(3);
        // testRead(9);
        // Delay_ms(200);
      
        
    }
}

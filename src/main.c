/*
 * @Description: 
 * @Author: hecai
 * @Date: 2021-05-12 10:42:58
 * @LastEditors: huzhenhong
 * @LastEditTime: 2021-05-24 22:44:15
 * @FilePath: \WSJ\src\main.c
 */
#include "IIC.h"
#include "EEPROM.h"
#include "power.h"
#include "Tools.h"
#include "OB38R08A1.h"
#include "oled.h"
#include "Timer.h"


#define KBI_VECTOR  11          //KBI Interrupt Vevtor
#define d_KBLS      0x00        //KBI Low/High level detection selection (0~0x0F)
#define d_KBEX      0x01        //KBI Input Enable (0~0x0F)
#define d_KBDEN     0x00        //KBI De-bounce Function Enable
#define d_KBDS      0x00        //KBD[1:0] KBI De-bounce Time Selection (0~3)
#define d_KBIIE     0x01        //KBI Interrupt Enable bit

#define BT_POW   P3_0
#define BT_ADD  P3_1
#define BT_MIN   P0_0
#define POW_INT P1_6

u8 test;
u8 num;
u8 readyResume=0;
u8 isRunning=1,curBtPow=1,curBtMin=1,curBtAdd=1;
u16 curIBus;
u32 clickTime;
u32 refreshTime;
u32 refreshIBusTime;
extern u8 curVolt;
extern u8 isOtg;
extern u8 isDisplay;
extern u8 tempDisplay;
extern u8 forcePow;
u8 getVbatCount=255;
u8 curVBat;


//函数定义--------------------------------
void refreshDisplay();
void powClickLong();
//---------------------------------------


// void testRead(u8 add)
// {
//     Debug(add);
//     if(ReadCmd(add,&test))
//     {
//         Debug(test);
//     }else
//         Debug(0xE0);
// }

/**
 * @description: 系统停止
 * @param {*}
 * @return {*}
 */
void SystemStop()
{
    stopPow();
    stop8812();
    //闪屏提醒
    DisplayOn();
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
    Delay_ms(300);
    refreshDisplay();
    Delay_ms(300);
    clear();
    DisplayOff();
    Delay_ms(3000);
    
    isRunning=0;
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
    //PCON=0;  
    isRunning=1;  
    readyResume=0;
    Delay_ms(2000);
    init8812();
    stopPow();
    DisplayOn();
    curBtPow=1;
}

void init()
{
    //定时器初始化
    TIMER0_initialize();    
    TR0  = 1;
    TR1  = 1;
    //EX0 = 1;        //开启外部中断0
    //IT0 = 1;                //设置外部中断0触发模式:下降沿触发

    IEKBI = (d_KBIIE);              //Enable KBI Interrupt Function
    //KBD   = (d_KBDEN<<7)|(d_KBDS);  //Enable KBI De-bounce and Select De-bounce Time Function
    KBLS  = (d_KBLS);               //KBI Input High/Low Level Select
    KBE   = (d_KBEX);               //KBI Input Channel Enable



    EA   = 1;//中断开启

    //屏幕初始化
    Initial();
    clear();
    
    //8812初始化    
    init8812();
    DisplayChar_b(curVolt);

}


void KBI_ISR(void) interrupt KBI_VECTOR //KBI Interrupt Subroutine
{
    switch(KBF)         //Decision Occur Channel Flag (KBF)
    {
        
    case 0x01:          //KBI Channel 0 Occur Interrupt(KBF0)
        //休眠状态中,如果不按电源键,只是中断不唤醒设备
        if(isRunning==0 && BT_POW==0)
        {
            readyResume=1;
        }
        break;
    } 
    KBF=0;
    ////KBIIF=0;            //Hardware Clear KBI Flag
}

void refreshDisplay()
{
    u32 diffTime=GetSysTick()-refreshTime;
    if(diffTime>300)
    {        
        refreshTime=GetSysTick();

        if(isDisplay==0)
        {
            //关屏的时候充电显示电池图标，大于等于100ma的时候才算充电。
            if(isOtg==0 && curIBus>=20)
            {
                if(tempDisplay==0)
                {
                    DisplayOn();
                    clear();
                    isDisplay=0;  
                    tempDisplay=1; 
                }             
                DisplayBat(255);
            }else
            {
                //关屏充电，拔掉适配器之后重新关闭屏幕
                if(tempDisplay==1)
                    DisplayOff();
            }
        }else
        {            
            if(isOtg==0 && curIBus>=20)
            {
                DisplayBat(255);
            }            
            else
            {
                getVbatCount--;
                if(getVbatCount<200)
                {
                    getVbatCount=255;
                    curVBat=GetVBatAvg();
                }
                DisplayBat(curVBat);
            }                
            //放电才显示电流
            if(isOtg==1)
            {
                DisplayChar_s(curIBus);
            }else
            {
                DisplayChar_s(0);
            }
            
            DisplayChar_b(curVolt);
            DisplayShan_s(forcePow);
        }
    }
    diffTime=GetSysTick()-refreshIBusTime;
    if(diffTime>100)
    {   
        refreshIBusTime=GetSysTick();  
        //GetIBusAvg();   
        curIBus=GetIBusAvg();
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
    if(isOtg==1)
    {
        stopPow();
    }
    else 
    {
        //屏幕有显示，且没有充电的时候才打开
        if(isDisplay==1 && curIBus<10)
        {
            startPow();
        }
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
    u32 diffTime;
    if(isRunning)
    {
        while(BT_POW==0 && BT_MIN==0)
        {
            diffTime=GetSysTick()-clickTime;
            if(diffTime>3000)
            {
                SystemStop();
                break;
            }
        }
    }
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
        else
        {
            if(isDisplay)
            {
                stopPow();
                DisplayOff();
                Write_EEPROM(6,forcePow);
                Write_EEPROM(7,curVolt);
            }
            else
            {
                stopPow();
                DisplayOn();
                refreshTime=0;
                refreshDisplay();
                refreshTime=0;
                refreshDisplay();
                refreshTime=0;
                refreshDisplay();
                refreshTime=0;
                refreshDisplay();
                refreshTime=0;
                refreshDisplay();
            }

        } 
        
        curBtPow=1;       
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
            if(diffTime>200)            
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
            if(diffTime>200)            
                minClickLong();            
        }
    }else
    {
        //释放的时候才触发电源按钮
        if(curBtPow==0)
        {
            powClick();
        }
        curBtPow=1;
        curBtMin=1;
        curBtAdd=1;
    }
}

/**
 * @description: 检测休眠
 * @param {*}
 * @return {*}
 */
void checkSleep()
{
    u32 curTime=GetSysTick();
    if(isRunning==0)
    {
        if(readyResume==1)
        {
            SystemResume();
        }
        else
        {
            PCON=2;
        }
    }else
    {
        
        if(isOtg==0 && isDisplay)
        {
            if(curTime-clickTime > 300000)
            {
                DisplayOff();
            }
        }
    }
}

void main()
{
    init();

    Delay_ms(100);  

    num=0;
    curVBat=GetVBatAvg();
    curVBat=GetVBatAvg();
    curVBat=GetVBatAvg();
    curVBat=GetVBatAvg();
    curVBat=GetVBatAvg();
    curVBat=GetVBatAvg();
    refreshTime=GetSysTick();
    refreshIBusTime=GetSysTick();
    while(1)
    {         
        checkSleep();
        procClick();
        refreshDisplay();
        //Delay_ms(100);
        // if(isRunning==0)
        // {
        //     DisplayOn();
        //     refreshDisplay();
        // }
        //checkSleep();
    }
}

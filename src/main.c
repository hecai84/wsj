/*
 * @Description: 
 * @Author: hecai
 * @Date: 2021-05-12 10:42:58
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2021-07-11 13:16:49
 * @FilePath: \WSJ\src\main.c
 */
#include "IIC.h"
#include "EEPROM.h"
#include "power.h"
#include "Tools.h"
#include "oled.h"
#include "Timer.h"
#include "WDT.h"

#define KBI_VECTOR  11          //KBI Interrupt Vevtor
#define d_KBLS      0x00        //KBI Low/High level detection selection (0~0x0F)
#define d_KBEX      0x02        //KBI Input Enable (0~0x0F)
#define d_KBDEN     0x01        //KBI De-bounce Function Enable
#define d_KBDS      0x00        //KBD[1:0] KBI De-bounce Time Selection (0~3)
#define d_KBIIE     0x01        //KBI Interrupt Enable bit

#define BT_POW   P0_1
#define BT_ADD  P3_1
#define BT_MIN   P0_0
#define POW_INT P1_6
#define POWIN_CTRL P0_3

u8 isRunning=1,curBtPow=1,curBtMin=1,curBtAdd=1;

u32 clickTime;
u32 refreshTime;
u32 refreshIBusTime;
extern u8 curVolt;
extern u8 isOtg;
extern u8 isDisplay;
u8 tempDisplay;
extern u8 forcePow;



//函数定义--------------------------------
void refreshDisplay();
void powClickLong();
void waitClickUp();
//---------------------------------------


/**
 * @description: 检测充电电源插入
 * @param {*}
 * @return {*}
 */
void checkPowIn()
{
    if(POW_INT==1)
    {
        Delay_ms(5);
        if(POW_INT==1)
        {        
            Delay_ms(5);
            if(POW_INT==1)
            {
                if(isOtg==1)
                {
                    stopPow();
                }
                POWIN_CTRL=1;
                Delay_ms(100);
            }
        }
    }else
    {
        POWIN_CTRL=0;
    }
}

// void KBI_Disable(void)
// {
//     IEKBI = 0;              //Disable KBI Interrupt
//     KBE   = 0;              //Disable KBI Function
// }
// void KBI_Enable(void)
// {
//     IEKBI = (d_KBIIE);              //Enable KBI Interrupt Function
//     KBE   = (d_KBEX);
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
    Delay_ms(1000);
    waitClickUp();
    WDT_Disable();
    curBtPow=1;
    isRunning=0;
    //mcu休眠
    PCON=0x02;
}


/**
 * @description: 系统恢复
 * @param {*}
 * @return {*}
 */
void SystemResume()
{  
    WDT_initialize();
    init8812();
    stopPow();
    DisplayOn();    
    refreshTime=0;
    refreshDisplay();
    Delay_ms(10);
    refreshTime=0;
    refreshDisplay();
    Delay_ms(10);
    refreshTime=0;
    refreshDisplay();
    waitClickUp();
    isRunning=1;
    curBtPow=1;
}

void init()
{
    EA    = 0;                      //Disable All Interrupt Function 
    //看门狗初始化
    if ((RSTS&0x08))            //Decision WDT Occur (WDTF=1)
    {
        RSTS = RSTS&0xF7;       //Clear WDTF (WDT Timer Reset Flag)
        WDT_CountClear();       //Clear WDT Count Subroutine
        WDT_Disable();
        while(1);
    }
    WDT_initialize();           //Call WDT Initial Subroutine
    //定时器初始化
    TIMER0_initialize();   
    TR0  = 1;
    TR1  = 1;
    //EX0 = 1;        //开启外部中断0
    //IT0 = 1;                //设置外部中断0触发模式:下降沿触发


    //Initialize KBI
    IEKBI = (d_KBIIE);              //Enable KBI Interrupt Function
    KBD   = (d_KBDEN<<7)|(d_KBDS);  //Enable KBI De-bounce and Select De-bounce Time Function
    KBLS  = (d_KBLS);               //KBI Input High/Low Level Select
    KBE   = (d_KBEX);               //KBI Input Channel Enable


    EA   = 1;//中断开启

    
    //0.3,0.4,0.5上拉
    P0M0=P0M0 | 0x38;

    //8812初始化    
    init8812();
    DisplayChar_b(curVolt);

    //屏幕初始化
    Delay_ms(3000);
    Initial();
    clear();


    //KBI_Disable();



}


void KBI_ISR(void) interrupt KBI_VECTOR //KBI Interrupt Subroutine
{
    switch(KBF)         //Decision Occur Channel Flag (KBF)
    {
    case 0x08:          //KBI Channel 3 Occur Interrupt(KBF3)
        break;
        
    case 0x04:          //KBI Channel 2 Occur Interrupt(KBF2)
        break;
    
    case 0x02:          //KBI Channel 1 Occur Interrupt(KBF1)
        break;
        
    case 0x01:          //KBI Channel 0 Occur Interrupt(KBF0)
        break;
    } 
    KBF=0;
    ////KBIIF=0;            //Hardware Clear KBI Flag
}

void refreshIBus()
{
    u32 diffTime;
    if(isOtg==1 || POW_INT==1)
    {
        diffTime=GetSysTick()-refreshIBusTime;
        if(diffTime>50)
        {
            refreshIBusTime=GetSysTick();
            UpdateIBusArr();
        }   
    } 
}

void refreshDisplay()
{
    u16 curIBus;
    u8 curVBat;
    u32 diffTime=GetSysTick()-refreshTime;
    if(diffTime>500)
    {        
        refreshTime=GetSysTick();
        curIBus=GetIBusAvg();
        curVBat=GetBatAvg(0);
        if(isDisplay==0)
        {
            //关屏的时候充电显示电池图标，大于等于100ma的时候才算充电。
            if(POW_INT==1)
            {
                if(tempDisplay==0)
                {
                    clear();
                    DisplayOn();
                    isDisplay=0;  
                    tempDisplay=1; 
                }             
                if(curIBus>=20)
                {
                    DisplayBat(255);
                    curVBat=GetBatAvg(1);
                }
                else
                    DisplayBat(4);
                //电源插入的时候不计算空闲时间
                if(BT_MIN==1 && BT_ADD==1 && BT_POW==1)
                {
                    clickTime=GetSysTick();
                } 
            }else
            {
                //关屏充电，拔掉适配器之后重新关闭屏幕 
                if(tempDisplay==1)
                {
                    DisplayOff();
                    tempDisplay=0;   
                }           
            }
        }else
        {            
            if(POW_INT==1)            
            {
                if(curIBus>=20 && isOtg==0)
                {
                    DisplayBat(255);
                    curVBat=GetBatAvg(1);
                }else
                {
                    DisplayBat(4);
                }
                //电源插入的时候不计算空闲时间
                if(BT_MIN==1 && BT_ADD==1 && BT_POW==1)
                {
                    clickTime=GetSysTick();
                }
            }      
            else
            {
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
}

void waitClickUp()
{
    while(BT_ADD==0 || BT_POW==0 || BT_MIN==0)
    {
        WDT_CountClear();
    }
}

void powClick()
{
    if(isOtg==1)
    {
        stopPow();
        POWIN_CTRL=0;
    }
    else 
    {
        //当前处于显示状态,且没有插入充电器
        if(isDisplay==1 && POW_INT==0)
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


void powClickLong()
{
    u32 diffTime;  

    if(isRunning==0)
    {
        SystemResume();
    }else{
        if(isDisplay)
        {
            stopPow();
            DisplayOff();
            tempDisplay=0;
            Write_EEPROM(forcePow,curVolt);
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
        while(BT_POW==0)
        {
            diffTime=GetSysTick()-clickTime;
            if(diffTime>5000)    
            {
                if(isRunning==1)
                {
                    SystemStop();
                    return;
                }
            }  
            
            WDT_CountClear();    
        }
    }                 
        
    curBtPow=1;       
    waitClickUp();

}


void minClick()
{
    VoltMin();
    DisplayChar_b(curVolt);
}
void minClickLong()
{
    while(BT_MIN==0)
    {
        VoltMin();
        DisplayChar_b(curVolt);
        if(BT_ADD==0)
        {
            doubleAddMin();
            waitClickUp();
        }
        Delay_ms(50);   
        WDT_CountClear();       
    }
}
void addClick()
{
    VoltAdd();
    DisplayChar_b(curVolt);
}
void addClickLong()
{        
    while(BT_ADD==0)
    {
        VoltAdd();
        DisplayChar_b(curVolt);
        if(BT_MIN==0)
        {
            doubleAddMin();
            waitClickUp();
        }
        Delay_ms(50);     
        WDT_CountClear();        
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
            addClick();
            clickTime=GetSysTick();
        }else
        {
            diffTime=GetSysTick()-clickTime;
            if(diffTime>300)            
                addClickLong();            
        }
    }else if(BT_MIN==0)
    {
        if(curBtMin==1)
        {
            curBtMin=0;
            minClick();
            clickTime=GetSysTick();
        }else
        {
            diffTime=GetSysTick()-clickTime;
            if(diffTime>300)            
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
        if(BT_POW==1)
            PCON=0x02;
    }else
    {
        
        if(isOtg==0 && isDisplay)
        {
            if(curTime-clickTime > 1800000)
            {
                DisplayOff();
            }
        }
        if(isDisplay==0)
        {
            if(curTime-clickTime > 10800000)
            {
                SystemStop();
            }
        }
    }
}

void main()
{
    init();
    Delay_ms(100);  

    refreshTime=GetSysTick();
    refreshIBusTime=GetSysTick();
    while(1)
    {         
        checkPowIn();
        checkSleep();
        procClick();
        refreshIBus();
        refreshDisplay();
        WDT_CountClear();
        //Delay_ms(100);
        // if(isRunning==0)
        // {
        //     DisplayOn();
        //     refreshDisplay();
        // }
        //checkSleep();
    }
}


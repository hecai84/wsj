/*
 * @Description: 
 * @Author: hecai
 * @Date: 2021-06-13 04:34:13
 * @LastEditTime: 2021-06-16 18:50:46
 * @FilePath: \wsj\src\WDT.c
 */
#include "OB39R16A3.h"
#include "WDT.h"

#define d_WDTE      1           //WDT Enable Bit
#define d_WDTM      0x0A        //WDTM[3:0] WDT Clock Source Divider Bit (0~F)
#define d_CWDTR     0        //if set 0: watchdog reset, set 1: watchdog interrupt
#define d_IEWDT     0        //WDT interrupt enable if set 0: Disable WDT interrupt, set 1: Enable WDT interrupt



void WDT_initialize(void)    //Initialize WDT
{
    TAKEY = 0x55;                   //**************************************//
    TAKEY = 0xAA;                   //Set To Enable The WDTC Write Attribute//
    TAKEY = 0x5A;                   //**************************************//
    WDTC  = (d_WDTM)|(d_WDTE<<5)|(d_CWDTR<<6);    //Set WDT Reset Time and Enable WDT and select RST/Interrupt
    IEN2  = (d_IEWDT<<1);           //for WDT Interrupt 
}

void WDT_CountClear(void)
{
    WDTK = 0x55;                //WDT Timer Clear To Zero
}

void WDT_Disable(void)
{
    TAKEY = 0x55;               //**************************************//
    TAKEY = 0xAA;               //Set To Enable The WDTC Write Attribute//
    TAKEY = 0x5A;               //**************************************//
    WDTC  = 0x00;               //Disable WDT Function
}



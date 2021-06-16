/*
 * @Description: 
 * @Author: hecai
 * @Date: 2021-06-13 04:34:13
 * @LastEditTime: 2021-06-16 18:32:41
 * @FilePath: \wsj\src\WDT.h
 */
/*--------------------------------------------------------------------------
Header file for WDT.
Modify on 2018/01/09
--------------------------------------------------------------------------*/

#ifndef __WDT_H__
#define __WDT_H__



void WDT_initialize(void);
void WDT_CountClear(void);
void WDT_Disable(void);
void WDT_ON();
void WDT_OFF();
#endif

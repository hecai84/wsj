/*
 * @Author: your name
 * @Date: 2021-04-18 09:32:57
 * @LastEditTime: 2021-05-14 01:03:22
 * @LastEditors: huzhenhong
 * @Description: In User Settings Edit
 * @FilePath: \WSJ\src\power.h
 */
#ifndef __POWER_H__
#define __POWER_H__
#include "Tools.h"
void stop8812(void);
void init8812(void);
void startPow(void);
u8 GetIBus();
u8 GetIBusAvg();
u8 GetVBatAvg();
//u8 GetIBatAvg();
void SetVolt(u8 v);
u8 ReadCmd(u8 addr,u8 * dat);
u8 WriteCmd(u8 addr,u8 dat);
void VoltMin();
void VoltAdd();
void stopPow(void);
#endif

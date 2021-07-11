/*
 * @Author: your name
 * @Date: 2021-04-18 09:32:57
 * @LastEditTime: 2021-07-10 22:59:35
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \WSJ\src\power.h
 */
#ifndef __POWER_H__
#define __POWER_H__
#include "Tools.h"
void stop8812(void);
void init8812(void);
void startPow(void);
u16 GetIBus();
u16 GetIBusAvg();
void UpdateIBusArr();
u8 GetBatAvg(u8 update);
//u8 GetIBatAvg();
void SetVolt(u8 v);
u8 ReadCmd(u8 addr,u8 * dat);
u8 WriteCmd(u8 addr,u8 dat);
void VoltMin();
void VoltAdd();
void stopPow(void);
#endif

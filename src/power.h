/*
 * @Author: your name
 * @Date: 2021-04-18 09:32:57
 * @LastEditTime: 2023-10-20 10:45:06
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \wsj\src\power.h
 */
#ifndef __POWER_H__
#define __POWER_H__
#include "Tools.h"

extern u16 curBatVolt;

void stop8812(void);
void init8812(void);
void loadConfig(void);
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
void pause8812(void);
void resume8812(void);
#endif

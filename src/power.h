/*
 * @Author: your name
 * @Date: 2021-04-18 09:32:57
 * @LastEditTime: 2023-12-27 17:53:47
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \wsj\src\power.h
 */
#ifndef __POWER_H__
#define __POWER_H__
#include "Tools.h"

extern u16 curBatVolt;

#if defined(B2_1000MA)
#define BATNUM 2
#define IBAT_LIM_SET 0x3F
#elif defined(B2_800MA)
#define BATNUM 2
#define IBAT_LIM_SET 0x32
#elif defined(B2_600MA)
#define BATNUM 2
#define IBAT_LIM_SET 0x26
#else
#define BATNUM 1
#define IBAT_LIM_SET 0x26
#endif

// 配置单节或者双节电池
#define BATNUM 2
#if (BATNUM == 1)
#define VBAT_SET 0x01
#else
#define VBAT_SET 0x09
#endif


void stop8812(void);
void init8812(void);
void loadConfig(void);
void startPow(void);
u16 GetIBus();
u16 GetIBusAvg();
void UpdateIBusArr();
u8 GetBatAvg(u8 update);
// u8 GetIBatAvg();
void SetVolt(u8 v);
u8 ReadCmd(u8 addr, u8 *dat);
u8 WriteCmd(u8 addr, u8 dat);
void VoltMin();
void VoltAdd();
void stopPow(void);
void pause8812(void);
void resume8812(void);
#endif

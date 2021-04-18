/*
 * @Author: your name
 * @Date: 2021-04-18 09:32:57
 * @LastEditTime: 2021-04-18 16:35:49
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \WSJ\src\power.h
 */
#ifndef __POWER_H__
#define __POWER_H__

void initPow(void);
u8 ReadCmd(u8 addr,u8 * dat);
u8 WriteCmd(u8 addr,u8 dat);
#endif

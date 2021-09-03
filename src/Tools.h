/*
 * @Author: your name
 * @Date: 2021-04-16 23:27:16
 * @LastEditTime: 2021-08-30 14:53:30
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \wsj\src\Tools.h
 */
#ifndef __TOOLS_H__
#define __TOOLS_H__


#include "OB39R16A3.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned long u32;

#ifdef __VSCODE__
#endif

void Debug(u8 dat);
void Delay_ms(u16 n);
void Delay_10us(u8 n);

#endif
